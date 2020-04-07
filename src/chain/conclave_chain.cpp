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
        
        void ConclaveChain::submitEntryTx(const EntryTx& entryTx)
        {
        }
        
        const Hash256 ConclaveChain::submitTx(const ConclaveTx& conclaveTx)
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
            std::optional <Outpoint> fundTip = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
            uint64_t fundTotal = 0;
            while (fundTip.has_value()) {
                /**
                 * Potential for an infinite loop here if there is a graph cycle.
                 * TODO: Do something about it
                 */
                std::optional <ConclaveTx> tx = databaseClient.getItem(fundTip->txId);
                if (!tx.has_value()) {
                    throw std::runtime_error("Can not find transaction: " + static_cast<std::string>(fundTip->txId));
                }
                if (tx->conclaveOutputs.size() <= fundTip->index) {
                    throw std::runtime_error("Output index out of bounds" + static_cast<std::string>(*fundTip));
                }
                ConclaveOutput output = tx->conclaveOutputs[fundTip->index];
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
