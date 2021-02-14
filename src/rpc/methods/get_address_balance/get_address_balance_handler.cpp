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

#include "get_address_balance_request.h"
#include "get_address_balance_response.h"
#include "../../../conclave_node.h"

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace get_address_balance
            {
                GetAddressBalanceResponse*
                getAddressBalanceHandler(const GetAddressBalanceRequest& getAddressBalanceRequest,
                                         ConclaveNode& conclaveNode)
                {
                    const Address& address = getAddressBalanceRequest.getAddress();
                    uint64_t balance;
                    if (address.isConclave()) {
                        balance = conclaveNode.getConclaveChain().getAddressBalance(address);
                    } else {
                        balance = conclaveNode.getBitcoinChain().getAddressBalance(address);
                    }
                    return new GetAddressBalanceResponse(balance);
                }
            }
        }
    }
}
