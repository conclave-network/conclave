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

#include "../../config/database_client_config.h"
#include "../../hash256.h"
#include "../../conclave.h"
#include <lmdb++.h>
#include <vector>
#include <optional>
#include <string>

namespace conclave
{
    namespace chain
    {
        namespace database
        {
            class DatabaseClient
            {
                public:
                DatabaseClient(const std::string&);
                DatabaseClient(const DatabaseClientConfig&);
                ~DatabaseClient();
                Hash256 putItem(const std::vector<BYTE>&);
                std::optional<std::vector<BYTE>> getItem(const Hash256&);
                void putMutableItem(const std::string&, const Hash256&, const std::vector<BYTE>&);
                std::optional<std::vector<BYTE>> getMutableItem(const std::string&, const Hash256&);
                void putSingletonItem(const std::string&, const std::vector<BYTE>&);
                std::optional<std::vector<BYTE>> getSingletonItem(const std::string&);
                private:
                const static Hash256 SINGLETON_KEY;
                lmdb::env env;
            };
        }
    }
}
