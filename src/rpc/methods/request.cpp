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

#include "request.h"
#include "node_info/node_info_request.h"
#include "get_address_balance/get_address_balance_request.h"
#include "make_entry_tx/make_entry_tx_request.h"
#include "submit_entry_tx/submit_entry_tx_request.h"

namespace conclave
{
    namespace rpc
    {
        using namespace methods::node_info;
        using namespace methods::get_address_balance;
        using namespace methods::make_entry_tx;
        using namespace methods::submit_entry_tx;
        
        Request* Request::deserializeJson(const std::string& json)
        {
            return deserializeJson(parseJson(json));
        }
        
        Request* Request::deserializeJson(const pt::ptree& root)
        {
            // Go for JSON-RPC 2.0 specification (https://www.jsonrpc.org/specification)
            std::string method = getPrimitiveFromJson<std::string>(root, "method");
            const pt::ptree params = root.get_child("params", pt::ptree());
            // TODO - find out a way to lose this switch statement
            switch (stringToRpcMethod(method)) {
                case RpcMethod::NodeInfo:
                    return new NodeInfoRequest(params);
                case RpcMethod::GetAddressBalance:
                    return new GetAddressBalanceRequest(params);
                case RpcMethod::MakeEntryTx:
                    return new MakeEntryTxRequest(params);
                case RpcMethod::SubmitEntryTx:
                    return new SubmitEntryTxRequest(params);
                default:
                    throw std::logic_error("No implementation found for RPC method: " + method);
            }
        }
    }
}
