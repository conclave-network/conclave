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

#include <stdexcept>
#include <string>

#pragma once
namespace conclave
{
    namespace rpc
    {
        const static char* RESPONSE_HEADERS = "Content-Type: application/json\nConnection: close";
        
        inline void ensure_correct_user_input(const bool predicate, const std::string& errMsg)
        {
            if (!predicate) {
                throw std::runtime_error("Bad input: " + errMsg);
            }
        }
    }
}
