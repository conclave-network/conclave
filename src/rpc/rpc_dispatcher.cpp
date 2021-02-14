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

#include "rpc.h"
#include "rpc_dispatcher.h"
#include "../mongoose/mongoose.h"
#include <iostream>

namespace conclave
{
    namespace rpc
    {
        RpcDispatcher::RpcDispatcher(const unsigned int id, ConcurrentList<Response*>& responseQueue)
            : Worker(),
              id(id), responseQueue(responseQueue)
        {
        }
        
        void RpcDispatcher::work()
        {
            std::cout << "RPC dispatcher " << id << " waiting for next response..." << std::endl;
            std::optional<Response*> opResponse = responseQueue.popFromStart();
            if (!opResponse.has_value()) {
                std::cout << "RPC dispatcher " << id << " popped an empty optional" << std::endl;
                return;
            }
            Response& response = **opResponse;
            std::cout << "RPC dispatcher " << id << " dequeued a " <<
                      response.getMethodName() << " response" << std::endl;
            struct mg_connection* conn = (struct mg_connection*) response.tag;
            const std::string& json = response.getSerializedJson();
            const size_t len = json.length();
            mg_send_head(conn, 200, len, RESPONSE_HEADERS);
            mg_send(conn, json.c_str(), len);
            mg_mgr_poll(conn->mgr, 0);
            delete &response;
        }
    }
}
