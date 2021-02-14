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
#include "response.h"
#include "../../util/json.h"
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    class ConclaveNode;
    namespace rpc
    {
        class Request
        {
            public:
            virtual ~Request() = default;
            static Request* deserializeJson(const std::string&);
            virtual RpcMethod getMethod() const = 0;
            virtual const std::string& getMethodName() const = 0;
            virtual Response* handle(ConclaveNode&) const = 0;
            // Tag some arbitrary data to the request. Typically this will point to
            // something the networking library understands which contains the
            // return address, and will be copied to the `tag` on the generated
            // response.
            void* tag;
            private:
            static Request* deserializeJson(const pt::ptree&);
        };
    }
}
