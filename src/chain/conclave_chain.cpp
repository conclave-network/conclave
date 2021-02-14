/*
 * CONCLAVE - Scaling Bitcoin Simply.
 * Copyright (C) 2019-2021 Conclave development team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "conclave_chain.h"
#include "../private_key.h"

namespace conclave
{
    namespace chain
    {
        //
        // Genesis
        //
        
        const ConclaveBlock ConclaveChain::GENESIS_BLOCK(
            0,
            0,
            0,
            Hash256("0000000000000000000000000000000000000000000000000000000000000000"),
            Hash256("0000000000000000000ec9fb4c1ddcfd51b366278a1bdddb7dbee1e9a1aba654"),
            0,
            0,
            Hash256("0000000000000000000000000000000000000000000000000000000000000000")
        );
        
        //
        // Collection Names
        //
        
        const std::string ConclaveChain::COLLECTION_CHAIN_TIP = "ChainTip";
        const std::string ConclaveChain::COLLECTION_CLAIMS = "Claims";
        const std::string ConclaveChain::COLLECTION_SPENDS = "Spends";
        const std::string ConclaveChain::COLLECTION_SPEND_TIPS = "SpendTips";
        const std::string ConclaveChain::COLLECTION_FUND_TIPS = "FundTips";
        
        
        //
        // Constructors
        //
        
        ConclaveChain::ConclaveChain(const ConclaveChainConfig& conclaveChainConfig, BitcoinChain& bitcoinChain)
            : bitcoinChain(bitcoinChain), databaseClient(DatabaseClient(conclaveChainConfig.getDatabaseClientConfig()))
        {
        }
        
        const uint64_t ConclaveChain::getAddressBalance(const Address& address)
        {
            const Hash256 walletHash = Script::p2hScript(address).getHash256();
            return countFundTotal(walletHash) - countSpendTotal(walletHash);
        }
        
        const std::vector<ConclaveRichOutput> ConclaveChain::getUtxos(const Address& address)
        {
            const Hash256 walletHash = Script::p2hScript(address).getHash256();
            std::vector<ConclaveRichOutput> utxos;
            std::optional<Outpoint> fundTip = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
            while (fundTip.has_value()) {
                std::optional<ConclaveTx> conclaveTx = databaseClient.getItem(fundTip->txId);
                CONCLAVE_ASSERT(conclaveTx.has_value(),
                                "can not find transaction: " + std::string(fundTip->txId));
                CONCLAVE_ASSERT(fundTip->index <= conclaveTx->conclaveOutputs.size(),
                                "index out of range: " + std::to_string(fundTip->index));
                ConclaveOutput& conclaveOutput = conclaveTx->conclaveOutputs[fundTip->index];
                CONCLAVE_ASSERT(conclaveOutput.scriptPubKey.getHash256() == walletHash,
                                "wallet hash does not match hash of scriptPubKey");
                utxos.emplace_back(ConclaveRichOutput(
                    *fundTip, ConclaveOutput(conclaveOutput.scriptPubKey, conclaveOutput.value)));
                fundTip = conclaveOutput.predecessor;
            }
            return utxos;
        }
        
        const Hash256 ConclaveChain::submitTx(const ConclaveTx& conclaveTx)
        {
            if (conclaveTx.isClaimTx()) {
                return processClaimTx(conclaveTx);
            } else {
                return processTx(conclaveTx);
            }
        }
        
        const Hash256 ConclaveChain::getChainTipHash()
        {
            std::optional<Hash256> chainTipHash = databaseClient.getSingletonItem(COLLECTION_CHAIN_TIP);
            if (chainTipHash.has_value()) {
                return *chainTipHash;
            } else {
                // "Genesis" hash
                return GENESIS_BLOCK.getHash256();
            }
        }
        
        const ConclaveBlock ConclaveChain::getChainTip()
        {
            const Hash256 chainTipHash = getChainTipHash();
            std::optional<ConclaveBlock> chainTipBV = databaseClient.getItem(chainTipHash);
            if (chainTipBV.has_value()) {
                return *chainTipBV;
            } else {
                return GENESIS_BLOCK;
            }
        }
        
        const uint64_t ConclaveChain::countFundTotal(const Hash256& walletHash)
        {
            uint64_t fundTotal = 0;
            std::optional<Outpoint> fundTip = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
            while (fundTip.has_value()) {
                // Potential for an infinite loop here if there is a graph cycle.
                // TODO: Do something about it
                std::optional<ConclaveTx> conclaveTx = databaseClient.getItem(fundTip->txId);
                if (!conclaveTx.has_value()) {
                    throw std::runtime_error("can not find transaction: " + static_cast<std::string>(fundTip->txId));
                }
                if (conclaveTx->conclaveOutputs.size() <= fundTip->index) {
                    throw std::runtime_error("output index out of bounds" + static_cast<std::string>(*fundTip));
                }
                ConclaveOutput& conclaveOutput = conclaveTx->conclaveOutputs[fundTip->index];
                fundTotal += conclaveOutput.value;
                fundTip = conclaveOutput.predecessor;
            }
            return fundTotal;
        }
        
        const uint64_t ConclaveChain::countSpendTotal(const Hash256& walletHash)
        {
            uint64_t spendTotal = 0;
            std::optional<Inpoint> spendTip = databaseClient.getMutableItem(COLLECTION_SPEND_TIPS, walletHash);
            while (spendTip.has_value()) {
                // Potential for an infinite loop here if there is a graph cycle.
                // TODO: Do something about it
                std::optional<ConclaveTx> conclaveTx = databaseClient.getItem(spendTip->txId);
                if (!conclaveTx.has_value()) {
                    throw std::runtime_error("can not find transaction: " + static_cast<std::string>(spendTip->txId));
                }
                if (conclaveTx->conclaveInputs.size() <= spendTip->index) {
                    throw std::runtime_error("input index out of bounds" + static_cast<std::string>(*spendTip));
                }
                ConclaveInput& conclaveInput = conclaveTx->conclaveInputs[spendTip->index];
                Outpoint& outpoint = conclaveInput.outpoint;
                std::optional<ConclaveTx> prevConclaveTx = databaseClient.getItem(outpoint.txId);
                if (!prevConclaveTx.has_value()) {
                    throw std::runtime_error("can not find transaction: " + static_cast<std::string>(outpoint.txId));
                }
                if (prevConclaveTx->conclaveOutputs.size() <= outpoint.index) {
                    throw std::runtime_error("output index out of bounds" + static_cast<std::string>(outpoint));
                }
                ConclaveOutput& conclaveOutput = prevConclaveTx->conclaveOutputs[outpoint.index];
                spendTotal += conclaveOutput.value;
                spendTip = conclaveInput.predecessor;
            }
            return spendTotal;
        }
        
        const bool ConclaveChain::txIsOnBlockchain(const Hash256& txId)
        {
            return databaseClient.getItem(txId).has_value();
        }
        
        const Hash256 ConclaveChain::processClaimTx(ConclaveTx claimTx)
        {
            const Hash256 initialTxId = claimTx.getHash256();
            const Outpoint fundPoint = *claimTx.fundPoint;
            const Hash256 fundPointHash = fundPoint.getHash256();
            
            // Ensure fundPoint hasn't already been claimed
            if (databaseClient.getMutableItem(COLLECTION_CLAIMS, fundPointHash).has_value()) {
                throw std::runtime_error("fundPoint already claimed");
            }
            
            // Attempt to get fundTx
            const std::optional<BitcoinTx> fundTx = bitcoinChain.getTx(fundPoint.txId);
            if (!fundTx.has_value()) {
                throw std::runtime_error("fundTx not found");
            }
            
            // Ensure claimTx claims no more than the claimable value
            const BitcoinOutput fundOutput = fundTx->outputs[fundPoint.index];
            if (fundOutput.value < claimTx.getTotalOutputValue()) {
                throw std::runtime_error("claim tx claims too much value");
            }
            
            // Ensure scriptPubKey is a P2WSH encumbrance which pays to
            // our claim script, thus to our claim transaction
            const Script claimScript = claimTx.getClaimScript();
            const Hash256 claimScriptHash = claimScript.getSingleSHA256();
            const std::optional<Hash256> redeemScriptHash = fundOutput.scriptPubKey.getP2wshHash();
            if (!redeemScriptHash.has_value() || *redeemScriptHash != claimScriptHash) {
                throw std::runtime_error("redeem script hash does not match claim script hash");
            }
            
            // Update outpoint predecessors
            // TODO: Currently this does not work properly if there are 2 or more outputs in the
            // same tx paid to the same wallet. fix it.
            for (uint64_t i = 0; i < claimTx.conclaveOutputs.size(); i++) {
                ConclaveOutput& conclaveOutput = claimTx.conclaveOutputs[i];
                const Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                const std::optional<Outpoint> fundTip = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
                if (fundTip.has_value()) {
                    const bool fundTipPointsToThisTx = (fundTip->txId == initialTxId && fundTip->index == i);
                    CONCLAVE_ASSERT(!fundTipPointsToThisTx, "fund tip points to this transaction");
                    conclaveOutput.predecessor = fundTip;
                }
            }
            
            // txId is now final
            const Hash256 finalTxId = claimTx.getHash256();
            
            // Ensure tx is not yet on the blockchain
            CONCLAVE_ASSERT(!txIsOnBlockchain(finalTxId), "transaction is already on blockchain");
            
            // Update fund tips
            for (uint64_t i = 0; i < claimTx.conclaveOutputs.size(); i++) {
                ConclaveOutput& conclaveOutput = claimTx.conclaveOutputs[i];
                const Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                const Outpoint newFundTip(finalTxId, i);
                databaseClient.putMutableItem(COLLECTION_FUND_TIPS, walletHash, newFundTip);
            }
            
            // Claim the fundPoint
            databaseClient.putMutableItem(COLLECTION_CLAIMS, fundPointHash, finalTxId);
            
            // Store the transaction
            databaseClient.putItem(claimTx);
            return finalTxId;
        }
        
        const Hash256 ConclaveChain::processTx(ConclaveTx conclaveTx)
        {
            const Hash256 initialTxId = conclaveTx.getHash256();
            
            // Verify that each referenced output is either still spendable or
            // already marked as spent by this transaction at the same inpoint.
            for (uint64_t i = 0; i < conclaveTx.conclaveInputs.size(); i++) {
                const Outpoint& outpoint = conclaveTx.conclaveInputs[i].outpoint;
                const std::optional<Inpoint> inpoint =
                    databaseClient.getMutableItem(COLLECTION_SPENDS, outpoint.getHash256());
                if (inpoint.has_value()) {
                    throw std::runtime_error("outpoint already spent: " + std::string(outpoint));
                }
            }
            
            // Gather previous outputs and sum up the spendable value
            uint64_t spendableValue = 0;
            std::vector<ConclaveOutput> prevOutputs;
            prevOutputs.reserve(conclaveTx.conclaveInputs.size());
            for (uint64_t i = 0; i < conclaveTx.conclaveInputs.size(); i++) {
                const Outpoint& outpoint = conclaveTx.conclaveInputs[i].outpoint;
                const std::optional<ConclaveTx> prevTx = databaseClient.getItem(outpoint.txId);
                if (!prevTx.has_value()) {
                    throw new std::runtime_error("can not find previous tx");
                }
                if (prevTx->conclaveOutputs.size() <= outpoint.index) {
                    throw new std::runtime_error("index out of range");
                }
                const ConclaveOutput& prevOutput = prevTx->conclaveOutputs[outpoint.index];
                spendableValue += prevOutput.value;
                prevOutputs.emplace_back(prevOutput);
            }
            
            // Ensure tx spends no more than the spendable value
            const uint64_t spentValue = conclaveTx.getTotalOutputValue();
            if (spendableValue < spentValue) {
                throw std::runtime_error("tx spends too much value");
            }
            
            // TODO: Check signatures
            
            // Update inpoint predecessors
            // TODO: Currently this does not work properly if there are 2 or more inputs in the
            // same tx paid from the same wallet. fix it.
            for (uint64_t i = 0; i < conclaveTx.conclaveInputs.size(); i++) {
                const Hash256 walletHash = prevOutputs[i].scriptPubKey.getHash256();
                const std::optional<Inpoint> spendTip =
                    databaseClient.getMutableItem(COLLECTION_SPEND_TIPS, walletHash);
                if (spendTip.has_value()) {
                    const bool spendTipPointsToThisTx = (spendTip->txId == initialTxId && spendTip->index == i);
                    CONCLAVE_ASSERT(!spendTipPointsToThisTx, "spend tip points to this transaction");
                    conclaveTx.conclaveInputs[i].predecessor = spendTip;
                }
            }
            
            // Update outpoint predecessors
            // TODO: Currently this does not work properly if there are 2 or more outputs in the
            // same tx paid to the same wallet. fix it.
            for (uint64_t i = 0; i < conclaveTx.conclaveOutputs.size(); i++) {
                ConclaveOutput& conclaveOutput = conclaveTx.conclaveOutputs[i];
                const Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                const std::optional<Outpoint> fundTip = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
                if (fundTip.has_value()) {
                    const bool fundTipPointsToThisTx = (fundTip->txId == initialTxId && fundTip->index == i);
                    CONCLAVE_ASSERT(!fundTipPointsToThisTx, "fund tip points to this transaction");
                    conclaveOutput.predecessor = fundTip;
                }
            }
            
            // txId is now final
            const Hash256 finalTxId = conclaveTx.getHash256();
            
            // Ensure tx is not yet on the blockchain
            CONCLAVE_ASSERT(!txIsOnBlockchain(finalTxId), "transaction is already on blockchain");
            
            // Spend the inputs - update spends and spend tips
            for (uint64_t i = 0; i < conclaveTx.conclaveInputs.size(); i++) {
                ConclaveInput& conclaveInput = conclaveTx.conclaveInputs[i];
                const Outpoint& outpoint = conclaveInput.outpoint;
                const ConclaveOutput& prevOutput = prevOutputs[i];
                const Inpoint spendTip(finalTxId, i);
                const Hash256 walletHash = prevOutput.scriptPubKey.getHash256();
                
                // Update spend
                databaseClient.putMutableItem(COLLECTION_SPENDS, outpoint.getHash256(), spendTip);
                
                // Update spend tip
                databaseClient.putMutableItem(COLLECTION_SPEND_TIPS, walletHash, spendTip);
            }
            
            // Update fund tips
            for (uint64_t i = 0; i < conclaveTx.conclaveOutputs.size(); i++) {
                ConclaveOutput& conclaveOutput = conclaveTx.conclaveOutputs[i];
                const Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                const Outpoint newFundTip(finalTxId, i);
                databaseClient.putMutableItem(COLLECTION_FUND_TIPS, walletHash, newFundTip);
            }
            
            // Process Bitcoin outputs
            // TEMPORARY !!!
            if (conclaveTx.bitcoinOutputs.size() > 0) {
                withdrawOutputs(conclaveTx.bitcoinOutputs);
            }
            
            // Store the transaction in database
            databaseClient.putItem(conclaveTx);
            return finalTxId;
        }
        
        const void ConclaveChain::withdrawOutputs(const std::vector<BitcoinOutput>& withdrawalOutputs)
        {
            //TODO
        }
    }
}
