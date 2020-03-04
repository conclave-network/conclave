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
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    const static Script SCRIPTPUBKEY_1("dup hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equalverify checksig");
    const static Script SCRIPTPUBKEY_2("hash160 [39a95df3c155a9c017c2099723a0a70ef85721b0] equal");
    const static uint64_t VALUE_1 = 0xffffffffffffffff;
    const static uint64_t VALUE_2 = 0x0000000000000001;
    const static Outpoint PREDESESSOR_1("f214030b76c722a3564887ef667c33424b64b2a1ab8c5f72d2fc5ff284b4e3d4", 53);
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
    std::string CONCLAVE_OUTPUT_STR_WITH_PREDESESSOR =
        "{\n"
        "    \"scriptPubKey\": [\n"
        "        \"dup\",\n"
        "        \"hash160\",\n"
        "        \"[39a95df3c155a9c017c2099723a0a70ef85721b0]\",\n"
        "        \"equalverify\",\n"
        "        \"checksig\"\n"
        "    ],\n"
        "    \"value\": \"18446744073709551615\",\n"
        "    \"predesessor\": {\n"
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
    
    pt::ptree makeConclaveOutputTreeWithPredesessor()
    {
        std::stringstream ss(CONCLAVE_OUTPUT_STR_WITH_PREDESESSOR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_CASE(ConclaveOutputConstructorsTest)
    {
        ConclaveOutput conclaveOutputFromProps(SCRIPTPUBKEY_1, VALUE_1);
        ConclaveOutput conclaveOutputFromPropsWithPredesessor(SCRIPTPUBKEY_1, VALUE_1, PREDESESSOR_1);
        ConclaveOutput conclaveOutputFromPtree(makeConclaveOutputTree());
        ConclaveOutput conclaveOutputFromPtreeWithPredesessor(makeConclaveOutputTreeWithPredesessor());
        BOOST_TEST((conclaveOutputFromProps.scriptPubKey == SCRIPTPUBKEY_1));
        BOOST_TEST((conclaveOutputFromProps.value == VALUE_1));
        BOOST_TEST((conclaveOutputFromPropsWithPredesessor.scriptPubKey == SCRIPTPUBKEY_1));
        BOOST_TEST((conclaveOutputFromPropsWithPredesessor.value == VALUE_1));
        BOOST_TEST((conclaveOutputFromPropsWithPredesessor.predesessor == PREDESESSOR_1));
        BOOST_TEST((conclaveOutputFromPtree.scriptPubKey == SCRIPTPUBKEY_1));
        BOOST_TEST((conclaveOutputFromPtree.value == VALUE_1));
        BOOST_TEST((conclaveOutputFromPtreeWithPredesessor.scriptPubKey == SCRIPTPUBKEY_1));
        BOOST_TEST((conclaveOutputFromPtreeWithPredesessor.value == VALUE_1));
        BOOST_TEST((conclaveOutputFromPtreeWithPredesessor.predesessor == PREDESESSOR_1));
    }
    
    BOOST_AUTO_TEST_CASE(ConclaveOutputCastToPtreeTest)
    {
        BOOST_TEST((makeConclaveOutputTree() == (pt::ptree) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1)));
        BOOST_TEST((makeConclaveOutputTreeWithPredesessor() ==
                    (pt::ptree) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1, PREDESESSOR_1)));
    }
    
    BOOST_AUTO_TEST_CASE(ConclaveOutputCastToStringTest)
    {
        BOOST_TEST((CONCLAVE_OUTPUT_STR == (std::string) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1)));
        BOOST_TEST((CONCLAVE_OUTPUT_STR_WITH_PREDESESSOR ==
                    (std::string) ConclaveOutput(SCRIPTPUBKEY_1, VALUE_1, PREDESESSOR_1)));
    }
    
    BOOST_AUTO_TEST_CASE(ConclaveOutputOperatorsTest)
    {
        ConclaveOutput conclaveOutput1(SCRIPTPUBKEY_1, VALUE_1);
        ConclaveOutput conclaveOutput2(SCRIPTPUBKEY_1, VALUE_2);
        ConclaveOutput conclaveOutput3(SCRIPTPUBKEY_2, VALUE_1);
        ConclaveOutput conclaveOutput4(SCRIPTPUBKEY_2, VALUE_2);
        ConclaveOutput conclaveOutput5(SCRIPTPUBKEY_1, VALUE_1, PREDESESSOR_1);
        ConclaveOutput conclaveOutput6(SCRIPTPUBKEY_1, VALUE_2, PREDESESSOR_1);
        ConclaveOutput conclaveOutput7(SCRIPTPUBKEY_2, VALUE_1, PREDESESSOR_1);
        ConclaveOutput conclaveOutput8(SCRIPTPUBKEY_2, VALUE_2, PREDESESSOR_1);
        ConclaveOutput conclaveOutput9(SCRIPTPUBKEY_1, VALUE_1);
        ConclaveOutput conclaveOutput10(SCRIPTPUBKEY_1, VALUE_2);
        ConclaveOutput conclaveOutput11(SCRIPTPUBKEY_2, VALUE_1);
        ConclaveOutput conclaveOutput12(SCRIPTPUBKEY_2, VALUE_2);
        ConclaveOutput conclaveOutput13(SCRIPTPUBKEY_1, VALUE_1, PREDESESSOR_1);
        ConclaveOutput conclaveOutput14(SCRIPTPUBKEY_1, VALUE_2, PREDESESSOR_1);
        ConclaveOutput conclaveOutput15(SCRIPTPUBKEY_2, VALUE_1, PREDESESSOR_1);
        ConclaveOutput conclaveOutput16(SCRIPTPUBKEY_2, VALUE_2, PREDESESSOR_1);
        // TODO - assertions
    }
}
