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

#include "database_client_config.h"

DatabaseClientConfig::DatabaseClientConfig(const pt::ptree& tree)
    : rootDirectory(tree.get<std::string>("RootDirectory"))
{
}

DatabaseClientConfig::DatabaseClientConfig(const std::string& rootDirectory)
    : rootDirectory(rootDirectory)
{
}

const std::string& DatabaseClientConfig::getRootDirectory() const
{
    return rootDirectory;
}
