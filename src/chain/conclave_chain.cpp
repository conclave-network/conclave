/*
 * CONCLAVE - Making Bitcoin Scale And Be Useful
 * Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>
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
            const Hash256 txId = conclaveTx.getHash256();
            
            // Check if tx is already on the blockchain
            if (txIsOnBlockchain(txId)) {
                std::cout << "tx " << txId << " already on blockchain" << std::endl;
                return txId;
            }
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
                    throw std::runtime_error(
                        "Can not find transaction: " + static_cast<std::string>(fundTip->txId));
                }
                if (conclaveTx->conclaveOutputs.size() <= fundTip->index) {
                    throw std::runtime_error("Output index out of bounds" + static_cast<std::string>(*fundTip));
                }
                ConclaveOutput output = conclaveTx->conclaveOutputs[fundTip->index];
                fundTotal += output.value;
                fundTip = output.predecessor;
            }
            return fundTotal;
        }
        
        const uint64_t ConclaveChain::countSpendTotal(const Hash256& walletHash)
        {
            std::optional<Inpoint> spendTip = databaseClient.getMutableItem(COLLECTION_SPEND_TIPS, walletHash);
            uint64_t spendTotal = 0;
            while (spendTip.has_value()) {
                break; // TODO
            }
            return spendTotal;
        }
        
        const bool ConclaveChain::txIsOnBlockchain(const Hash256& txId)
        {
            return databaseClient.getItem(txId).has_value();
        }
        
        const Hash256 ConclaveChain::processClaimTx(ConclaveTx claimTx)
        {
            // TODO: ensure claimTx looks like a valid claim tx as much as possible
            // from inspecting claimTx structure alone before entering this function.
            const Outpoint fundPoint = *claimTx.fundPoint;
            const Script claimScript = claimTx.getClaimScript();
            const Hash256 claimScriptHash = claimScript.getHash256();
            const BitcoinTx fundTx = bitcoinChain.getTx(fundPoint.txId);
            const BitcoinOutput fundOutput = fundTx.outputs[fundPoint.index];
            
            // Ensure scriptPubKey is a P2WSH encumbrance which pays to
            // our claim script, thus to our claim transaction
            const std::optional<Hash256> redeemScriptHash = fundOutput.scriptPubKey.getP2wshHash();
            if (!redeemScriptHash.has_value() || *redeemScriptHash != claimScriptHash) {
                throw std::runtime_error("redeem script hash does not match claim script hash");
            }
            
            // Ensure claimTx claims no more than the claimable value
            const uint64_t claimableValue = fundOutput.value;
            const uint64_t claimedValue = claimTx.getClaimedValue();
            if (claimableValue < claimedValue) {
                throw std::runtime_error("claim tx claims too much value");
            }
            const Hash256 claimTxHashSansPredecessors = claimTx.getHash256();
            
            // Update fundTips
            for (uint64_t i = 0; i < claimTx.conclaveOutputs.size(); i++) {
                ConclaveOutput& conclaveOutput = claimTx.conclaveOutputs[i];
                const Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                const std::optional<Outpoint> prevFundTip =
                    databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
                if (prevFundTip.has_value()) {
                    if (prevFundTip->txId == claimTxHashSansPredecessors && prevFundTip->index == i) {
                        throw std::runtime_error("already have this fundTip in db");
                    }
                    conclaveOutput.predecessor = prevFundTip;
                }
                const Outpoint newFundTip(claimTx.getHash256(), i);
                databaseClient.putMutableItem(COLLECTION_FUND_TIPS, walletHash, newFundTip);
            }
            // Store the transaction in database
            databaseClient.putItem(claimTx);
            return claimTx.getHash256();
        }
        
        const Hash256 ConclaveChain::processTx(ConclaveTx conclaveTx)
        {
            return conclaveTx.getHash256();
        }
    }
}
