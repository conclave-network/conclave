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
#include "../../../structs/conclave_claim_tx.h"
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
                    "03519e185b4c0de842cb5cf9e49cd6df00569c4f34bf87ed295b65f9473d508e9d",
                    "02a013b162136d674a37b95ce50d0ae02c5f3d3ab74e52d363e809cde86e92cbbb",
                    "031f74bdb5873d9ae3d46aa3473718eac8fe19d4b9d62d9421cf6b9b6beab1e6c4"
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
                
                Script makeEntryRedeemScript(const Hash256& claimTxHash,
                                             const std::vector<PublicKey>& trustees,
                                             const uint32_t minSigs)
                {
                    uint32_t nTrustees = trustees.size();
                    std::vector<ScriptElement> scriptElements{
                        claimTxHash, ScriptOp::drop, nTrustees
                    };
                    scriptElements.reserve(3 + nTrustees + 2);
                    for (const PublicKey& trustee: trustees) {
                        scriptElements.emplace_back(trustee);
                    }
                    scriptElements.emplace_back(minSigs);
                    scriptElements.emplace_back(ScriptOp::checkmultisig);
                    return Script(scriptElements);
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
                    const std::vector<Destination>& bitcoinDestinations, const std::vector<PublicKey>& trustees,
                    const uint32_t minSigs, const Hash256& claimTxHash, const uint64_t fundValue)
                {
                    const Script redeemScript = makeEntryRedeemScript(claimTxHash, trustees, minSigs);
                    const Script scriptPubKey = Script::p2wshScript(redeemScript);
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
                    // Make claim Tx (Conclave)
                    //ConclaveTx claimTx(makeConclaveOutputs(conclaveDestinations), TRUSTEES, MIN_SIGS);
                    ConclaveTx claimTx(0, 0, MIN_SIGS,)
                    // Make fund Tx (Bitcoin)
                    std::vector<BitcoinInput> bitcoinInputs = makeBitcoinInputs(sources);
                    std::vector<BitcoinOutput> bitcoinOutputs =
                        makeBitcoinOutputs(bitcoinDestinations, TRUSTEES, MIN_SIGS, claimTx.getHash256(), fundValue);
                    BitcoinTx fundTx(FUND_TX_VERSION, bitcoinInputs, bitcoinOutputs, FUND_TX_LOCK_TIME);
                    return new MakeEntryTxResponse(EntryTx(fundTx, claimTx));
                }
            }
        }
    }
}
