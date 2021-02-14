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

#define BOOST_TEST_MODULE BITCOIN_OUTPUT_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/bitcoin_output.h"
#include "../../src/util/serialization.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    const static uint64_t VALUE_1 = 0xffffffffffffffff;
    const static uint64_t VALUE_2 = 0x0000000000000001;
    const static Script SCRIPTPUBKEY_1("dup hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equalverify checksig");
    const static Script SCRIPTPUBKEY_2("hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equal");
    std::string BITCOIN_OUTPUT_STR =
        "{\n"
        "    \"value\": \"18446744073709551615\",\n"
        "    \"scriptPubKey\": [\n"
        "        \"dup\",\n"
        "        \"hash160\",\n"
        "        \"[39a95df3c155a9c017c2099723a0a70ef85721b0]\",\n"
        "        \"equalverify\",\n"
        "        \"checksig\"\n"
        "    ]\n"
        "}\n";
    const static std::vector<BYTE> BITCOIN_OUTPUT_1_SERIALIZED =
        HEX_TO_BYTE_VECTOR("ffffffffffffffff1976a91439a95df3c155a9c017c2099723a0a70ef85721b088ac");
    const static std::vector<BYTE> BITCOIN_OUTPUT_2_SERIALIZED =
        HEX_TO_BYTE_VECTOR("ffffffffffffffff17a91439a95df3c155a9c017c2099723a0a70ef85721b087");
    const static std::vector<BYTE> BITCOIN_OUTPUT_3_SERIALIZED =
        HEX_TO_BYTE_VECTOR("01000000000000001976a91439a95df3c155a9c017c2099723a0a70ef85721b088ac");
    const static std::vector<BYTE> BITCOIN_OUTPUT_4_SERIALIZED =
        HEX_TO_BYTE_VECTOR("010000000000000017a91439a95df3c155a9c017c2099723a0a70ef85721b087");
    
    pt::ptree makeBitcoinOutputTree()
    {
        std::stringstream ss(BITCOIN_OUTPUT_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_SUITE(BitcoinOutputTestSuite)
        
        BOOST_AUTO_TEST_CASE(BitcoinOutputConstructorsTest)
        {
            BitcoinOutput bitcoinOutputFromProps(VALUE_1, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutputFromPtree(makeBitcoinOutputTree());
            BOOST_TEST((bitcoinOutputFromProps.value == VALUE_1));
            BOOST_TEST((bitcoinOutputFromProps.scriptPubKey == SCRIPTPUBKEY_1));
            BOOST_TEST((bitcoinOutputFromPtree.value == VALUE_1));
            BOOST_TEST((bitcoinOutputFromPtree.scriptPubKey == SCRIPTPUBKEY_1));
        }
        
        BOOST_AUTO_TEST_CASE(BitcoinOutputCastToPtreeTest)
        {
            BOOST_TEST((makeBitcoinOutputTree() == (pt::ptree) BitcoinOutput(VALUE_1, SCRIPTPUBKEY_1)));
        }
        
        BOOST_AUTO_TEST_CASE(BitcoinOutputCastToStringTest)
        {
            BOOST_TEST((BITCOIN_OUTPUT_STR == (std::string) BitcoinOutput(VALUE_1, SCRIPTPUBKEY_1)));
        }
        
        BOOST_AUTO_TEST_CASE(BitcoinOutputOperatorsTest)
        {
            BitcoinOutput bitcoinOutput1(VALUE_1, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutput2(VALUE_1, SCRIPTPUBKEY_2);
            BitcoinOutput bitcoinOutput3(VALUE_2, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutput4(VALUE_2, SCRIPTPUBKEY_2);
            BitcoinOutput bitcoinOutput5(VALUE_1, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutput6(VALUE_1, SCRIPTPUBKEY_2);
            BitcoinOutput bitcoinOutput7(VALUE_2, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutput8(VALUE_2, SCRIPTPUBKEY_2);
            BOOST_TEST((bitcoinOutput1 == bitcoinOutput1));
            BOOST_TEST((bitcoinOutput1 != bitcoinOutput2));
            BOOST_TEST((bitcoinOutput1 != bitcoinOutput3));
            BOOST_TEST((bitcoinOutput1 != bitcoinOutput4));
            BOOST_TEST((bitcoinOutput1 == bitcoinOutput5));
            BOOST_TEST((bitcoinOutput1 != bitcoinOutput6));
            BOOST_TEST((bitcoinOutput1 != bitcoinOutput7));
            BOOST_TEST((bitcoinOutput1 != bitcoinOutput8));
            BOOST_TEST((bitcoinOutput2 != bitcoinOutput1));
            BOOST_TEST((bitcoinOutput2 == bitcoinOutput2));
            BOOST_TEST((bitcoinOutput2 != bitcoinOutput3));
            BOOST_TEST((bitcoinOutput2 != bitcoinOutput4));
            BOOST_TEST((bitcoinOutput2 != bitcoinOutput5));
            BOOST_TEST((bitcoinOutput2 == bitcoinOutput6));
            BOOST_TEST((bitcoinOutput2 != bitcoinOutput7));
            BOOST_TEST((bitcoinOutput2 != bitcoinOutput8));
            BOOST_TEST((bitcoinOutput3 != bitcoinOutput1));
            BOOST_TEST((bitcoinOutput3 != bitcoinOutput2));
            BOOST_TEST((bitcoinOutput3 == bitcoinOutput3));
            BOOST_TEST((bitcoinOutput3 != bitcoinOutput4));
            BOOST_TEST((bitcoinOutput3 != bitcoinOutput5));
            BOOST_TEST((bitcoinOutput3 != bitcoinOutput6));
            BOOST_TEST((bitcoinOutput3 == bitcoinOutput7));
            BOOST_TEST((bitcoinOutput3 != bitcoinOutput8));
            BOOST_TEST((bitcoinOutput4 != bitcoinOutput1));
            BOOST_TEST((bitcoinOutput4 != bitcoinOutput2));
            BOOST_TEST((bitcoinOutput4 != bitcoinOutput3));
            BOOST_TEST((bitcoinOutput4 == bitcoinOutput4));
            BOOST_TEST((bitcoinOutput4 != bitcoinOutput5));
            BOOST_TEST((bitcoinOutput4 != bitcoinOutput6));
            BOOST_TEST((bitcoinOutput4 != bitcoinOutput7));
            BOOST_TEST((bitcoinOutput4 == bitcoinOutput8));
            BOOST_TEST((bitcoinOutput5 == bitcoinOutput1));
            BOOST_TEST((bitcoinOutput5 != bitcoinOutput2));
            BOOST_TEST((bitcoinOutput5 != bitcoinOutput3));
            BOOST_TEST((bitcoinOutput5 != bitcoinOutput4));
            BOOST_TEST((bitcoinOutput5 == bitcoinOutput5));
            BOOST_TEST((bitcoinOutput5 != bitcoinOutput6));
            BOOST_TEST((bitcoinOutput5 != bitcoinOutput7));
            BOOST_TEST((bitcoinOutput5 != bitcoinOutput8));
            BOOST_TEST((bitcoinOutput6 != bitcoinOutput1));
            BOOST_TEST((bitcoinOutput6 == bitcoinOutput2));
            BOOST_TEST((bitcoinOutput6 != bitcoinOutput3));
            BOOST_TEST((bitcoinOutput6 != bitcoinOutput4));
            BOOST_TEST((bitcoinOutput6 != bitcoinOutput5));
            BOOST_TEST((bitcoinOutput6 == bitcoinOutput6));
            BOOST_TEST((bitcoinOutput6 != bitcoinOutput7));
            BOOST_TEST((bitcoinOutput6 != bitcoinOutput8));
            BOOST_TEST((bitcoinOutput7 != bitcoinOutput1));
            BOOST_TEST((bitcoinOutput7 != bitcoinOutput2));
            BOOST_TEST((bitcoinOutput7 == bitcoinOutput3));
            BOOST_TEST((bitcoinOutput7 != bitcoinOutput4));
            BOOST_TEST((bitcoinOutput7 != bitcoinOutput5));
            BOOST_TEST((bitcoinOutput7 != bitcoinOutput6));
            BOOST_TEST((bitcoinOutput7 == bitcoinOutput7));
            BOOST_TEST((bitcoinOutput7 != bitcoinOutput8));
            BOOST_TEST((bitcoinOutput8 != bitcoinOutput1));
            BOOST_TEST((bitcoinOutput8 != bitcoinOutput2));
            BOOST_TEST((bitcoinOutput8 != bitcoinOutput3));
            BOOST_TEST((bitcoinOutput8 == bitcoinOutput4));
            BOOST_TEST((bitcoinOutput8 != bitcoinOutput5));
            BOOST_TEST((bitcoinOutput8 != bitcoinOutput6));
            BOOST_TEST((bitcoinOutput8 != bitcoinOutput7));
            BOOST_TEST((bitcoinOutput8 == bitcoinOutput8));
        }
        
        BOOST_AUTO_TEST_CASE(BitcoinOutputSerializeTest)
        {
            BitcoinOutput bitcoinOutput1(VALUE_1, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutput2(VALUE_1, SCRIPTPUBKEY_2);
            BitcoinOutput bitcoinOutput3(VALUE_2, SCRIPTPUBKEY_1);
            BitcoinOutput bitcoinOutput4(VALUE_2, SCRIPTPUBKEY_2);
            BOOST_TEST((bitcoinOutput1.serialize() == BITCOIN_OUTPUT_1_SERIALIZED));
            BOOST_TEST((bitcoinOutput2.serialize() == BITCOIN_OUTPUT_2_SERIALIZED));
            BOOST_TEST((bitcoinOutput3.serialize() == BITCOIN_OUTPUT_3_SERIALIZED));
            BOOST_TEST((bitcoinOutput4.serialize() == BITCOIN_OUTPUT_4_SERIALIZED));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
