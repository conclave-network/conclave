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

#define BOOST_TEST_MODULE Conclave_Block_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../src/chain/structs/conclave_block.h"
#include <boost/property_tree/json_parser.hpp>
#include <cstdint>
#include <vector>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        const static uint64_t POT_1 = 43543543545235;
        const static uint64_t POT_2 = 90234782359871;
        const static uint64_t HEIGHT_1 = 92305981724234;
        const static uint64_t HEIGHT_2 = 21489320927812;
        const static uint32_t EPOCH_1 = 453939008;
        const static uint32_t EPOCH_2 = 129082432;
        const static Hash256 HASH_PREV_BLOCK_1("5885d77d868796a32167d0f62b91a384480681fab54f42ad94db5d9da53563f6");
        const static Hash256 HASH_PREV_BLOCK_2("287a13f631b93dc344715e2e20498f11fa27a8f47ed7195cb19480c0c9b391db");
        const static Hash256 LOWEST_PARENT_BITCOIN_BLOCK_HASH_1
            ("685da7ae1ddb7cae0985638b23f1e557336880177632baad474304e11caff60b");
        const static Hash256 LOWEST_PARENT_BITCOIN_BLOCK_HASH_2
            ("05eb56b629d720308cc608767d4978d4358c821aac19a510871979eadfe1abc9");
        const static uint16_t TX_TYPE_ID_1 = 12418;
        const static uint16_t TX_TYPE_ID_2 = 46438;
        const static uint16_t TX_VERSION_1 = 10924;
        const static uint16_t TX_VERSION_2 = 61093;
        const static Hash256 TX_HASH_1("7b1a23b60a934a3775b5f451693c6c8c8b676569f4ab59566ad4b35d4a2afc6a");
        const static Hash256 TX_HASH_2("97c19b69e4aef026530beca4cbee4e2ef5225a57ad8f66b5c04922098fdffd78");
        const static std::string CONCLAVE_BLOCK_1_STR =
            "{\n"
            "    \"pot\": \"43543543545235\",\n"
            "    \"height\": \"92305981724234\",\n"
            "    \"epoch\": \"453939008\",\n"
            "    \"hashPrevBlock\": \"5885d77d868796a32167d0f62b91a384480681fab54f42ad94db5d9da53563f6\",\n"
            "    \"lowestParentBitcoinBlockHash\": \"685da7ae1ddb7cae0985638b23f1e557336880177632baad474304e11caff60b\",\n"
            "    \"txTypeId\": \"12418\",\n"
            "    \"txVersion\": \"10924\",\n"
            "    \"txHash\": \"7b1a23b60a934a3775b5f451693c6c8c8b676569f4ab59566ad4b35d4a2afc6a\"\n"
            "}\n";
        BOOST_AUTO_TEST_SUITE(ConclaveBlockTestSuite)
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockDeserializeFactoryTest)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockConstructorsTest)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockCopyAndMoveConstructorTest)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockHash256Test)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockSerializeTest)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockCastToPtreeTest)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockCastToStringTest)
            {
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockAssignmentOperatorsTest)
            {
            }
            
            BOOST_AUTO_TEST_SUITE()
            }
        }
