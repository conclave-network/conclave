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

#define BOOST_TEST_MODULE Outpoint_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/outpoint.h"
#include "../../src/util/json.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    const static Hash256 TXID_1("bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17");
    const static Hash256 TXID_2("f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37");
    const static uint32_t INDEX_1 = 0xfffffffe;
    const static uint32_t INDEX_2 = 0x00000001;
    const static std::string OUTPOINT_1_STR =
        "{\n"
        "    \"txId\": \"bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17\",\n"
        "    \"index\": \"4294967294\"\n"
        "}\n";
    const static std::string OUTPOINT_2_STR =
        "{\n"
        "    \"txId\": \"bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17\",\n"
        "    \"index\": \"1\"\n"
        "}\n";
    const static std::string OUTPOINT_3_STR =
        "{\n"
        "    \"txId\": \"f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37\",\n"
        "    \"index\": \"4294967294\"\n"
        "}\n";
    const static std::string OUTPOINT_4_STR =
        "{\n"
        "    \"txId\": \"f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37\",\n"
        "    \"index\": \"1\"\n"
        "}\n";
    const static pt::ptree OUTPOINT_1_PTREE = stringToPtree(OUTPOINT_1_STR);
    const static pt::ptree OUTPOINT_2_PTREE = stringToPtree(OUTPOINT_2_STR);
    const static pt::ptree OUTPOINT_3_PTREE = stringToPtree(OUTPOINT_3_STR);
    const static pt::ptree OUTPOINT_4_PTREE = stringToPtree(OUTPOINT_4_STR);
    const static std::vector<BYTE> OUTPOINT_1_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17bcb44f41abb055e153b7f395fd31e88f98c1ed8e249256198fb1e00b75cebdfeffffff");
    const static std::vector<BYTE> OUTPOINT_2_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17bcb44f41abb055e153b7f395fd31e88f98c1ed8e249256198fb1e00b75cebd01000000");
    const static std::vector<BYTE> OUTPOINT_3_SERIALIZED =
        HEX_TO_BYTE_VECTOR("370cfa2dc1a8525c1973acbb69cf4f5eea7404dad2c72931731f645ab9c277f4feffffff");
    const static std::vector<BYTE> OUTPOINT_4_SERIALIZED =
        HEX_TO_BYTE_VECTOR("370cfa2dc1a8525c1973acbb69cf4f5eea7404dad2c72931731f645ab9c277f401000000");
    BOOST_AUTO_TEST_SUITE(OutpointTestSuite)
        
        BOOST_AUTO_TEST_CASE(OutpointConstructorsTest)
        {
            Outpoint outpointFromProps(TXID_1, INDEX_1);
            Outpoint outpointFromPtree(OUTPOINT_1_PTREE);
            Outpoint outpointFromByteVector(OUTPOINT_1_SERIALIZED);
            BOOST_TEST((outpointFromProps.txId == TXID_1));
            BOOST_TEST((outpointFromProps.index == INDEX_1));
            BOOST_TEST((outpointFromPtree.txId == TXID_1));
            BOOST_TEST((outpointFromPtree.index == INDEX_1));
            BOOST_TEST((outpointFromByteVector.txId == TXID_1));
            BOOST_TEST((outpointFromByteVector.index == INDEX_1));
        }
        
        BOOST_AUTO_TEST_CASE(OutpointCastToStringTest)
        {
            BOOST_TEST((OUTPOINT_1_STR == (std::string) Outpoint(TXID_1, INDEX_1)));
            BOOST_TEST((OUTPOINT_2_STR == (std::string) Outpoint(TXID_1, INDEX_2)));
            BOOST_TEST((OUTPOINT_3_STR == (std::string) Outpoint(TXID_2, INDEX_1)));
            BOOST_TEST((OUTPOINT_4_STR == (std::string) Outpoint(TXID_2, INDEX_2)));
        }
        
        BOOST_AUTO_TEST_CASE(OutpointCastToPtreeTest)
        {
            BOOST_TEST((OUTPOINT_1_PTREE == (pt::ptree) Outpoint(TXID_1, INDEX_1)));
            BOOST_TEST((OUTPOINT_2_PTREE == (pt::ptree) Outpoint(TXID_1, INDEX_2)));
            BOOST_TEST((OUTPOINT_3_PTREE == (pt::ptree) Outpoint(TXID_2, INDEX_1)));
            BOOST_TEST((OUTPOINT_4_PTREE == (pt::ptree) Outpoint(TXID_2, INDEX_2)));
        }
        
        BOOST_AUTO_TEST_CASE(OutpointOperatorsTest)
        {
            Outpoint outpoint1(TXID_1, INDEX_1);
            Outpoint outpoint2(TXID_1, INDEX_2);
            Outpoint outpoint3(TXID_2, INDEX_1);
            Outpoint outpoint4(TXID_2, INDEX_2);
            Outpoint outpoint5(TXID_1, INDEX_1);
            Outpoint outpoint6(TXID_1, INDEX_2);
            Outpoint outpoint7(TXID_2, INDEX_1);
            Outpoint outpoint8(TXID_2, INDEX_2);
            BOOST_TEST((outpoint1 == outpoint5));
            BOOST_TEST((outpoint1 != outpoint6));
            BOOST_TEST((outpoint1 != outpoint7));
            BOOST_TEST((outpoint1 != outpoint8));
            BOOST_TEST((outpoint2 != outpoint5));
            BOOST_TEST((outpoint2 == outpoint6));
            BOOST_TEST((outpoint2 != outpoint7));
            BOOST_TEST((outpoint2 != outpoint8));
            BOOST_TEST((outpoint3 != outpoint5));
            BOOST_TEST((outpoint3 != outpoint6));
            BOOST_TEST((outpoint3 == outpoint7));
            BOOST_TEST((outpoint3 != outpoint8));
            BOOST_TEST((outpoint4 != outpoint5));
            BOOST_TEST((outpoint4 != outpoint6));
            BOOST_TEST((outpoint4 != outpoint7));
            BOOST_TEST((outpoint4 == outpoint8));
        }
        
        BOOST_AUTO_TEST_CASE(OutpointSerializeTest)
        {
            BOOST_TEST((Outpoint(TXID_1, INDEX_1).serialize() == OUTPOINT_1_SERIALIZED));
            BOOST_TEST((Outpoint(TXID_1, INDEX_2).serialize() == OUTPOINT_2_SERIALIZED));
            BOOST_TEST((Outpoint(TXID_2, INDEX_1).serialize() == OUTPOINT_3_SERIALIZED));
            BOOST_TEST((Outpoint(TXID_2, INDEX_2).serialize() == OUTPOINT_4_SERIALIZED));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
};
