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

#define BOOST_TEST_MODULE Bitcoin_Input_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/bitcoin_input.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    /**
     * Transactions used in this test:
     * https://www.blockchain.com/btc/tx/89d5867cda03494ee1259d1d74f2d42bedae83448a05845c216cb91d15644141 (p2pkh)
     * https://www.blockchain.com/btc/tx/7a1f22f49d1837563bcbc8b680bd1519bd9bc97c6655da853c5deb1777c7ea44 (p2wpkh)
     */
    Outpoint OUTPOINT_1("aa39e8c8f1e03d2036689699c9e1d72da60172b058b43a4e00b29df22bb3dfde", 1);
    Outpoint OUTPOINT_2("885f99fe001cbc8dabddbf730ef5292548be4656d9067109cb5f6cf36df4fcdd", 0);
    Script SCRIPTSIG_1
        ("[3045022100e9115849d3ec1b308c790359fb3a47683aed9182e8f8ef2b440f445d9d9eaad0022"
         "075643177584ed63f1110bb7a9ea63291c1f4c55050c74e94fa0f05b59827bc3b01] "
         "[0347fe3d8bf67687148c43750db49dc5461c460e9ef16a63157329d47cc1178c4e]");
    Script SCRIPTSIG_2;
    uint32_t SEQUENCE_1 = 0xfffffffd;
    uint32_t SEQUENCE_2 = 0xfffffffe;
    std::string BITCOIN_INPUT_STR =
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
    
    pt::ptree makeBitcoinInputTree()
    {
        std::stringstream ss(BITCOIN_INPUT_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinInputConstructorsTest)
    {
        BitcoinInput bitcoinInputFromProps(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1);
        BitcoinInput bitcoinInputFromPtree(makeBitcoinInputTree());
        BOOST_TEST(bitcoinInputFromProps.outpoint == OUTPOINT_1);
        BOOST_TEST(bitcoinInputFromProps.scriptSig == SCRIPTSIG_1);
        BOOST_TEST(bitcoinInputFromProps.sequence == SEQUENCE_1);
        BOOST_TEST(bitcoinInputFromPtree.outpoint == OUTPOINT_1);
        BOOST_TEST(bitcoinInputFromPtree.scriptSig == SCRIPTSIG_1);
        BOOST_TEST(bitcoinInputFromPtree.sequence == SEQUENCE_1);
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinInputCastToPtreeTest)
    {
        BOOST_TEST(makeBitcoinInputTree() == (pt::ptree) BitcoinInput(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1));
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinInputCastToStringTest)
    {
        BOOST_TEST(BITCOIN_INPUT_STR == (std::string) BitcoinInput(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1));
    }
    
    BOOST_AUTO_TEST_CASE(BitcoinInputOperatorsTest)
    {
        BitcoinInput bitcoinInput1(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1);
        BitcoinInput bitcoinInput2(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_2);
        BitcoinInput bitcoinInput3(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_1);
        BitcoinInput bitcoinInput4(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_2);
        BitcoinInput bitcoinInput5(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_1);
        BitcoinInput bitcoinInput6(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_2);
        BitcoinInput bitcoinInput7(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_1);
        BitcoinInput bitcoinInput8(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_2);
        BitcoinInput bitcoinInput9(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_1);
        BitcoinInput bitcoinInput10(OUTPOINT_1, SCRIPTSIG_1, SEQUENCE_2);
        BitcoinInput bitcoinInput11(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_1);
        BitcoinInput bitcoinInput12(OUTPOINT_1, SCRIPTSIG_2, SEQUENCE_2);
        BitcoinInput bitcoinInput13(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_1);
        BitcoinInput bitcoinInput14(OUTPOINT_2, SCRIPTSIG_1, SEQUENCE_2);
        BitcoinInput bitcoinInput15(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_1);
        BitcoinInput bitcoinInput16(OUTPOINT_2, SCRIPTSIG_2, SEQUENCE_2);
        BOOST_TEST((bitcoinInput1 == bitcoinInput9));
        BOOST_TEST((bitcoinInput1 != bitcoinInput10));
        BOOST_TEST((bitcoinInput1 != bitcoinInput11));
        BOOST_TEST((bitcoinInput1 != bitcoinInput12));
        BOOST_TEST((bitcoinInput1 != bitcoinInput13));
        BOOST_TEST((bitcoinInput1 != bitcoinInput14));
        BOOST_TEST((bitcoinInput1 != bitcoinInput15));
        BOOST_TEST((bitcoinInput1 != bitcoinInput16));
        BOOST_TEST((bitcoinInput2 != bitcoinInput9));
        BOOST_TEST((bitcoinInput2 == bitcoinInput10));
        BOOST_TEST((bitcoinInput2 != bitcoinInput11));
        BOOST_TEST((bitcoinInput2 != bitcoinInput12));
        BOOST_TEST((bitcoinInput2 != bitcoinInput13));
        BOOST_TEST((bitcoinInput2 != bitcoinInput14));
        BOOST_TEST((bitcoinInput2 != bitcoinInput15));
        BOOST_TEST((bitcoinInput2 != bitcoinInput16));
        BOOST_TEST((bitcoinInput3 != bitcoinInput9));
        BOOST_TEST((bitcoinInput3 != bitcoinInput10));
        BOOST_TEST((bitcoinInput3 == bitcoinInput11));
        BOOST_TEST((bitcoinInput3 != bitcoinInput12));
        BOOST_TEST((bitcoinInput3 != bitcoinInput13));
        BOOST_TEST((bitcoinInput3 != bitcoinInput14));
        BOOST_TEST((bitcoinInput3 != bitcoinInput15));
        BOOST_TEST((bitcoinInput3 != bitcoinInput16));
        BOOST_TEST((bitcoinInput4 != bitcoinInput9));
        BOOST_TEST((bitcoinInput4 != bitcoinInput10));
        BOOST_TEST((bitcoinInput4 != bitcoinInput11));
        BOOST_TEST((bitcoinInput4 == bitcoinInput12));
        BOOST_TEST((bitcoinInput4 != bitcoinInput13));
        BOOST_TEST((bitcoinInput4 != bitcoinInput14));
        BOOST_TEST((bitcoinInput4 != bitcoinInput15));
        BOOST_TEST((bitcoinInput4 != bitcoinInput16));
        BOOST_TEST((bitcoinInput5 != bitcoinInput9));
        BOOST_TEST((bitcoinInput5 != bitcoinInput10));
        BOOST_TEST((bitcoinInput5 != bitcoinInput11));
        BOOST_TEST((bitcoinInput5 != bitcoinInput12));
        BOOST_TEST((bitcoinInput5 == bitcoinInput13));
        BOOST_TEST((bitcoinInput5 != bitcoinInput14));
        BOOST_TEST((bitcoinInput5 != bitcoinInput15));
        BOOST_TEST((bitcoinInput5 != bitcoinInput16));
        BOOST_TEST((bitcoinInput6 != bitcoinInput9));
        BOOST_TEST((bitcoinInput6 != bitcoinInput10));
        BOOST_TEST((bitcoinInput6 != bitcoinInput11));
        BOOST_TEST((bitcoinInput6 != bitcoinInput12));
        BOOST_TEST((bitcoinInput6 != bitcoinInput13));
        BOOST_TEST((bitcoinInput6 == bitcoinInput14));
        BOOST_TEST((bitcoinInput6 != bitcoinInput15));
        BOOST_TEST((bitcoinInput6 != bitcoinInput16));
        BOOST_TEST((bitcoinInput7 != bitcoinInput9));
        BOOST_TEST((bitcoinInput7 != bitcoinInput10));
        BOOST_TEST((bitcoinInput7 != bitcoinInput11));
        BOOST_TEST((bitcoinInput7 != bitcoinInput12));
        BOOST_TEST((bitcoinInput7 != bitcoinInput13));
        BOOST_TEST((bitcoinInput7 != bitcoinInput14));
        BOOST_TEST((bitcoinInput7 == bitcoinInput15));
        BOOST_TEST((bitcoinInput7 != bitcoinInput16));
        BOOST_TEST((bitcoinInput8 != bitcoinInput9));
        BOOST_TEST((bitcoinInput8 != bitcoinInput10));
        BOOST_TEST((bitcoinInput8 != bitcoinInput11));
        BOOST_TEST((bitcoinInput8 != bitcoinInput12));
        BOOST_TEST((bitcoinInput8 != bitcoinInput13));
        BOOST_TEST((bitcoinInput8 != bitcoinInput14));
        BOOST_TEST((bitcoinInput8 != bitcoinInput15));
        BOOST_TEST((bitcoinInput8 == bitcoinInput16));
    }
}
