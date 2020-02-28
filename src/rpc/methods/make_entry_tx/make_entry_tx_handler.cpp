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

#include "make_entry_tx_request.h"
#include "make_entry_tx_response.h"
#include "structs/sources.h"
#include "structs/destinations.h"
#include "../../../structs/bitcoin_tx.h"
#include "../../../structs/conclave_entry_tx.h"
#include "../../../conclave_node.h"
#include <vector>

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace make_entry_tx
            {
                std::vector<BitcoinInput> makeBitcoinInputs(const Sources& sources)
                {
                    std::vector<BitcoinInput> inputs;
                    for (const Outpoint& outpoint:sources.outpoints) {
                        inputs.emplace_back(BitcoinInput(outpoint, Script(), 0xffffffff));
                    }
                    return inputs;
                }
                
                std::vector<BitcoinOutput> makeBitcoinOutputs(const Destinations& destinations)
                {
                    std::vector<BitcoinOutput> outputs;
                    // Add bitcoin change outputs
                    for (const Destination& destination: destinations.bitcoinDestinations) {
                        Script scriptPubKey = Script::p2hScript(destination.address);
                        uint64_t value = destination.value;
                        outputs.emplace_back(BitcoinOutput(scriptPubKey, value));
                    }
                    return outputs;
                }
                
                std::vector<BitcoinOutput> makeConclaveOutputs(const Destinations& destinations)
                {
                    std::vector<BitcoinOutput> outputs;
                    for (const Destination& destination: destinations.conclaveDestinations) {
                        Script scriptPubKey = Script::p2hScript(destination.address);
                        uint64_t value = destination.value;
                        outputs.emplace_back(BitcoinOutput(scriptPubKey, value));
                    }
                    return outputs;
                }
                
                MakeEntryTxResponse* makeEntryTxHandler(const MakeEntryTxRequest& makeEntryTxRequest,
                                                        ConclaveNode& conclaveNode)
                {
                    const Sources sources = makeEntryTxRequest.getSources();
                    const Destinations destinations = makeEntryTxRequest.getDestinations();
                    BitcoinTx bitcoinTx(makeBitcoinInputs(sources), makeBitcoinOutputs(destinations), 2, 0);
                    ConclaveEntryTx conclaveEntryTx(makeConclaveOutputs(destinations));
                    return new MakeEntryTxResponse(bitcoinTx, conclaveEntryTx);
                }
            }
        }
    }
}
