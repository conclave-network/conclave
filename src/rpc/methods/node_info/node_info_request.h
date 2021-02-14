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

#include "node_info_response.h"
#include "../request.h"
#include <boost/property_tree/ptree.hpp>

namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        namespace methods
        {
            namespace node_info
            {
                class NodeInfoRequest;
                
                NodeInfoResponse* nodeInfoHandler(const NodeInfoRequest&, ConclaveNode&);
                
                class NodeInfoRequest : public Request
                {
                    public:
                    NodeInfoRequest(const pt::ptree& params)
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
                        return nodeInfoHandler(*this, conclaveNode);
                    }
                    
                    private:
                    const static RpcMethod rpcMethod = RpcMethod::NodeInfo;
                };
            }
        }
    }
}
