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
            return 4112;
        }
        
        void ConclaveChain::submitEntryTx(const EntryTx& entryTx)
        {
            BitcoinTx fundTx = entryTx.fundTx;
            ConclaveClaimTx claimTx = entryTx.claimTx;
            
            // Check if the funding transaction has been confirmed or not
            if (!bitcoinChain.txIsConfirmed(fundTx.getHash256())) {
                bitcoinChain.submitTx(fundTx);
            } else if (!bitcoinChain.outputIsSpendable(*claimTx.fundingOutpoint)) {
                throw std::runtime_error(std::string("Output is not spendable: "));
            } else {
                // Tx is confirmed and output is spendable
                // TODO: Put claimTx into block
                const Hash256 chainTipHash = getChainTipHash();
                for (uint32_t i = 0; i < claimTx.conclaveOutputs.size(); i++) {
                    ConclaveOutput& conclaveOutput = claimTx.conclaveOutputs[i];
                    Hash256 walletHash = conclaveOutput.scriptPubKey.getHash256();
                    // Set predecessor to fundTip (or nullopt)
                    conclaveOutput.predecessor = databaseClient.getMutableItem(COLLECTION_FUND_TIPS, walletHash);
                    // Update fundTip
                    Outpoint outpoint(claimTx.getHash256(), i);
                    databaseClient.putMutableItem(COLLECTION_FUND_TIPS, walletHash, outpoint);
                }
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
                return Hash256("C07C7A7EC07C7A7EC07C7A7EC07C7A7EC07C7A7EC07C7A7EC07C7A7EC07C7A7E");
            }
        }
    }
}
