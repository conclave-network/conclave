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

#define BOOST_TEST_MODULE Bitcoin_Output_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/bitcoin_output.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    Script SCRIPTPUBKEY_1("dup hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equalverify checksig");
    Script SCRIPTPUBKEY_2("hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equal");
    uint64_t VALUE_1 = 0xffffffffffffffff;
    uint64_t VALUE_2 = 0x0000000000000001;
    std::string BITCOIN_OUTPUT_STR =
        "{\n"
        "    \"scriptPubKey\": [\n"
        "        \"dup\",\n"
        "        \"hash160\",\n"
        "        \"[39a95df3c155a9c017c2099723a0a70ef85721b0]\",\n"
        "        \"equalverify\",\n"
        "        \"checksig\"\n"
        "    ],\n"
        "    \"value\": \"18446744073709551615\"\n"
        "}\n";
    
    pt::ptree makeBitcoinOutputTree()
    {
        std::stringstream ss(BITCOIN_OUTPUT_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinOutputConstructorsTest)
    {
        BitcoinOutput bitcoinOutputFromProps(SCRIPTPUBKEY_1, VALUE_1);
        BitcoinOutput bitcoinOutputFromPtree(makeBitcoinOutputTree());
        BOOST_TEST(bitcoinOutputFromProps.scriptPubKey == SCRIPTPUBKEY_1);
        BOOST_TEST(bitcoinOutputFromProps.value == VALUE_1);
        BOOST_TEST(bitcoinOutputFromPtree.scriptPubKey == SCRIPTPUBKEY_1);
        BOOST_TEST(bitcoinOutputFromPtree.value == VALUE_1);
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinOutputCastToPtreeTest)
    {
        BOOST_TEST(makeBitcoinOutputTree() == (pt::ptree) BitcoinOutput(SCRIPTPUBKEY_1, VALUE_1));
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinOutputCastToStringTest)
    {
        BOOST_TEST(BITCOIN_OUTPUT_STR == (std::string) BitcoinOutput(SCRIPTPUBKEY_1, VALUE_1));
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinOutputOperatorsTest)
    {
        BitcoinOutput bitcoinOutput1(SCRIPTPUBKEY_1, VALUE_1);
        BitcoinOutput bitcoinOutput2(SCRIPTPUBKEY_1, VALUE_2);
        BitcoinOutput bitcoinOutput3(SCRIPTPUBKEY_2, VALUE_1);
        BitcoinOutput bitcoinOutput4(SCRIPTPUBKEY_2, VALUE_2);
        BitcoinOutput bitcoinOutput5(SCRIPTPUBKEY_1, VALUE_1);
        BitcoinOutput bitcoinOutput6(SCRIPTPUBKEY_1, VALUE_2);
        BitcoinOutput bitcoinOutput7(SCRIPTPUBKEY_2, VALUE_1);
        BitcoinOutput bitcoinOutput8(SCRIPTPUBKEY_2, VALUE_2);
        BOOST_TEST((bitcoinOutput1 == bitcoinOutput5));
        BOOST_TEST((bitcoinOutput1 != bitcoinOutput6));
        BOOST_TEST((bitcoinOutput1 != bitcoinOutput7));
        BOOST_TEST((bitcoinOutput1 != bitcoinOutput8));
        BOOST_TEST((bitcoinOutput2 != bitcoinOutput5));
        BOOST_TEST((bitcoinOutput2 == bitcoinOutput6));
        BOOST_TEST((bitcoinOutput2 != bitcoinOutput7));
        BOOST_TEST((bitcoinOutput2 != bitcoinOutput8));
        BOOST_TEST((bitcoinOutput3 != bitcoinOutput5));
        BOOST_TEST((bitcoinOutput3 != bitcoinOutput6));
        BOOST_TEST((bitcoinOutput3 == bitcoinOutput7));
        BOOST_TEST((bitcoinOutput3 != bitcoinOutput8));
        BOOST_TEST((bitcoinOutput4 != bitcoinOutput5));
        BOOST_TEST((bitcoinOutput4 != bitcoinOutput6));
        BOOST_TEST((bitcoinOutput4 != bitcoinOutput7));
        BOOST_TEST((bitcoinOutput4 == bitcoinOutput8));
    }
}
