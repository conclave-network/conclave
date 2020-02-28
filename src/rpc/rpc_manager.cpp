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

#include "rpc_manager.h"
#include <iostream>

namespace conclave
{
    namespace rpc
    {
        RpcManager::RpcManager(const RpcConfig& rpcConfig, ConclaveNode& conclaveNode)
            : Worker(), rpcConfig(rpcConfig),
              conclaveNode(conclaveNode),
              rpcAcceptor(RpcAcceptor(0, rpcConfig.getRpcAcceptorConfig(), requestQueue)),
              rpcDispatcher(0, responseQueue)
        {
        }
        
        void RpcManager::prepare()
        {
            std::cout << "Starting RPC manager..." << std::endl;
            // Order matters!
            rpcDispatcher.start();
            startProcessors();
            rpcAcceptor.start();
        }
        
        void RpcManager::cleanup()
        {
            std::cout << "Stopping RPC manager..." << std::endl;
            // Order matters!
            rpcAcceptor.stop();
            stopProcessors();
            requestQueue.shutdown();
            rpcDispatcher.stop(false);
            responseQueue.shutdown();
        }
        
        void RpcManager::startProcessors()
        {
            // Create processors
            for (unsigned int i = 0; i < rpcConfig.getNumProcessors(); i++) {
                processors.emplace_back(std::move(RpcProcessor(i, conclaveNode, requestQueue, responseQueue)));
                std::cout << "Created RPC processor " << i << std::endl;
            }
            std::cout << "Created " << rpcConfig.getNumProcessors() <<
                      " RPC processors. Starting them now..." << std::endl;
            
            // Start up processors
            for (unsigned int i = 0; i < rpcConfig.getNumProcessors(); i++) {
                processors[i].start();
                std::cout << "Started RPC processor " << i << std::endl;
            }
            std::cout << "Started " << rpcConfig.getNumProcessors() <<
                      " RPC processors" << std::endl;
        }
        
        void RpcManager::stopProcessors()
        {
            for (unsigned int i = 0; i < rpcConfig.getNumProcessors(); i++) {
                processors[i].stop(false);
                std::cout << "Stopped RPC processor " << i << std::endl;
            }
            std::cout << "Stopped " << rpcConfig.getNumProcessors() <<
                      " RPC processors" << std::endl;
        }
    }
}
