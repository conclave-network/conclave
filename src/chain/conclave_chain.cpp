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
            // Check if the funding transaction has been confirmed or not
            if (!bitcoinChain.txIsConfirmed(entryTx.fundTx.getHash256())) {
                bitcoinChain.submitTx(entryTx.fundTx);
            } else if (!bitcoinChain.outputIsSpendable(entryTx.claimTx.fundingOutpoint.value())) {
                throw std::runtime_error(std::string("Output is not spendable: "));
            } else {
                // Output is spendable
                appendToChain(entryTx.claimTx);
            }
        }
        
        const Hash256 ConclaveChain::submitStandardTx(const ConclaveStandardTx& conclaveStandardTx)
        {
            return Hash256();
        }
        
        void ConclaveChain::appendToChain(const ConclaveTx& conclaveTx)
        {
        }
    }
}
