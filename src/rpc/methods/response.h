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

#include "methods.h"
#include <iostream>

namespace conclave
{
    namespace rpc
    {
        class Response
        {
            public:
            virtual ~Response() = default;
            virtual RpcMethod getMethod() const = 0;
            virtual const std::string& getMethodName() const = 0;
            const std::string& getSerializedJson();
            // Tag some arbitrary data to the request. Typically this will point to
            // something the networking library understands which contains the
            // return address, and will be copied from the `tag` on the original
            // request.
            void* tag;
            protected:
            std::string serializedJson;
            private:
            virtual void serialize() = 0;
            bool serialized = false;
        };
    }
}
