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

namespace conclave
{
    namespace chain
    {
        namespace bitcoin
        {
            BitcoinChain::BitcoinChain(const BitcoinChainConfig& bitcoinChainConfig)
                : electrumxClient(ElectrumxClient(bitcoinChainConfig.getElectrumxClientConfig()))
            {
            }
            
            const uint64_t BitcoinChain::getAddressBalance(const Address& address)
            {
                Script p2shScript = Script::p2hScript(address);
                Hash256 scriptHash = p2shScript.getSingleSHA256();
                pt::ptree res = electrumxClient.blockchainScripthashGetBalance(scriptHash.reversed());
                return getPrimitiveFromJson<uint64_t>(res, "confirmed");
            }
            
            const Hash256 BitcoinChain::submitTx(const BitcoinTx& bitcoinTx)
            {
                return Hash256(); // TODO!
            }
        };
    }
}
