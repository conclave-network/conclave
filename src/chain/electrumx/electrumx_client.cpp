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

#include "electrumx_client.h"
#include "../../util/json.h"
#include "../../util/json_rpc.h"
#include <string>

using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        namespace electrumx
        {
            ElectrumxClient::ElectrumxClient(const std::string& host, const unsigned short port)
                : receiveBuffer(new BYTE[RECEIVE_BUFFER_SIZE + 1]),
                  socketAddress(host, port),
                  streamSocket(socketAddress)
            {
            }
            
            ElectrumxClient::ElectrumxClient(const ElectrumxClientConfig& electrumxClientConfig)
                : ElectrumxClient(electrumxClientConfig.getHost(), electrumxClientConfig.getPort())
            {
            }
            
            ElectrumxClient::~ElectrumxClient()
            {
                delete[] receiveBuffer;
            }
            
            /***
             * blockchain.scripthash.get_balance
             * https://electrumx.readthedocs.io/en/latest/protocol-methods.html#blockchain-scripthash-get-balance
             */
            const pt::ptree ElectrumxClient::blockchainScripthashGetBalance(const std::string& scripthash)
            {
                pt::ptree params;
                params.add("scripthash", scripthash);
                pt::ptree request = buildRequest("blockchain.scripthash.get_balance");
                request.add_child("params", params);
                return doRequest(request);
            }
            
            /***
             * blockchain.transaction.broadcast
             * https://electrumx.readthedocs.io/en/latest/protocol-methods.html#blockchain-transaction-broadcast
             */
            const pt::ptree ElectrumxClient::blockchainTransactionBroadcast(const std::string& rawTx)
            {
                pt::ptree params;
                params.add("raw_tx", rawTx);
                pt::ptree request = buildRequest("blockchain.transaction.broadcast");
                request.add_child("params", params);
                return doRequest(request);
            }
            
            /***
             * blockchain.headers.subscribe
             * https://electrumx.readthedocs.io/en/latest/protocol-methods.html#blockchain-headers-subscribe
             */
            const pt::ptree ElectrumxClient::blockchainHeadersSubscribe()
            {
                pt::ptree params;
                pt::ptree request = buildRequest("blockchain.headers.subscribe");
                return doRequest(request);
            }
            
            const pt::ptree ElectrumxClient::doRequest(const pt::ptree& request)
            {
                std::string requestString = jsonToString(request, false);
                streamSocket.sendBytes(requestString.c_str(), requestString.length(), 0);
                receiveBufferMutex.lock();
                int br = streamSocket.receiveBytes(receiveBuffer, RECEIVE_BUFFER_SIZE, 0);
                if (br <= 0 || (unsigned) br >= RECEIVE_BUFFER_SIZE) {
                    receiveBufferMutex.unlock();
                    throw std::runtime_error("Error receiving response from ElectrumX client");
                }
                receiveBuffer[br] = '\0';
                const pt::ptree response = parseJson((char*) receiveBuffer);
                receiveBufferMutex.unlock();
                return getResultOrThrowError(response);
            }
        };
    };
};
