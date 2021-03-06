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
#pragma once

#include "methods/request.h"
#include "methods/response.h"
#include "methods/methods.h"
#include "../worker.h"
#include "../util/concurrent_list.h"

namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        class RpcProcessor final : public Worker
        {
            public:
            RpcProcessor(const unsigned int, ConclaveNode&, ConcurrentList<Request*>&, ConcurrentList<Response*>&);
            private:
            void work() override final;
            const unsigned int id;
            ConclaveNode& conclaveNode;
            ConcurrentList<Request*>& requestQueue;
            ConcurrentList<Response*>& responseQueue;
        };
    }
}
