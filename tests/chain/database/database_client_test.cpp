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

#define BOOST_TEST_MODULE Database_Client_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../src/chain/database/database_client.h"
#include "../../../src/util/filesystem.h"
#include "../../../src/conclave.h"
#include <vector>
#include <string>

namespace conclave
{
    namespace chain
    {
        namespace database
        {
            const static std::string DB_ROOT = "/tmp/conclaveDB.mdb";
            const static std::vector<BYTE> ITEM_1{'B', 'i', 't', 'c', 'o', 'i', 'n'};
            const static std::vector<BYTE> ITEM_2{'C', 'o', 'n', 'c', 'l', 'a', 'v', 'e'};
            const static Hash256 ITEM_1_KEY("6fef50c603dcf8f3723119e7d4f2d62160dd1814b145521524eaee7c82b6b31a");
            const static Hash256 ITEM_2_KEY("b976c0370263098cb4e01625a9b103b36a8d915d619e8635ca716ab049e762dd");
            const static Hash256 RANDOM_HASH_1;
            const static Hash256 RANDOM_HASH_2;
            
            BOOST_AUTO_TEST_CASE(DatabaseClientConstructorsTest)
            {
                fs::remove_all(DB_ROOT);
                DatabaseClient databaseClient(DB_ROOT);
                BOOST_TEST(fs::exists(DB_ROOT));
                BOOST_TEST(fs::is_directory(DB_ROOT));
                BOOST_TEST(fs::is_regular_file(fs::path(DB_ROOT).concat("/data.mdb")));
                BOOST_TEST(fs::is_regular_file(fs::path(DB_ROOT).concat("/lock.mdb")));
            }
            
            BOOST_AUTO_TEST_CASE(DatabaseClientPutItemTest)
            {
                fs::remove_all(DB_ROOT);
                DatabaseClient databaseClient(DB_ROOT);
                Hash256 immutableItem1Key = databaseClient.putItem(ITEM_1);
                Hash256 immutableItem2Key = databaseClient.putItem(ITEM_2);
                BOOST_TEST(immutableItem1Key == ITEM_1_KEY);
                BOOST_TEST(immutableItem2Key == ITEM_2_KEY);
            }
            
            BOOST_AUTO_TEST_CASE(DatabaseClientGetItemTest)
            {
                fs::remove_all(DB_ROOT);
                DatabaseClient databaseClient(DB_ROOT);
                databaseClient.putItem(ITEM_1);
                std::optional<std::vector<BYTE>> immutableItem1 = databaseClient.getItem(ITEM_1_KEY);
                std::optional<std::vector<BYTE>> immutableItem2 = databaseClient.getItem(ITEM_2_KEY);
                BOOST_TEST((immutableItem1 == ITEM_1));
                BOOST_TEST((immutableItem2 == std::nullopt));
            }
            
            BOOST_AUTO_TEST_CASE(DatabaseClientPutMutableItemTest)
            {
                // Test that putMutableItem replaces the value when called with the same key
                fs::remove_all(DB_ROOT);
                DatabaseClient databaseClient(DB_ROOT);
                databaseClient.putMutableItem(RANDOM_HASH_2, ITEM_1);
                databaseClient.putMutableItem(RANDOM_HASH_2, ITEM_2);
                std::optional<std::vector<BYTE>> mutableItem1 = databaseClient.getMutableItem(RANDOM_HASH_1);
                std::optional<std::vector<BYTE>> mutableItem2 = databaseClient.getMutableItem(RANDOM_HASH_2);
                BOOST_TEST((mutableItem1 == std::nullopt));
                BOOST_TEST((mutableItem2 == ITEM_2));
            }
            
            BOOST_AUTO_TEST_CASE(DatabaseClientGetMutableItemTest)
            {
                fs::remove_all(DB_ROOT);
                DatabaseClient databaseClient(DB_ROOT);
                databaseClient.putMutableItem(RANDOM_HASH_1, ITEM_1);
                databaseClient.putMutableItem(RANDOM_HASH_2, ITEM_2);
                std::optional<std::vector<BYTE>> mutableItem1 = databaseClient.getMutableItem(RANDOM_HASH_1);
                std::optional<std::vector<BYTE>> mutableItem2 = databaseClient.getMutableItem(RANDOM_HASH_2);
                BOOST_TEST((mutableItem1 == ITEM_1));
                BOOST_TEST((mutableItem2 == ITEM_2));
            }
        }
    }
}
