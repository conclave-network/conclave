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

#include "conclave_node.h"
#include <iostream>

using namespace conclave::rpc;
using namespace conclave::chain;
//using namespace conclave::chain::electrumx;
namespace conclave
{
    ConclaveNode::ConclaveNode(const Config& config)
        : Worker(),
          testnet(config.getTestnet()),
          displayName(config.getDisplayName()),
          privateKey(config.getPrivateKey()),
          bitcoinChain(BitcoinChain(config.getBitcoinChainConfig())),
          conclaveChain(ConclaveChain(config.getConclaveChainConfig(), this->bitcoinChain)),
          chainwatchManager(config.getChainwatchConfig(), this->bitcoinChain),
          rpcManager(RpcManager(config.getRpcConfig(), *this))
    {
    }
    
    void ConclaveNode::prepare()
    {
        std::cout << "Starting node: " << getDisplayName() << std::endl;
        std::cout << "Public key: " << getPublicKey() << std::endl;
        std::cout << "Testnet: " << isTestnet() << std::endl;
        chainwatchManager.start();
        rpcManager.start();
    }
    
    void ConclaveNode::cleanup()
    {
        rpcManager.stop();
        chainwatchManager.stop();
    }
    
    bool ConclaveNode::isTestnet() const
    {
        return testnet;
    }
    
    const std::string ConclaveNode::getDisplayName() const
    {
        return displayName;
    }
    
    const PublicKey ConclaveNode::getPublicKey() const
    {
        return privateKey.getPublicKey();
    }
    
    BitcoinChain& ConclaveNode::getBitcoinChain()
    {
        return bitcoinChain;
    }
    
    ConclaveChain& ConclaveNode::getConclaveChain()
    {
        return conclaveChain;
    }
}