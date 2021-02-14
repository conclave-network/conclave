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

#define BOOST_TEST_MODULE Conclave_Input_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/conclave_input.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    const static Outpoint OUTPOINT_1("aa39e8c8f1e03d2036689699c9e1d72da60172b058b43a4e00b29df22bb3dfde", 1);
    const static Outpoint OUTPOINT_2("885f99fe001cbc8dabddbf730ef5292548be4656d9067109cb5f6cf36df4fcdd", 0);
    const static Script SCRIPTSIG_1
        ("[3045022100e9115849d3ec1b308c790359fb3a47683aed9182e8f8ef2b440f445d9d9eaad0022"
         "075643177584ed63f1110bb7a9ea63291c1f4c55050c74e94fa0f05b59827bc3b01] "
         "[0347fe3d8bf67687148c43750db49dc5461c460e9ef16a63157329d47cc1178c4e]");
    const static Script SCRIPTSIG_2;
    const static uint32_t SEQUENCE_1 = 0xfffffffd;
    const static uint32_t SEQUENCE_2 = 0xfffffffe;
    const static Inpoint PREDECESSOR_1("f214030b76c722a3564887ef667c33424b64b2a1ab8c5f72d2fc5ff284b4e3d4", 31);
    const static std::string CONCLAVE_INPUT_STR =
        "{\n"
        "    \"outpoint\": {\n"
        "        \"txId\": \"aa39e8c8f1e03d2036689699c9e1d72da60172b058b43a4e00b29df22bb3dfde\",\n"
        "        \"index\": \"1\"\n"
        "    },\n"
        "    \"scriptSig\": [\n"
        "        \"[3045022100e9115849d3ec1b308c790359fb3a47683aed9182e8f8ef2b440f445d9d9eaad0022075"
        "643177584ed63f1110bb7a9ea63291c1f4c55050c74e94fa0f05b59827bc3b01]\",\n"
        "        \"[0347fe3d8bf67687148c43750db49dc5461c460e9ef16a63157329d47cc1178c4e]\"\n"
        "    ],\n"
        "    \"sequence\": \"4294967293\"\n"
        "}\n";
    const static std::string CONCLAVE_INPUT_STR_WITH_PREDECESSOR =
        "{\n"
        "    \"outpoint\": {\n"
        "        \"txId\": \"aa39e8c8f1e03d2036689699c9e1d72da60172b058b43a4e00b29df22bb3dfde\",\n"
        "        \"index\": \"1\"\n"
        "    },\n"
        "    \"scriptSig\": [\n"
        "        \"[3045022100e9115849d3ec1b308c790359fb3a47683aed9182e8f8ef2b440f445d9d9eaad0022075"
        "643177584ed63f1110bb7a9ea63291c1f4c55050c74e94fa0f05b59827bc3b01]\",\n"
        "        \"[0347fe3d8bf67687148c43750db49dc5461c460e9ef16a63157329d47cc1178c4e]\"\n"
        "    ],\n"
        "    \"sequence\": \"4294967293\",\n"
        "    \"predecessor\": {\n"
        "        \"txId\": \"f214030b76c722a3564887ef667c33424b64b2a1ab8c5f72d2fc5ff284b4e3d4\",\n"
        "        \"index\": \"31\"\n"
        "    }\n"
        "}\n";
    
    const static pt::ptree makeConclaveInputTree()
    {
        std::stringstream ss(CONCLAVE_INPUT_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    const static pt::ptree makeConclaveInputTreeWithPredecessor()
    {
        std::stringstream ss(CONCLAVE_INPUT_STR_WITH_PREDECESSOR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_SUITE(ConclaveInputTestSuite)
        
        BOOST_AUTO_TEST_CASE(ConclaveInputConstructorsTest)
        {
            ConclaveInput conclaveInputFromProps(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1);
            ConclaveInput conclaveInputFromPropsWithPredecessor(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1, PREDECESSOR_1);
            ConclaveInput conclaveInputFromPtree(makeConclaveInputTree());
            ConclaveInput conclaveInputFromPtreeWithPredecessor(makeConclaveInputTreeWithPredecessor());
            BOOST_TEST((conclaveInputFromProps.outpoint == OUTPOINT_1));
            BOOST_TEST((conclaveInputFromProps.scriptSig == SCRIPTSIG_1));
            BOOST_TEST((conclaveInputFromProps.sequence == SEQUENCE_1));
            BOOST_TEST((conclaveInputFromProps.predecessor == std::nullopt));
            BOOST_TEST((conclaveInputFromPropsWithPredecessor.outpoint == OUTPOINT_1));
            BOOST_TEST((conclaveInputFromPropsWithPredecessor.scriptSig == SCRIPTSIG_1));
            BOOST_TEST((conclaveInputFromPropsWithPredecessor.sequence == SEQUENCE_1));
            BOOST_TEST((conclaveInputFromPropsWithPredecessor.predecessor == PREDECESSOR_1));
            BOOST_TEST((conclaveInputFromPtree.outpoint == OUTPOINT_1));
            BOOST_TEST((conclaveInputFromPtree.scriptSig == SCRIPTSIG_1));
            BOOST_TEST((conclaveInputFromPtree.sequence == SEQUENCE_1));
            BOOST_TEST((conclaveInputFromPtree.predecessor == std::nullopt));
            BOOST_TEST((conclaveInputFromPtreeWithPredecessor.outpoint == OUTPOINT_1));
            BOOST_TEST((conclaveInputFromPtreeWithPredecessor.scriptSig == SCRIPTSIG_1));
            BOOST_TEST((conclaveInputFromPtreeWithPredecessor.sequence == SEQUENCE_1));
            BOOST_TEST((conclaveInputFromPtreeWithPredecessor.predecessor == PREDECESSOR_1));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveInputCastToPtreeTest)
        {
            BOOST_TEST((makeConclaveInputTree() ==
                        static_cast<pt::ptree>(ConclaveInput(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1))));
            BOOST_TEST((makeConclaveInputTreeWithPredecessor() ==
                        static_cast<pt::ptree>(ConclaveInput(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1, PREDECESSOR_1))));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveInputCastToStringTest)
        {
            BOOST_TEST((CONCLAVE_INPUT_STR ==
                        static_cast<std::string>(ConclaveInput(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1))));
            BOOST_TEST((CONCLAVE_INPUT_STR_WITH_PREDECESSOR ==
                        static_cast<std::string>(ConclaveInput(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1, PREDECESSOR_1))));
        }
        
        BOOST_AUTO_TEST_CASE(ConclaveInputOperatorsTest)
        {
            ConclaveInput conclaveInput1(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1);
            ConclaveInput conclaveInput2(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_2);
            ConclaveInput conclaveInput3(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_1);
            ConclaveInput conclaveInput4(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_2);
            ConclaveInput conclaveInput5(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_1);
            ConclaveInput conclaveInput6(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_2);
            ConclaveInput conclaveInput7(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_1);
            ConclaveInput conclaveInput8(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_2);
            ConclaveInput conclaveInput9(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1);
            ConclaveInput conclaveInput10(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_2);
            ConclaveInput conclaveInput11(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_1);
            ConclaveInput conclaveInput12(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_2);
            ConclaveInput conclaveInput13(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_1);
            ConclaveInput conclaveInput14(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_2);
            ConclaveInput conclaveInput15(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_1);
            ConclaveInput conclaveInput16(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_2);
            BOOST_TEST((conclaveInput1 == conclaveInput9));
            BOOST_TEST((conclaveInput1 != conclaveInput10));
            BOOST_TEST((conclaveInput1 != conclaveInput11));
            BOOST_TEST((conclaveInput1 != conclaveInput12));
            BOOST_TEST((conclaveInput1 != conclaveInput13));
            BOOST_TEST((conclaveInput1 != conclaveInput14));
            BOOST_TEST((conclaveInput1 != conclaveInput15));
            BOOST_TEST((conclaveInput1 != conclaveInput16));
            BOOST_TEST((conclaveInput2 != conclaveInput9));
            BOOST_TEST((conclaveInput2 == conclaveInput10));
            BOOST_TEST((conclaveInput2 != conclaveInput11));
            BOOST_TEST((conclaveInput2 != conclaveInput12));
            BOOST_TEST((conclaveInput2 != conclaveInput13));
            BOOST_TEST((conclaveInput2 != conclaveInput14));
            BOOST_TEST((conclaveInput2 != conclaveInput15));
            BOOST_TEST((conclaveInput2 != conclaveInput16));
            BOOST_TEST((conclaveInput3 != conclaveInput9));
            BOOST_TEST((conclaveInput3 != conclaveInput10));
            BOOST_TEST((conclaveInput3 == conclaveInput11));
            BOOST_TEST((conclaveInput3 != conclaveInput12));
            BOOST_TEST((conclaveInput3 != conclaveInput13));
            BOOST_TEST((conclaveInput3 != conclaveInput14));
            BOOST_TEST((conclaveInput3 != conclaveInput15));
            BOOST_TEST((conclaveInput3 != conclaveInput16));
            BOOST_TEST((conclaveInput4 != conclaveInput9));
            BOOST_TEST((conclaveInput4 != conclaveInput10));
            BOOST_TEST((conclaveInput4 != conclaveInput11));
            BOOST_TEST((conclaveInput4 == conclaveInput12));
            BOOST_TEST((conclaveInput4 != conclaveInput13));
            BOOST_TEST((conclaveInput4 != conclaveInput14));
            BOOST_TEST((conclaveInput4 != conclaveInput15));
            BOOST_TEST((conclaveInput4 != conclaveInput16));
            BOOST_TEST((conclaveInput5 != conclaveInput9));
            BOOST_TEST((conclaveInput5 != conclaveInput10));
            BOOST_TEST((conclaveInput5 != conclaveInput11));
            BOOST_TEST((conclaveInput5 != conclaveInput12));
            BOOST_TEST((conclaveInput5 == conclaveInput13));
            BOOST_TEST((conclaveInput5 != conclaveInput14));
            BOOST_TEST((conclaveInput5 != conclaveInput15));
            BOOST_TEST((conclaveInput5 != conclaveInput16));
            BOOST_TEST((conclaveInput6 != conclaveInput9));
            BOOST_TEST((conclaveInput6 != conclaveInput10));
            BOOST_TEST((conclaveInput6 != conclaveInput11));
            BOOST_TEST((conclaveInput6 != conclaveInput12));
            BOOST_TEST((conclaveInput6 != conclaveInput13));
            BOOST_TEST((conclaveInput6 == conclaveInput14));
            BOOST_TEST((conclaveInput6 != conclaveInput15));
            BOOST_TEST((conclaveInput6 != conclaveInput16));
            BOOST_TEST((conclaveInput7 != conclaveInput9));
            BOOST_TEST((conclaveInput7 != conclaveInput10));
            BOOST_TEST((conclaveInput7 != conclaveInput11));
            BOOST_TEST((conclaveInput7 != conclaveInput12));
            BOOST_TEST((conclaveInput7 != conclaveInput13));
            BOOST_TEST((conclaveInput7 != conclaveInput14));
            BOOST_TEST((conclaveInput7 == conclaveInput15));
            BOOST_TEST((conclaveInput7 != conclaveInput16));
            BOOST_TEST((conclaveInput8 != conclaveInput9));
            BOOST_TEST((conclaveInput8 != conclaveInput10));
            BOOST_TEST((conclaveInput8 != conclaveInput11));
            BOOST_TEST((conclaveInput8 != conclaveInput12));
            BOOST_TEST((conclaveInput8 != conclaveInput13));
            BOOST_TEST((conclaveInput8 != conclaveInput14));
            BOOST_TEST((conclaveInput8 != conclaveInput15));
            BOOST_TEST((conclaveInput8 == conclaveInput16));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
