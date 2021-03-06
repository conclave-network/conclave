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
        testnet = root.get<bool>("Testnet");
        displayName = root.get<std::string>("DisplayName");
        privateKey = root.get<std::string>("PrivateKey");
        rpcConfig = RpcConfig(root.get_child("RPC"));
        bitcoinChainConfig = BitcoinChainConfig(root.get_child("BitcoinChain"));
        conclaveChainConfig = ConclaveChainConfig(root.get_child("ConclaveChain"));
        chainwatchConfig = ChainwatchConfig(root.get_child("Chainwatch"));
    } catch (std::exception& e) {
        throw std::runtime_error("Error parsing config file " + configFilePath + ": " + e.what());
    }
}

bool Config::getTestnet() const
{
    return testnet;
}

const std::string& Config::getDisplayName() const
{
    return displayName;
}

const std::string& Config::getPrivateKey() const
{
    return privateKey;
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

const ChainwatchConfig& Config::getChainwatchConfig() const
{
    return *chainwatchConfig;
}

