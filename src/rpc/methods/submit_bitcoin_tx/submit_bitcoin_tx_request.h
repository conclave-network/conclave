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

#include "submit_bitcoin_tx_response.h"
#include "../request.h"
#include "../../../structs/bitcoin_tx.h"
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;
namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        namespace methods
        {
            namespace submit_bitcoin_tx
            {
                class SubmitBitcoinTxRequest;
                
                SubmitBitcoinTxResponse* submitBitcoinTxHandler(const SubmitBitcoinTxRequest&, ConclaveNode&);
                
                class SubmitBitcoinTxRequest : public Request
                {
                    public:
                    SubmitBitcoinTxRequest(const pt::ptree& params)
                        : bitcoinTx(params.get_child("BitcoinTx"))
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
                        return submitBitcoinTxHandler(*this, conclaveNode);
                    }
                    
                    const BitcoinTx& getBitcoinTx() const
                    {
                        return bitcoinTx;
                    }
                    
                    private:
                    const static RpcMethod rpcMethod = RpcMethod::SubmitBitcoinTx;
                    const BitcoinTx bitcoinTx;
                };
            }
        }
    }
}
