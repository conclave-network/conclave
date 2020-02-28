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

#include "../worker.h"
#include "../config/rpc_acceptor_config.h"
#include "../mongoose/mongoose.h"
#include "../util/concurrent_list.h"
#include "methods/request.h"

namespace conclave
{
    namespace rpc
    {
        const static int MG_MGR_POLL_INTERVAL_MS = 1000;
        
        class RpcAcceptor final : public Worker
        {
            public:
            RpcAcceptor(const unsigned int, const RpcAcceptorConfig&, ConcurrentList<Request*>&);
            private:
            void prepare() override final;
            void work() override final;
            void cleanup() override final;
            const unsigned int id;
            const RpcAcceptorConfig& rpcAcceptorConfig;
            ConcurrentList<Request*>& requestQueue;
            // Mongoose structs
            struct mg_mgr mgMgr;
            struct mg_connection* mgConn;
        };
    }
}
