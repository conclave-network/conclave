/*
 * CONCLAVE - Scaling Bitcoin Simply.
 * Copyright (C) 2019-2021 Conclave development team
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

#include "submit_bitcoin_tx_request.h"
#include "submit_bitcoin_tx_response.h"
#include "../../../conclave_node.h"
#include "../../../hash256.h"

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace submit_bitcoin_tx
            {
                SubmitBitcoinTxResponse* submitBitcoinTxHandler(
                    const SubmitBitcoinTxRequest& submitBitcoinTxRequest,
                    ConclaveNode& conclaveNode)
                {
                    const BitcoinTx bitcoinTx = submitBitcoinTxRequest.getBitcoinTx();
                    const Hash256 txId = conclaveNode.getBitcoinChain().submitTx(bitcoinTx);
                    return new SubmitBitcoinTxResponse(txId);
                }
            }
        }
    }
}
