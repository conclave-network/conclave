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

#pragma once

#include "structs/sources.h"
#include "structs/destinations.h"
#include "../../../structs/outpoint.h"
#include "make_entry_tx_response.h"
#include "../request.h"
#include <iostream>
#include <vector>

namespace pt = boost::property_tree;
namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        namespace methods
        {
            namespace make_entry_tx
            {
                class MakeEntryTxRequest;
                
                MakeEntryTxResponse* makeEntryTxHandler(const MakeEntryTxRequest&, ConclaveNode&);
                
                class MakeEntryTxRequest : public Request
                {
                    public:
                    MakeEntryTxRequest(const pt::ptree& params)
                        : sources(params.get_child("sources")),
                          destinations(params.get_child("destinations"))
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
                        return makeEntryTxHandler(*this, conclaveNode);
                    }
                    
                    const Sources& getSources() const
                    {
                        return sources;
                    }
                    
                    const Destinations& getDestinations() const
                    {
                        return destinations;
                    }
                    
                    private:
                    const static RpcMethod rpcMethod = RpcMethod::MakeEntryTx;
                    const Sources sources;
                    const Destinations destinations;
                };
            }
        }
    }
}
