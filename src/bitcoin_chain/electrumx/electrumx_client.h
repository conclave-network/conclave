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

#include "../../conclave.h"
#include "../../config/electrumx_client_config.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <mutex>

using Poco::Net::StreamSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        namespace bitcoin
        {
            namespace electrumx
            {
                const static size_t RECEIVE_BUFFER_SIZE = 4096;
                
                class ElectrumxClient
                {
                    public:
                    ElectrumxClient(const std::string&, const unsigned short);
                    ElectrumxClient(const ElectrumxClientConfig&);
                    ~ElectrumxClient();
                    const pt::ptree blockchainScripthashGetBalance(const std::string&);
                    const pt::ptree blockchainTransactionBroadcast(const std::string&);
                    private:
                    const pt::ptree doRequest(const pt::ptree&);
                    BYTE* receiveBuffer;
                    const SocketAddress socketAddress;
                    StreamSocket streamSocket;
                    std::mutex receiveBufferMutex;
                };
            }
        }
    }
}
