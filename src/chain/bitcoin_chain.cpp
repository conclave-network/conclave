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

#include "../util/hex.h"
#include "../util/json.h"
#include "../script.h"
#include "bitcoin_chain.h"
#include <stdexcept>

namespace conclave
{
    namespace chain
    {
        BitcoinChain::BitcoinChain(const BitcoinChainConfig& bitcoinChainConfig)
            : electrumxClient(ElectrumxClient(bitcoinChainConfig.getElectrumxClientConfig()))
        {
        }
        
        const uint64_t BitcoinChain::getAddressBalance(const Address& address)
        {
            Script p2shScript = Script::p2hScript(address);
            Hash256 scriptHash = p2shScript.getSingleSHA256();
            pt::ptree tree = electrumxClient.blockchainScripthashGetBalance(scriptHash.reversed());
            return getPrimitiveFromJson<uint64_t>(tree, "confirmed");
        }
        
        const std::vector<BitcoinRichOutput> BitcoinChain::getUtxos(const Address& address)
        {
            return {};//TODO
        }
        
        const BitcoinTx BitcoinChain::getTx(const Hash256& txId)
        {
            pt::ptree tree = electrumxClient.blockchainTransactionGet(txId, true);
            return BitcoinTx(HEX_TO_BYTE_VECTOR(getPrimitiveFromJson<std::string>(tree, "result")));
        }
        
        const Hash256 BitcoinChain::submitTx(const BitcoinTx& bitcoinTx)
        {
            pt::ptree tree = electrumxClient
                .blockchainTransactionBroadcast(BYTE_VECTOR_TO_HEX(bitcoinTx.serialize()));
            Hash256 txId = Hash256(tree.data());
            std::cout << "returned " << txId << std::endl;
            // Make sure returned TxId is same as our calculated TxId
            if (txId != bitcoinTx.getHash256()) {
                throw std::runtime_error("ElectrumX server returned different TxId");
            }
            return txId;
        }
        
        const bool BitcoinChain::txIsConfirmed(const Hash256& txid)
        {
            return true; //TODO
        }
        
        const bool BitcoinChain::outputIsConclaveOwned(const Outpoint& outpoint)
        {
            return true; //TODO
        }
        
        const Hash256 BitcoinChain::getLatestBlockHash()
        {
            return Hash256(); // TODO
        }
    }
}
