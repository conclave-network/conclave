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
#include "../../../structs/conclave_tx.h"
#include "../../../conclave_node.h"
#include <vector>
#include <iostream>

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace make_entry_tx
            {
                // TEMP !!!
                const static std::vector<PublicKey> TRUSTEES{
                    "022054a424a0f76037d7fbe9dca924ba42f87574e2a2f4b6d9fd68231516fcbaeb",
                    "039642b5f4defadc2b65d4dadc6479c88174e6fccdcb4c4e636c111fcd949efa3b",
                    "02421b7dc96af3b9f73b219d7ee5d99d73086505b493294b1b7dc38eaf3667b734"
                };
                const static uint32_t MIN_SIGS = 2;
                const static uint32_t FUND_TX_VERSION = 2;
                const static uint32_t FUND_TX_LOCK_TIME = 0;
                
                static uint64_t totalValue(const std::vector<Destination>& destinations)
                {
                    uint64_t total = 0;
                    for (const Destination& destination: destinations) {
                        total += destination.value;
                    }
                    return total;
                }
                
                static std::vector<BitcoinInput> makeBitcoinInputs(const Sources& sources)
                {
                    std::vector<BitcoinInput> inputs;
                    for (const Outpoint& outpoint:sources.outpoints) {
                        inputs.emplace_back(BitcoinInput(outpoint, Script(), 0xffffffff));
                    }
                    return inputs;
                }
                
                static std::vector<BitcoinOutput> makeBitcoinOutputs(
                    const std::vector<Destination>& bitcoinDestinations,
                    const uint64_t fundValue, const Script& claimScript)
                {
                    const Script scriptPubKey = Script::p2wshScript(claimScript);
                    std::vector<BitcoinOutput> outputs{
                        // Add Conclave funding output
                        BitcoinOutput(fundValue, scriptPubKey)
                    };
                    // Add bitcoin change outputs
                    for (const Destination& destination: bitcoinDestinations) {
                        Script scriptPubKey = Script::p2hScript(destination.address);
                        outputs.emplace_back(BitcoinOutput(destination.value, scriptPubKey));
                    }
                    return outputs;
                }
                
                static std::vector<ConclaveOutput> makeConclaveOutputs(
                    const std::vector<Destination>& conclaveDestinations)
                {
                    std::vector<ConclaveOutput> outputs;
                    for (const Destination& destination: conclaveDestinations) {
                        Script scriptPubKey = Script::p2hScript(destination.address);
                        uint64_t value = destination.value;
                        outputs.emplace_back(ConclaveOutput(scriptPubKey, value));
                    }
                    return outputs;
                }
                
                MakeEntryTxResponse* makeEntryTxHandler(
                    const MakeEntryTxRequest& makeEntryTxRequest, ConclaveNode& conclaveNode)
                {
                    const Sources sources = makeEntryTxRequest.getSources();
                    const Destinations destinations = makeEntryTxRequest.getDestinations();
                    const std::vector<Destination> bitcoinDestinations = destinations.bitcoinDestinations;
                    const std::vector<Destination> conclaveDestinations = destinations.conclaveDestinations;
                    const uint64_t fundValue = totalValue(conclaveDestinations);
                    
                    // Make Conclave Tx
                    ConclaveTx claimTx(MIN_SIGS, TRUSTEES, makeConclaveOutputs(conclaveDestinations));
                    
                    // Make Bitcoin Tx
                    BitcoinTx fundTx(FUND_TX_VERSION,
                                     makeBitcoinInputs(sources),
                                     makeBitcoinOutputs(bitcoinDestinations, fundValue, claimTx.getClaimScript()),
                                     FUND_TX_LOCK_TIME
                    );
                    return new MakeEntryTxResponse(EntryTx(fundTx, claimTx));
                }
            }
        }
    }
}
