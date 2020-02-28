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

#include "methods/request.h"
#include "methods/response.h"
#include "rpc_acceptor.h"
#include "rpc_dispatcher.h"
#include "rpc_processor.h"
#include "../config/rpc_config.h"
#include "../worker.h"
#include "../util/concurrent_list.h"
#include <string>
#include <vector>

namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        class RpcManager final : public Worker
        {
            public:
            RpcManager(const RpcConfig&, ConclaveNode&);
            private:
            void prepare() override final;
            void cleanup() override final;
            void startProcessors();
            void stopProcessors();
            const RpcConfig& rpcConfig;
            // Reference to parent node
            ConclaveNode& conclaveNode;
            RpcAcceptor rpcAcceptor;
            RpcDispatcher rpcDispatcher;
            std::vector<RpcProcessor> processors;
            ConcurrentList<Request*> requestQueue;
            ConcurrentList<Response*> responseQueue;
        };
    }
}