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

#include "rpc_acceptor.h"
#include "methods/request.h"
#include "../mongoose/mongoose.h"
#include "../mongoose/mongoose_helpers.h"
#include <iostream>

namespace conclave
{
    namespace rpc
    {
        RpcAcceptor::RpcAcceptor(const unsigned int id, const RpcAcceptorConfig& rpcAcceptorConfig,
                                 ConcurrentList<Request*>& requestQueue)
            : Worker(), id(id), rpcAcceptorConfig(rpcAcceptorConfig),
              requestQueue(requestQueue)
        {
        }
        
        void RpcAcceptor::prepare()
        {
            mg_mgr_init(&mgMgr, nullptr);
            auto handler = [](struct mg_connection* conn, int ev, void* p) {
                if (ev == MG_EV_HTTP_REQUEST) {
                    struct http_message* hm = (struct http_message*) p;
                    Request* pRequest;
                    try {
                        pRequest = Request::deserializeJson(mgStrToString(hm->body));
                        pRequest->tag = (void*) conn;
                    } catch (std::exception& e) {
                        std::cerr << "RpcAcceptor handler caught:" << e.what() << std::endl;
                        mg_http_send_error(conn, 500, e.what());
                        return;
                    }
                    // Queue the request for processing
                    ((ConcurrentList<Request*>*) (conn->user_data))->addToEnd(pRequest);
                }
            };
            std::string mgAddressString = mgMakeAddressString(rpcAcceptorConfig.getIPAddress(),
                                                              rpcAcceptorConfig.getPort());
            if ((mgConn = mg_bind(&mgMgr, mgAddressString.c_str(), handler)) == nullptr) {
                throw std::runtime_error("RpcAcceptor: Failed to bind to " + mgAddressString);
            }
            std::cout << "RpcAcceptor: listening on " << mgAddressString << std::endl;
            
            // Pass a reference to the request queue so handler can en-queue new requests
            mgConn->user_data = &requestQueue;
            mg_set_protocol_http_websocket(mgConn);
        }
        
        void RpcAcceptor::work()
        {
            mg_mgr_poll(&mgMgr, MG_MGR_POLL_INTERVAL_MS);
        }
        
        void RpcAcceptor::cleanup()
        {
            std::cout << "RpcAcceptor: Freeing mongoose manager" << std::endl;
            mg_mgr_free(&mgMgr);
        }
    }
}
