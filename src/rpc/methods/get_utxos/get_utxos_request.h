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

#pragma once

#include "get_utxos_response.h"
#include "../request.h"
#include "../../../address.h"
#include "../../../util/json.h"
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;
namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        namespace methods
        {
            namespace get_utxos
            {
                class GetUtxosRequest;
                
                GetUtxosResponse* getUtxosHandler(const GetUtxosRequest&, ConclaveNode&);
                
                class GetUtxosRequest : public Request
                {
                    public:
                    GetUtxosRequest(const pt::ptree& params)
                        : address(getPrimitiveFromJson<std::string>(params, "address"))
                    {
                    }
                    
                    RpcMethod getMethod() const override
                    {
                        return rpcMethod;
                    }
                    
                    const std::string& getMethodName() const override
                    {
                        return rpcMethodToString(rpcMethod);
                    }
                    
                    Response* handle(ConclaveNode& conclaveNode) const override
                    {
                        return getUtxosHandler(*this, conclaveNode);
                    }
                    
                    const Address& getAddress() const
                    {
                        return address;
                    }
                    
                    private:
                    const static RpcMethod rpcMethod = RpcMethod::GetUtxos;
                    const Address address;
                };
            }
        }
    }
}
