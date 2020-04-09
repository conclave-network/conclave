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

#include "get_utxos_request.h"
#include "get_utxos_response.h"
#include "../../../conclave_node.h"

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace get_utxos
            {
                GetUtxosResponse*
                getUtxosHandler(const GetUtxosRequest& getUtxosRequest,
                                ConclaveNode& conclaveNode)
                {
                    const Address& address = getUtxosRequest.getAddress();
                    if (address.isConclave()) {
                        return new GetUtxosResponse(conclaveNode.getConclaveChain().getUtxos(address));
                    } else {
                        return new GetUtxosResponse(conclaveNode.getBitcoinChain().getUtxos(address));
                    }
                }
            }
        }
    }
}
