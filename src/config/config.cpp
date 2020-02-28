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

#include "config.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

namespace pt = boost::property_tree;

Config::Config(const std::string& configFilePath)
{
    pt::ptree root;
    try {
        pt::read_json(configFilePath, root);
    } catch (std::exception& e) {
        throw std::runtime_error("Error opening config file: " + configFilePath);
    }
    try {
        displayName = root.get<std::string>("DisplayName");
        privateKey = root.get<std::string>("PrivateKey");
        chainwatchConfig = ChainwatchConfig(root.get_child("Chainwatch"));
        rpcConfig = RpcConfig(root.get_child("RPC"));
        bitcoinChainConfig = BitcoinChainConfig(root.get_child("BitcoinChain"));
        conclaveChainConfig = ConclaveChainConfig(root.get_child("ConclaveChain"));
    } catch (std::exception& e) {
        throw std::runtime_error("Error parsing config file " + configFilePath + ": " + e.what());
    }
}

const std::string& Config::getDisplayName() const
{
    return displayName;
}

const std::string& Config::getPrivateKey() const
{
    return privateKey;
}

const ChainwatchConfig& Config::getChainwatchConfig() const
{
    return *chainwatchConfig;
}

const RpcConfig& Config::getRpcConfig() const
{
    return *rpcConfig;
}

const BitcoinChainConfig& Config::getBitcoinChainConfig() const
{
    return *bitcoinChainConfig;
}

const ConclaveChainConfig& Config::getConclaveChainConfig() const
{
    return *conclaveChainConfig;
}


