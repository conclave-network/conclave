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

#include "rpc_acceptor_config.h"

namespace pt = boost::property_tree;

RpcAcceptorConfig::RpcAcceptorConfig(const pt::ptree& tree)
    : ipAddress(tree.get<std::string>("IPAddress")),
      port(tree.get<unsigned short>("Port"))
{
}

RpcAcceptorConfig::RpcAcceptorConfig(const std::string& ipAddress, const unsigned short port)
    : ipAddress(ipAddress), port(port)
{
}

const std::string& RpcAcceptorConfig::getIPAddress() const
{
    return ipAddress;
}

unsigned short RpcAcceptorConfig::getPort() const
{
    return port;
}
