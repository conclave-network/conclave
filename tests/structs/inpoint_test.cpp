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

#define BOOST_TEST_MODULE Inpoint_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/inpoint.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    const static Hash256 TXID_1("bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17");
    const static Hash256 TXID_2("f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37");
    const static uint32_t INDEX_1 = 0xfffffffe;
    const static uint32_t INDEX_2 = 0x00000001;
    const static std::string INPOINT_1_STR =
        "{\n"
        "    \"txId\": \"bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17\",\n"
        "    \"index\": \"4294967294\"\n"
        "}\n";
    const static std::string INPOINT_2_STR =
        "{\n"
        "    \"txId\": \"bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17\",\n"
        "    \"index\": \"1\"\n"
        "}\n";
    const static std::string INPOINT_3_STR =
        "{\n"
        "    \"txId\": \"f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37\",\n"
        "    \"index\": \"4294967294\"\n"
        "}\n";
    const static std::string INPOINT_4_STR =
        "{\n"
        "    \"txId\": \"f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37\",\n"
        "    \"index\": \"1\"\n"
        "}\n";
    const static pt::ptree INPOINT_1_PTREE = parseJson(INPOINT_1_STR);
    const static pt::ptree INPOINT_2_PTREE = parseJson(INPOINT_2_STR);
    const static pt::ptree INPOINT_3_PTREE = parseJson(INPOINT_3_STR);
    const static pt::ptree INPOINT_4_PTREE = parseJson(INPOINT_4_STR);
    const static std::vector<BYTE> INPOINT_1_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17bcb44f41abb055e153b7f395fd31e88f98c1ed8e249256198fb1e00b75cebdfeffffff");
    const static std::vector<BYTE> INPOINT_2_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17bcb44f41abb055e153b7f395fd31e88f98c1ed8e249256198fb1e00b75cebd01000000");
    const static std::vector<BYTE> INPOINT_3_SERIALIZED =
        HEX_TO_BYTE_VECTOR("370cfa2dc1a8525c1973acbb69cf4f5eea7404dad2c72931731f645ab9c277f4feffffff");
    const static std::vector<BYTE> INPOINT_4_SERIALIZED =
        HEX_TO_BYTE_VECTOR("370cfa2dc1a8525c1973acbb69cf4f5eea7404dad2c72931731f645ab9c277f401000000");
    BOOST_AUTO_TEST_SUITE(InpointTestSuite)
        
        BOOST_AUTO_TEST_CASE(InpointConstructorsTest)
        {
            Inpoint inpointFromProps(TXID_1, INDEX_1);
            Inpoint inpointFromPtree(INPOINT_1_PTREE);
            BOOST_TEST((inpointFromProps.txId == TXID_1));
            BOOST_TEST((inpointFromProps.index == INDEX_1));
            BOOST_TEST((inpointFromPtree.txId == TXID_1));
            BOOST_TEST((inpointFromPtree.index == INDEX_1));
        }
        
        BOOST_AUTO_TEST_CASE(InpointCastToStringTest)
        {
            BOOST_TEST((INPOINT_1_STR == (std::string) Inpoint(TXID_1, INDEX_1)));
            BOOST_TEST((INPOINT_2_STR == (std::string) Inpoint(TXID_1, INDEX_2)));
            BOOST_TEST((INPOINT_3_STR == (std::string) Inpoint(TXID_2, INDEX_1)));
            BOOST_TEST((INPOINT_4_STR == (std::string) Inpoint(TXID_2, INDEX_2)));
        }
        
        BOOST_AUTO_TEST_CASE(InpointCastToPtreeTest)
        {
            BOOST_TEST((INPOINT_1_PTREE == (pt::ptree) Inpoint(TXID_1, INDEX_1)));
            BOOST_TEST((INPOINT_2_PTREE == (pt::ptree) Inpoint(TXID_1, INDEX_2)));
            BOOST_TEST((INPOINT_3_PTREE == (pt::ptree) Inpoint(TXID_2, INDEX_1)));
            BOOST_TEST((INPOINT_4_PTREE == (pt::ptree) Inpoint(TXID_2, INDEX_2)));
        }
        
        BOOST_AUTO_TEST_CASE(InpointOperatorsTest)
        {
            Inpoint inpoint1(TXID_1, INDEX_1);
            Inpoint inpoint2(TXID_1, INDEX_2);
            Inpoint inpoint3(TXID_2, INDEX_1);
            Inpoint inpoint4(TXID_2, INDEX_2);
            Inpoint inpoint5(TXID_1, INDEX_1);
            Inpoint inpoint6(TXID_1, INDEX_2);
            Inpoint inpoint7(TXID_2, INDEX_1);
            Inpoint inpoint8(TXID_2, INDEX_2);
            BOOST_TEST((inpoint1 == inpoint5));
            BOOST_TEST((inpoint1 != inpoint6));
            BOOST_TEST((inpoint1 != inpoint7));
            BOOST_TEST((inpoint1 != inpoint8));
            BOOST_TEST((inpoint2 != inpoint5));
            BOOST_TEST((inpoint2 == inpoint6));
            BOOST_TEST((inpoint2 != inpoint7));
            BOOST_TEST((inpoint2 != inpoint8));
            BOOST_TEST((inpoint3 != inpoint5));
            BOOST_TEST((inpoint3 != inpoint6));
            BOOST_TEST((inpoint3 == inpoint7));
            BOOST_TEST((inpoint3 != inpoint8));
            BOOST_TEST((inpoint4 != inpoint5));
            BOOST_TEST((inpoint4 != inpoint6));
            BOOST_TEST((inpoint4 != inpoint7));
            BOOST_TEST((inpoint4 == inpoint8));
        }
        
        BOOST_AUTO_TEST_CASE(InpointSerializeTest)
        {
            BOOST_TEST((Inpoint(TXID_1, INDEX_1).serialize() == INPOINT_1_SERIALIZED));
            BOOST_TEST((Inpoint(TXID_1, INDEX_2).serialize() == INPOINT_2_SERIALIZED));
            BOOST_TEST((Inpoint(TXID_2, INDEX_1).serialize() == INPOINT_3_SERIALIZED));
            BOOST_TEST((Inpoint(TXID_2, INDEX_2).serialize() == INPOINT_4_SERIALIZED));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
};
