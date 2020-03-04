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


#include "cloud.h"

namespace conclave
{
    namespace cloud
    {
        static lmdb::env initLmdb(const std::string& rootDirectory)
        {
            lmdb::env env = lmdb::env::create();
            env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); // 1GB - TODO: parameterise
            env.open(rootDirectory.c_str(), 0, 0664);
            return env;
        }
        
        Cloud::Cloud(const std::string& rootDirectory)
            : env(initLmdb(rootDirectory))
        {
        }
        
        Cloud::Cloud(const CloudConfig& cloudConfig)
            : Cloud(cloudConfig.getRootDirectory())
        {
        }
        
        bool Cloud::putItem(const std::vector<BYTE>& value)
        {
            return false;
        }
        
        std::optional<std::vector<BYTE>> Cloud::getItem(const Hash256&)
        {
            return std::nullopt;
        }
    }
}