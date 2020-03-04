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

#include "../config/cloud_config.h"
#include "../hash256.h"
#include "../conclave.h"
#include <lmdb++.h>
#include <vector>
#include <optional>
#include <string>

namespace conclave
{
    namespace cloud
    {
        class Cloud
        {
            public:
            Cloud(const std::string&);
            Cloud(const CloudConfig&);
            bool putItem(const std::vector<BYTE>&);
            std::optional<std::vector<BYTE>> getItem(const Hash256&);
            private:
            lmdb::env env;
        };
    }
}
