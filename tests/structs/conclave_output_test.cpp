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
    const static std::vector<BYTE> CONCLAVE_OUTPUT_1_SERIALIZED =
        HEX_TO_BYTE_VECTOR("1976a91439a95df3c155a9c017c2099723a0a70ef85721b088acffffffffffffffff00");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_2_SERIALIZED =
        HEX_TO_BYTE_VECTOR("1976a91439a95df3c155a9c017c2099723a0a70ef85721b088ac010000000000000000");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_3_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17a91439a95df3c155a9c017c2099723a0a70ef85721b087ffffffffffffffff00");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_4_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17a91439a95df3c155a9c017c2099723a0a70ef85721b087010000000000000000");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_5_SERIALIZED =
        HEX_TO_BYTE_VECTOR("1976a91439a95df3c155a9c017c2099723a0a70ef85721b088acffffffffffffffff24"
                           "d4e3b484f25ffcd2725f8caba1b2644b42337c66ef874856a322c7760b0314f235000000");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_6_SERIALIZED =
        HEX_TO_BYTE_VECTOR("1976a91439a95df3c155a9c017c2099723a0a70ef85721b088ac010000000000000024"
                           "d4e3b484f25ffcd2725f8caba1b2644b42337c66ef874856a322c7760b0314f235000000");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_7_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17a91439a95df3c155a9c017c2099723a0a70ef85721b087ffffffffffffffff24"
                           "d4e3b484f25ffcd2725f8caba1b2644b42337c66ef874856a322c7760b0314f235000000");
    const static std::vector<BYTE> CONCLAVE_OUTPUT_8_SERIALIZED =
        HEX_TO_BYTE_VECTOR("17a91439a95df3c155a9c017c2099723a0a70ef85721b087010000000000000024"
                           "d4e3b484f25ffcd2725f8caba1b2644b42337c66ef874856a322c7760b0314f235000000");
    
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
            BOOST_TEST((conclaveOutput1 == conclaveOutput1));
            BOOST_TEST((conclaveOutput1 != conclaveOutput2));
            BOOST_TEST((conclaveOutput1 != conclaveOutput3));
            BOOST_TEST((conclaveOutput1 != conclaveOutput4));
            BOOST_TEST((conclaveOutput1 != conclaveOutput5));
            BOOST_TEST((conclaveOutput1 != conclaveOutput6));
            BOOST_TEST((conclaveOutput1 != conclaveOutput7));
            BOOST_TEST((conclaveOutput1 != conclaveOutput8));
            BOOST_TEST((conclaveOutput1 == conclaveOutput9));
            BOOST_TEST((conclaveOutput1 != conclaveOutput10));
            BOOST_TEST((conclaveOutput1 != conclaveOutput11));
            BOOST_TEST((conclaveOutput1 != conclaveOutput12));
            BOOST_TEST((conclaveOutput1 != conclaveOutput13));
            BOOST_TEST((conclaveOutput1 != conclaveOutput14));
            BOOST_TEST((conclaveOutput1 != conclaveOutput15));
            BOOST_TEST((conclaveOutput1 != conclaveOutput16));
            BOOST_TEST((conclaveOutput2 != conclaveOutput1));
            BOOST_TEST((conclaveOutput2 == conclaveOutput2));
            BOOST_TEST((conclaveOutput2 != conclaveOutput3));
            BOOST_TEST((conclaveOutput2 != conclaveOutput4));
            BOOST_TEST((conclaveOutput2 != conclaveOutput5));
            BOOST_TEST((conclaveOutput2 != conclaveOutput6));
            BOOST_TEST((conclaveOutput2 != conclaveOutput7));
            BOOST_TEST((conclaveOutput2 != conclaveOutput8));
            BOOST_TEST((conclaveOutput2 != conclaveOutput9));
            BOOST_TEST((conclaveOutput2 == conclaveOutput10));
            BOOST_TEST((conclaveOutput2 != conclaveOutput11));
            BOOST_TEST((conclaveOutput2 != conclaveOutput12));
            BOOST_TEST((conclaveOutput2 != conclaveOutput13));
            BOOST_TEST((conclaveOutput2 != conclaveOutput14));
            BOOST_TEST((conclaveOutput2 != conclaveOutput15));
            BOOST_TEST((conclaveOutput2 != conclaveOutput16));
            BOOST_TEST((conclaveOutput3 != conclaveOutput1));
            BOOST_TEST((conclaveOutput3 != conclaveOutput2));
            BOOST_TEST((conclaveOutput3 == conclaveOutput3));
            BOOST_TEST((conclaveOutput3 != conclaveOutput4));
            BOOST_TEST((conclaveOutput3 != conclaveOutput5));
            BOOST_TEST((conclaveOutput3 != conclaveOutput6));
            BOOST_TEST((conclaveOutput3 != conclaveOutput7));
            BOOST_TEST((conclaveOutput3 != conclaveOutput8));
            BOOST_TEST((conclaveOutput3 != conclaveOutput9));
            BOOST_TEST((conclaveOutput3 != conclaveOutput10));
            BOOST_TEST((conclaveOutput3 == conclaveOutput11));
            BOOST_TEST((conclaveOutput3 != conclaveOutput12));
            BOOST_TEST((conclaveOutput3 != conclaveOutput13));
            BOOST_TEST((conclaveOutput3 != conclaveOutput14));
            BOOST_TEST((conclaveOutput3 != conclaveOutput15));
            BOOST_TEST((conclaveOutput3 != conclaveOutput16));
            BOOST_TEST((conclaveOutput4 != conclaveOutput1));
            BOOST_TEST((conclaveOutput4 != conclaveOutput2));
            BOOST_TEST((conclaveOutput4 != conclaveOutput3));
            BOOST_TEST((conclaveOutput4 == conclaveOutput4));
            BOOST_TEST((conclaveOutput4 != conclaveOutput5));
            BOOST_TEST((conclaveOutput4 != conclaveOutput6));
            BOOST_TEST((conclaveOutput4 != conclaveOutput7));
            BOOST_TEST((conclaveOutput4 != conclaveOutput8));
            BOOST_TEST((conclaveOutput4 != conclaveOutput9));
            BOOST_TEST((conclaveOutput4 != conclaveOutput10));
            BOOST_TEST((conclaveOutput4 != conclaveOutput11));
            BOOST_TEST((conclaveOutput4 == conclaveOutput12));
            BOOST_TEST((conclaveOutput4 != conclaveOutput13));
            BOOST_TEST((conclaveOutput4 != conclaveOutput14));
            BOOST_TEST((conclaveOutput4 != conclaveOutput15));
            BOOST_TEST((conclaveOutput4 != conclaveOutput16));
            BOOST_TEST((conclaveOutput5 != conclaveOutput1));
            BOOST_TEST((conclaveOutput5 != conclaveOutput2));
            BOOST_TEST((conclaveOutput5 != conclaveOutput3));
            BOOST_TEST((conclaveOutput5 != conclaveOutput4));
            BOOST_TEST((conclaveOutput5 == conclaveOutput5));
            BOOST_TEST((conclaveOutput5 != conclaveOutput6));
            BOOST_TEST((conclaveOutput5 != conclaveOutput7));
            BOOST_TEST((conclaveOutput5 != conclaveOutput8));
            BOOST_TEST((conclaveOutput5 != conclaveOutput9));
            BOOST_TEST((conclaveOutput5 != conclaveOutput10));
            BOOST_TEST((conclaveOutput5 != conclaveOutput11));
            BOOST_TEST((conclaveOutput5 != conclaveOutput12));
            BOOST_TEST((conclaveOutput5 == conclaveOutput13));
            BOOST_TEST((conclaveOutput5 != conclaveOutput14));
            BOOST_TEST((conclaveOutput5 != conclaveOutput15));
            BOOST_TEST((conclaveOutput5 != conclaveOutput16));
            BOOST_TEST((conclaveOutput6 != conclaveOutput1));
            BOOST_TEST((conclaveOutput6 != conclaveOutput2));
            BOOST_TEST((conclaveOutput6 != conclaveOutput3));
            BOOST_TEST((conclaveOutput6 != conclaveOutput4));
            BOOST_TEST((conclaveOutput6 != conclaveOutput5));
            BOOST_TEST((conclaveOutput6 == conclaveOutput6));
            BOOST_TEST((conclaveOutput6 != conclaveOutput7));
            BOOST_TEST((conclaveOutput6 != conclaveOutput8));
            BOOST_TEST((conclaveOutput6 != conclaveOutput9));
            BOOST_TEST((conclaveOutput6 != conclaveOutput10));
            BOOST_TEST((conclaveOutput6 != conclaveOutput11));
            BOOST_TEST((conclaveOutput6 != conclaveOutput12));
            BOOST_TEST((conclaveOutput6 != conclaveOutput13));
            BOOST_TEST((conclaveOutput6 == conclaveOutput14));
            BOOST_TEST((conclaveOutput6 != conclaveOutput15));
            BOOST_TEST((conclaveOutput6 != conclaveOutput16));
            BOOST_TEST((conclaveOutput7 != conclaveOutput1));
            BOOST_TEST((conclaveOutput7 != conclaveOutput2));
            BOOST_TEST((conclaveOutput7 != conclaveOutput3));
            BOOST_TEST((conclaveOutput7 != conclaveOutput4));
            BOOST_TEST((conclaveOutput7 != conclaveOutput5));
            BOOST_TEST((conclaveOutput7 != conclaveOutput6));
            BOOST_TEST((conclaveOutput7 == conclaveOutput7));
            BOOST_TEST((conclaveOutput7 != conclaveOutput8));
            BOOST_TEST((conclaveOutput7 != conclaveOutput9));
            BOOST_TEST((conclaveOutput7 != conclaveOutput10));
            BOOST_TEST((conclaveOutput7 != conclaveOutput11));
            BOOST_TEST((conclaveOutput7 != conclaveOutput12));
            BOOST_TEST((conclaveOutput7 != conclaveOutput13));
            BOOST_TEST((conclaveOutput7 != conclaveOutput14));
            BOOST_TEST((conclaveOutput7 == conclaveOutput15));
            BOOST_TEST((conclaveOutput7 != conclaveOutput16));
            BOOST_TEST((conclaveOutput8 != conclaveOutput1));
            BOOST_TEST((conclaveOutput8 != conclaveOutput2));
            BOOST_TEST((conclaveOutput8 != conclaveOutput3));
            BOOST_TEST((conclaveOutput8 != conclaveOutput4));
            BOOST_TEST((conclaveOutput8 != conclaveOutput5));
            BOOST_TEST((conclaveOutput8 != conclaveOutput6));
            BOOST_TEST((conclaveOutput8 != conclaveOutput7));
            BOOST_TEST((conclaveOutput8 == conclaveOutput8));
            BOOST_TEST((conclaveOutput8 != conclaveOutput9));
            BOOST_TEST((conclaveOutput8 != conclaveOutput10));
            BOOST_TEST((conclaveOutput8 != conclaveOutput11));
            BOOST_TEST((conclaveOutput8 != conclaveOutput12));
            BOOST_TEST((conclaveOutput8 != conclaveOutput13));
            BOOST_TEST((conclaveOutput8 != conclaveOutput14));
            BOOST_TEST((conclaveOutput8 != conclaveOutput15));
            BOOST_TEST((conclaveOutput8 == conclaveOutput16));
            BOOST_TEST((conclaveOutput9 == conclaveOutput1));
            BOOST_TEST((conclaveOutput9 != conclaveOutput2));
            BOOST_TEST((conclaveOutput9 != conclaveOutput3));
            BOOST_TEST((conclaveOutput9 != conclaveOutput4));
            BOOST_TEST((conclaveOutput9 != conclaveOutput5));
            BOOST_TEST((conclaveOutput9 != conclaveOutput6));
            BOOST_TEST((conclaveOutput9 != conclaveOutput7));
            BOOST_TEST((conclaveOutput9 != conclaveOutput8));
            BOOST_TEST((conclaveOutput9 == conclaveOutput9));
            BOOST_TEST((conclaveOutput9 != conclaveOutput10));
            BOOST_TEST((conclaveOutput9 != conclaveOutput11));
            BOOST_TEST((conclaveOutput9 != conclaveOutput12));
            BOOST_TEST((conclaveOutput9 != conclaveOutput13));
            BOOST_TEST((conclaveOutput9 != conclaveOutput14));
            BOOST_TEST((conclaveOutput9 != conclaveOutput15));
            BOOST_TEST((conclaveOutput9 != conclaveOutput16));
            BOOST_TEST((conclaveOutput10 != conclaveOutput1));
            BOOST_TEST((conclaveOutput10 == conclaveOutput2));
            BOOST_TEST((conclaveOutput10 != conclaveOutput3));
            BOOST_TEST((conclaveOutput10 != conclaveOutput4));
            BOOST_TEST((conclaveOutput10 != conclaveOutput5));
            BOOST_TEST((conclaveOutput10 != conclaveOutput6));
            BOOST_TEST((conclaveOutput10 != conclaveOutput7));
            BOOST_TEST((conclaveOutput10 != conclaveOutput8));
            BOOST_TEST((conclaveOutput10 != conclaveOutput9));
            BOOST_TEST((conclaveOutput10 == conclaveOutput10));
            BOOST_TEST((conclaveOutput10 != conclaveOutput11));
            BOOST_TEST((conclaveOutput10 != conclaveOutput12));
            BOOST_TEST((conclaveOutput10 != conclaveOutput13));
            BOOST_TEST((conclaveOutput10 != conclaveOutput14));
            BOOST_TEST((conclaveOutput10 != conclaveOutput15));
            BOOST_TEST((conclaveOutput10 != conclaveOutput16));
            BOOST_TEST((conclaveOutput11 != conclaveOutput1));
            BOOST_TEST((conclaveOutput11 != conclaveOutput2));
            BOOST_TEST((conclaveOutput11 == conclaveOutput3));
            BOOST_TEST((conclaveOutput11 != conclaveOutput4));
            BOOST_TEST((conclaveOutput11 != conclaveOutput5));
            BOOST_TEST((conclaveOutput11 != conclaveOutput6));
            BOOST_TEST((conclaveOutput11 != conclaveOutput7));
            BOOST_TEST((conclaveOutput11 != conclaveOutput8));
            BOOST_TEST((conclaveOutput11 != conclaveOutput9));
            BOOST_TEST((conclaveOutput11 != conclaveOutput10));
            BOOST_TEST((conclaveOutput11 == conclaveOutput11));
            BOOST_TEST((conclaveOutput11 != conclaveOutput12));
            BOOST_TEST((conclaveOutput11 != conclaveOutput13));
            BOOST_TEST((conclaveOutput11 != conclaveOutput14));
            BOOST_TEST((conclaveOutput11 != conclaveOutput15));
            BOOST_TEST((conclaveOutput11 != conclaveOutput16));
            BOOST_TEST((conclaveOutput12 != conclaveOutput1));
            BOOST_TEST((conclaveOutput12 != conclaveOutput2));
            BOOST_TEST((conclaveOutput12 != conclaveOutput3));
            BOOST_TEST((conclaveOutput12 == conclaveOutput4));
            BOOST_TEST((conclaveOutput12 != conclaveOutput5));
            BOOST_TEST((conclaveOutput12 != conclaveOutput6));
            BOOST_TEST((conclaveOutput12 != conclaveOutput7));
            BOOST_TEST((conclaveOutput12 != conclaveOutput8));
            BOOST_TEST((conclaveOutput12 != conclaveOutput9));
            BOOST_TEST((conclaveOutput12 != conclaveOutput10));
            BOOST_TEST((conclaveOutput12 != conclaveOutput11));
            BOOST_TEST((conclaveOutput12 == conclaveOutput12));
            BOOST_TEST((conclaveOutput12 != conclaveOutput13));
            BOOST_TEST((conclaveOutput12 != conclaveOutput14));
            BOOST_TEST((conclaveOutput12 != conclaveOutput15));
            BOOST_TEST((conclaveOutput12 != conclaveOutput16));
            BOOST_TEST((conclaveOutput13 != conclaveOutput1));
            BOOST_TEST((conclaveOutput13 != conclaveOutput2));
            BOOST_TEST((conclaveOutput13 != conclaveOutput3));
            BOOST_TEST((conclaveOutput13 != conclaveOutput4));
            BOOST_TEST((conclaveOutput13 == conclaveOutput5));
            BOOST_TEST((conclaveOutput13 != conclaveOutput6));
            BOOST_TEST((conclaveOutput13 != conclaveOutput7));
            BOOST_TEST((conclaveOutput13 != conclaveOutput8));
            BOOST_TEST((conclaveOutput13 != conclaveOutput9));
            BOOST_TEST((conclaveOutput13 != conclaveOutput10));
            BOOST_TEST((conclaveOutput13 != conclaveOutput11));
            BOOST_TEST((conclaveOutput13 != conclaveOutput12));
            BOOST_TEST((conclaveOutput13 == conclaveOutput13));
            BOOST_TEST((conclaveOutput13 != conclaveOutput14));
            BOOST_TEST((conclaveOutput13 != conclaveOutput15));
            BOOST_TEST((conclaveOutput13 != conclaveOutput16));
            BOOST_TEST((conclaveOutput14 != conclaveOutput1));
            BOOST_TEST((conclaveOutput14 != conclaveOutput2));
            BOOST_TEST((conclaveOutput14 != conclaveOutput3));
            BOOST_TEST((conclaveOutput14 != conclaveOutput4));
            BOOST_TEST((conclaveOutput14 != conclaveOutput5));
            BOOST_TEST((conclaveOutput14 == conclaveOutput6));
            BOOST_TEST((conclaveOutput14 != conclaveOutput7));
            BOOST_TEST((conclaveOutput14 != conclaveOutput8));
            BOOST_TEST((conclaveOutput14 != conclaveOutput9));
            BOOST_TEST((conclaveOutput14 != conclaveOutput10));
            BOOST_TEST((conclaveOutput14 != conclaveOutput11));
            BOOST_TEST((conclaveOutput14 != conclaveOutput12));
            BOOST_TEST((conclaveOutput14 != conclaveOutput13));
            BOOST_TEST((conclaveOutput14 == conclaveOutput14));
            BOOST_TEST((conclaveOutput14 != conclaveOutput15));
            BOOST_TEST((conclaveOutput14 != conclaveOutput16));
            BOOST_TEST((conclaveOutput15 != conclaveOutput1));
            BOOST_TEST((conclaveOutput15 != conclaveOutput2));
            BOOST_TEST((conclaveOutput15 != conclaveOutput3));
            BOOST_TEST((conclaveOutput15 != conclaveOutput4));
            BOOST_TEST((conclaveOutput15 != conclaveOutput5));
            BOOST_TEST((conclaveOutput15 != conclaveOutput6));
            BOOST_TEST((conclaveOutput15 == conclaveOutput7));
            BOOST_TEST((conclaveOutput15 != conclaveOutput8));
            BOOST_TEST((conclaveOutput15 != conclaveOutput9));
            BOOST_TEST((conclaveOutput15 != conclaveOutput10));
            BOOST_TEST((conclaveOutput15 != conclaveOutput11));
            BOOST_TEST((conclaveOutput15 != conclaveOutput12));
            BOOST_TEST((conclaveOutput15 != conclaveOutput13));
            BOOST_TEST((conclaveOutput15 != conclaveOutput14));
            BOOST_TEST((conclaveOutput15 == conclaveOutput15));
            BOOST_TEST((conclaveOutput15 != conclaveOutput16));
            BOOST_TEST((conclaveOutput16 != conclaveOutput1));
            BOOST_TEST((conclaveOutput16 != conclaveOutput2));
            BOOST_TEST((conclaveOutput16 != conclaveOutput3));
            BOOST_TEST((conclaveOutput16 != conclaveOutput4));
            BOOST_TEST((conclaveOutput16 != conclaveOutput5));
            BOOST_TEST((conclaveOutput16 != conclaveOutput6));
            BOOST_TEST((conclaveOutput16 != conclaveOutput7));
            BOOST_TEST((conclaveOutput16 == conclaveOutput8));
            BOOST_TEST((conclaveOutput16 != conclaveOutput9));
            BOOST_TEST((conclaveOutput16 != conclaveOutput10));
            BOOST_TEST((conclaveOutput16 != conclaveOutput11));
            BOOST_TEST((conclaveOutput16 != conclaveOutput12));
            BOOST_TEST((conclaveOutput16 != conclaveOutput13));
            BOOST_TEST((conclaveOutput16 != conclaveOutput14));
            BOOST_TEST((conclaveOutput16 != conclaveOutput15));
            BOOST_TEST((conclaveOutput16 == conclaveOutput16));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveOutputSerializeTest)
        {
            ConclaveOutput conclaveOutput1(SCRIPTPUBKEY_1, VALUE_1);
            ConclaveOutput conclaveOutput2(SCRIPTPUBKEY_1, VALUE_2);
            ConclaveOutput conclaveOutput3(SCRIPTPUBKEY_2, VALUE_1);
            ConclaveOutput conclaveOutput4(SCRIPTPUBKEY_2, VALUE_2);
            ConclaveOutput conclaveOutput5(SCRIPTPUBKEY_1, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutput6(SCRIPTPUBKEY_1, VALUE_2, PREDECESSOR_1);
            ConclaveOutput conclaveOutput7(SCRIPTPUBKEY_2, VALUE_1, PREDECESSOR_1);
            ConclaveOutput conclaveOutput8(SCRIPTPUBKEY_2, VALUE_2, PREDECESSOR_1);
            BOOST_TEST((conclaveOutput1.serialize() == CONCLAVE_OUTPUT_1_SERIALIZED));
            BOOST_TEST((conclaveOutput2.serialize() == CONCLAVE_OUTPUT_2_SERIALIZED));
            BOOST_TEST((conclaveOutput3.serialize() == CONCLAVE_OUTPUT_3_SERIALIZED));
            BOOST_TEST((conclaveOutput4.serialize() == CONCLAVE_OUTPUT_4_SERIALIZED));
            BOOST_TEST((conclaveOutput5.serialize() == CONCLAVE_OUTPUT_5_SERIALIZED));
            BOOST_TEST((conclaveOutput6.serialize() == CONCLAVE_OUTPUT_6_SERIALIZED));
            BOOST_TEST((conclaveOutput7.serialize() == CONCLAVE_OUTPUT_7_SERIALIZED));
            BOOST_TEST((conclaveOutput8.serialize() == CONCLAVE_OUTPUT_8_SERIALIZED));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
