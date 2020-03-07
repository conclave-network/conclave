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

#define BOOST_TEST_MODULE Conclave_Output_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/conclave_output.h"
#include "../../src/util/serialization.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    const static Script SCRIPTPUBKEY_1("dup hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equalverify checksig");
    const static Script SCRIPTPUBKEY_2("hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equal");
    const static uint64_t VALUE_1 = 0xffffffffffffffff;
    const static uint64_t VALUE_2 = 0x0000000000000001;
    const static Outpoint PREDECESSOR_1("f214030b76c722a3564887ef667c33424b64b2a1ab8c5f72d2fc5ff284b4e3d4", 53);
    std::string CONCLAVE_OUTPUT_STR =
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
    std::string CONCLAVE_OUTPUT_STR_WITH_PREDECESSOR =
        "{\n"
        "    \"scriptPubKey\": [\n"
        "        \"dup\",\n"
        "        \"hash160\",\n"
        "        \"[39a95df3c155a9c017c2099723a0a70ef85721b0]\",\n"
        "        \"equalverify\",\n"
        "        \"checksig\"\n"
        "    ],\n"
        "    \"value\": \"18446744073709551615\",\n"
        "    \"predecessor\": {\n"
        "        \"txId\": \"f214030b76c722a3564887ef667c33424b64b2a1ab8c5f72d2fc5ff284b4e3d4\",\n"
        "        \"index\": \"53\"\n"
        "    }\n"
        "}\n";
    
    pt::ptree makeConclaveOutputTree()
    {
        std::stringstream ss(CONCLAVE_OUTPUT_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    pt::ptree makeConclaveOutputTreeWithPredecessor()
    {
        std::stringstream ss(CONCLAVE_OUTPUT_STR_WITH_PREDECESSOR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_SUITE(ConclaveOutputTestSuite)
        
        BOOST_AUTO_TEST_CASE(ConclaveOutputConstructorsTest)
        {
            ConclaveOutput conclaveOutputFromProps(SCRIPTPUBKEY_1, VALUE_1);
            ConclaveOutput conclaveOutputFromPropsWithPredecessor(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutputFromPtree(makeConclaveOutputTree());
            ConclaveOutput conclaveOutputFromPtreeWithPredecessor(makeConclaveOutputTreeWithPredecessor());
            BOOST_TEST((conclaveOutputFromProps.scriptPubKey == SCRIPTPUBKEY_1));
            BOOST_TEST((conclaveOutputFromProps.value == VALUE_1));
            BOOST_TEST((conclaveOutputFromPropsWithPredecessor.scriptPubKey == SCRIPTPUBKEY_1));
            BOOST_TEST((conclaveOutputFromPropsWithPredecessor.value == VALUE_1));
            BOOST_TEST((conclaveOutputFromPropsWithPredecessor.predecessor == PREDECESSOR_1));
            BOOST_TEST((conclaveOutputFromPtree.scriptPubKey == SCRIPTPUBKEY_1));
            BOOST_TEST((conclaveOutputFromPtree.value == VALUE_1));
            BOOST_TEST((conclaveOutputFromPtreeWithPredecessor.scriptPubKey == SCRIPTPUBKEY_1));
            BOOST_TEST((conclaveOutputFromPtreeWithPredecessor.value == VALUE_1));
            BOOST_TEST((conclaveOutputFromPtreeWithPredecessor.predecessor == PREDECESSOR_1));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveOutputCastToPtreeTest)
        {
            BOOST_TEST((makeConclaveOutputTree() == (pt::ptree) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1)));
            BOOST_TEST((makeConclaveOutputTreeWithPredecessor() ==
                        (pt::ptree) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1)));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveOutputCastToStringTest)
        {
            BOOST_TEST((CONCLAVE_OUTPUT_STR == (std::string) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1)));
            BOOST_TEST((CONCLAVE_OUTPUT_STR_WITH_PREDECESSOR ==
                        (std::string) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1)));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveOutputOperatorsTest)
        {
            ConclaveOutput conclaveOutput1(SCRIPTPUBKEY_1, VALUE_1);
            ConclaveOutput conclaveOutput2(SCRIPTPUBKEY_1, VALUE_2);
            ConclaveOutput conclaveOutput3(SCRIPTPUBKEY_2, VALUE_1);
            ConclaveOutput conclaveOutput4(SCRIPTPUBKEY_2, VALUE_2);
            ConclaveOutput conclaveOutput5(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutput6(SCRIPTPUBKEY_1, VALUE_2, PREDECESSOR_1);
            ConclaveOutput conclaveOutput7(SCRIPTPUBKEY_2, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutput8(SCRIPTPUBKEY_2, VALUE_2, PREDECESSOR_1);
            ConclaveOutput conclaveOutput9(SCRIPTPUBKEY_1, VALUE_1);
            ConclaveOutput conclaveOutput10(SCRIPTPUBKEY_1, VALUE_2);
            ConclaveOutput conclaveOutput11(SCRIPTPUBKEY_2, VALUE_1);
            ConclaveOutput conclaveOutput12(SCRIPTPUBKEY_2, VALUE_2);
            ConclaveOutput conclaveOutput13(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutput14(SCRIPTPUBKEY_1, VALUE_2, PREDECESSOR_1);
            ConclaveOutput conclaveOutput15(SCRIPTPUBKEY_2, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutput16(SCRIPTPUBKEY_2, VALUE_2, PREDECESSOR_1);
            // TODO - assertions
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveOutputSerializeTest)
        {
            ConclaveOutput conclaveOutput1(SCRIPTPUBKEY_1, VALUE_1);
            ConclaveOutput conclaveOutput5(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1);
            std::vector<BYTE> scriptPubKey1Serialized = SCRIPTPUBKEY_1.serialize();
            std::vector<BYTE> scriptPubKey2Serialized = SCRIPTPUBKEY_2.serialize();
            std::vector<BYTE> value1Serialized = serializeU64(VALUE_1);
            std::vector<BYTE> value2Serialized = serializeU64(VALUE_2);
            std::vector<BYTE> conclaveOutput1Serialized = conclaveOutput1.serialize();
            std::vector<BYTE> conclaveOutput5Serialized = conclaveOutput5.serialize();
            BOOST_TEST((conclaveOutput1Serialized == HEX_TO_BYTE_VECTOR
                ("1976a91439a95df3c155a9c017c2099723a0a70ef85721b088acffffffffffffffff00")));
            BOOST_TEST((conclaveOutput5Serialized == HEX_TO_BYTE_VECTOR(
                "1976a91439a95df3c155a9c017c2099723a0a70ef85721b088acffffffffffffffff"
                "24d4e3b484f25ffcd2725f8caba1b2644b42337c66ef874856a322c7760b0314f235000000")));
            // TODO - more
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
