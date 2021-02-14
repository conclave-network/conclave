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

#include "methods/error_response.h"
#include "rpc_processor.h"
#include <iostream>
#include <stdexcept>

namespace conclave
{
    namespace rpc
    {
        using namespace methods;
        
        RpcProcessor::RpcProcessor(const unsigned int id, ConclaveNode& conclaveNode,
                                   ConcurrentList<Request*>& requestQueue, ConcurrentList<Response*>& responseQueue)
            : Worker(), id(id),
              conclaveNode(conclaveNode), requestQueue(requestQueue), responseQueue(responseQueue)
        {
        }
        
        void RpcProcessor::work()
        {
            std::cout << "RPC processor " << id << " waiting for next request..." << std::endl;
            std::optional<Request*> opRequest = requestQueue.popFromStart();
            if (!opRequest.has_value()) {
                std::cout << "RPC processor " << id << " popped an empty optional" << std::endl;
                return;
            }
            Request& request = **opRequest;
            std::cout << "RPC processor " << id << " dequeued a " << request.getMethodName() << " request" << std::endl;
            
            /**
             * Run the handler and hopefully get back a response. If the handler throws an exception, we catch the
             * exception here and make an `ErrorResponse` containing the error message which is sent back to the
             * client.
             */
            
            Response* response;
            try {
                response = request.handle(conclaveNode);
            } catch (std::exception& e) {
                std::cout << "RpcProcessor caught: " << e.what() << std::endl;
                std::cout << "Queueing an error response" << std::endl;
                response = new ErrorResponse(request.getMethod(), e.what());
            }
            response->tag = request.tag;
            responseQueue.addToStart(response);
            delete &request;
        }
    }
}
