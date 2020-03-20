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

#include "database_client.h"
#include "../../util/filesystem.h"

namespace conclave
{
    namespace chain
    {
        namespace database
        {
            static inline const Hash256 makeFQKey(const std::string& collectionName, const Hash256& key)
            {
                return Hash256::digest(collectionName) ^ key;
            }
            
            static lmdb::env initLmdb(const std::string& rootDirectory)
            {
                fs::create_directory(rootDirectory);
                lmdb::env env = lmdb::env::create();
                env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); // 1GB - TODO: parameterize
                env.open(rootDirectory.c_str(), 0, 0664);
                return env;
            }
            
            DatabaseClient::DatabaseClient(const std::string& rootDirectory)
                : env(initLmdb(rootDirectory))
            {
            }
            
            DatabaseClient::DatabaseClient(const DatabaseClientConfig& databaseClientConfig)
                : DatabaseClient(databaseClientConfig.getRootDirectory())
            {
            }
            
            Hash256 DatabaseClient::putItem(const std::vector<BYTE>& value)
            {
                Hash256 key = Hash256::digest(value);
                lmdb::txn wtxn = lmdb::txn::begin(env);
                lmdb::dbi dbi = lmdb::dbi::open(wtxn, nullptr);
                if (!dbi.put(wtxn, key, value)) {
                    throw std::runtime_error("putItem failed");
                }
                wtxn.commit();
                return key;
            }
            
            std::optional<std::vector<BYTE>> DatabaseClient::getItem(const Hash256& key)
            {
                lmdb::txn rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
                lmdb::dbi dbi = lmdb::dbi::open(rtxn, nullptr);
                std::vector<BYTE> value;
                if (!dbi.get(rtxn, key, value)) {
                    return std::nullopt;
                }
                // Compute hash of value and ensure it matches the key
                if (Hash256::digest(value) != key) {
                    throw std::runtime_error("getItem failed: data hash does not match key");
                }
                return value;
            }
            
            void DatabaseClient::putMutableItem(const std::string& collectionName,
                                                const Hash256& key, const std::vector<BYTE>& value)
            {
                lmdb::txn wtxn = lmdb::txn::begin(env);
                lmdb::dbi dbi = lmdb::dbi::open(wtxn, nullptr);
                Hash256 fqKey = makeFQKey(collectionName, key);
                if (!dbi.put(wtxn, fqKey, value)) {
                    throw std::runtime_error("putMutableItem failed");
                }
                wtxn.commit();
            }
            
            std::optional<std::vector<BYTE>>
            DatabaseClient::getMutableItem(const std::string& collectionName, const Hash256& key)
            {
                lmdb::txn rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
                lmdb::dbi dbi = lmdb::dbi::open(rtxn, nullptr);
                Hash256 fqKey = makeFQKey(collectionName, key);
                std::vector<BYTE> value;
                if (!dbi.get(rtxn, fqKey, value)) {
                    return std::nullopt;
                }
                return value;
            }
        }
    }
}
