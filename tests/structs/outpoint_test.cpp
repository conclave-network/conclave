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

#define BOOST_TEST_MODULE Outpoint_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/outpoint.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    Hash256 TXID_1("bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17");
    Hash256 TXID_2("f477c2b95a641f733129c7d2da0474ea5e4fcf69bbac73195c52a8c12dfa0c37");
    uint32_t INDEX_1 = 0xfffffffe;
    uint32_t INDEX_2 = 0x00000001;
    std::string OUTPOINT_STR =
        "{\n"
        "    \"txId\": \"bdce750be0b18f195692248eedc1988fe831fd95f3b753e155b0ab414fb4bc17\",\n"
        "    \"index\": \"4294967294\"\n"
        "}\n";
    
    pt::ptree makeOutpointTree()
    {
        std::stringstream ss(OUTPOINT_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_CASE(OutpointConstructorsTest)
    {
        Outpoint outpointFromProps(TXID_1, INDEX_1);
        Outpoint outpointFromPtree(makeOutpointTree());
        BOOST_TEST(outpointFromProps.txId == TXID_1);
        BOOST_TEST(outpointFromProps.index == INDEX_1);
        BOOST_TEST(outpointFromPtree.txId == TXID_1);
        BOOST_TEST(outpointFromPtree.index == INDEX_1);
    }
    
    BOOST_AUTO_TEST_CASE(OutpointCastToPtreeTest)
    {
        BOOST_TEST(makeOutpointTree() == (pt::ptree) Outpoint(TXID_1, INDEX_1));
    }
    
    BOOST_AUTO_TEST_CASE(OutpointCastToStringTest)
    {
        BOOST_TEST(OUTPOINT_STR == (std::string) Outpoint(TXID_1, INDEX_1));
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
};
