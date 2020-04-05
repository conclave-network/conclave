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
            const uint64_t fundTotal = countFundTotal(walletHash);
            const uint64_t spendTotal = countSpendTotal(walletHash);
            return fundTotal - spendTotal;
        }
        
        void ConclaveChain::submitEntryTx(const EntryTx& entryTx)
        {
            BitcoinTx fundTx = entryTx.fundTx;
            ConclaveClaimTx claimTx = entryTx.claimTx;
            
            // Check if the funding transaction has been confirmed or not
            if (!bitcoinChain.txIsConfirmed(fundTx.getHash256())) {
                bitcoinChain.submitTx(fundTx);
            } else if (!bitcoinChain.outputIsConclaveOwned(*claimTx.fundingOutpoint)) {
                throw std::runtime_error(std::string("Output is not spendable: "));
            } else {
                // Tx is confirmed and output is spendable
                // Sweep 1: update predecessors to most recent fund tips
                for (uint32_t i = 0; i < claimTx.outputs.size(); i++) {
                    ConclaveOutput& conclaveOutput = claimTx.outputs[i];
                    Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                    // Set predecessor to fundTip (or nullopt)
                    conclaveOutput.predecessor = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
                }
                
                // Get existing chain tip
                ConclaveBlock oldTip = getChainTip();
                
                // Make new chain tip
                ConclaveBlock newTip = ConclaveBlock(
                    0,                                       // pot
                    oldTip.height + 1,                       // height
                    0,                                       // epoch
                    oldTip.getHash256(),                     // hashPrevBlock
                    bitcoinChain.getLatestBlockHash(),       // lowestParentBitcoinBlockHash
                    0,                                       // txTypeId
                    0,                                       // txVersion
                    claimTx.getHash256()                     // txHash
                );
                
                // Sweep 2: update fund tips with new outpoints
                for (uint32_t i = 0; i < claimTx.outputs.size(); i++) {
                    Hash256 walletHash = claimTx.outputs[i].scriptPubKey.getHash256();
                    // Update fundTip
                    Outpoint outpoint(claimTx.getHash256(), i);
                    databaseClient.putMutableItem(COLLECTION_FUND_TIPS, walletHash, outpoint);
                }
                databaseClient.putItem(claimTx);
                databaseClient.putItem(newTip);
                databaseClient.putSingletonItem(COLLECTION_CHAIN_TIP, newTip.getHash256());
            }
        }
        
        const Hash256 ConclaveChain::submitStandardTx(const ConclaveStandardTx& conclaveStandardTx)
        {
            return Hash256();
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
            std::optional<Outpoint> fundTip = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
            uint64_t fundTotal = 0;
            while (fundTip.has_value()) {
                /**
                 * Potential for an infinite loop here if there is a graph cycle.
                 * TODO: Do something about it
                 */
                std::optional<ConclaveClaimTx> tx = databaseClient.getItem(fundTip->txId);
                if (!tx.has_value()) {
                    throw std::runtime_error("Can not find transaction: " + static_cast<std::string>(fundTip->txId));
                }
                if (tx->outputs.size() <= fundTip->index) {
                    throw std::runtime_error("Output index out of bounds" + static_cast<std::string>(*fundTip));
                }
                ConclaveOutput output = tx->outputs[fundTip->index];
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
    }
}
