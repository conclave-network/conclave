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

#define BOOST_TEST_MODULE Bitcoin_Block_Header_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../src/chain/structs/bitcoin_block_header.h"
#include <boost/property_tree/json_parser.hpp>
#include <cstdint>
#include <vector>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        const static uint32_t VERSION_1 = 1;
        const static uint32_t VERSION_2 = 1073733632;
        const static Hash256 HASH_PREV_BLOCK_1("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
        const static Hash256 HASH_PREV_BLOCK_2("0000000000000000000c18fe1f9f3772ea3b739f9c6276d70ae37704794b2e6b");
        const static Hash256 HASH_MERKLE_ROOT_1("0e3e2357e806b6cdb1f70b54c3a3a17b6714ee1f0e68bebb44a74b1efd512098");
        const static Hash256 HASH_MERKLE_ROOT_2("e30a6adf3249ded6a9e4c8c79340c1955339cba8ff0ff659ba1cf9de59438704");
        const static uint32_t TIME_1 = 1231469665;
        const static uint32_t TIME_2 = 1584811764;
        const static uint32_t BITS_1 = 0x1d00ffff;
        const static uint32_t BITS_2 = 0x17110119;
        const static uint32_t NONCE_1 = 2573394689;
        const static uint32_t NONCE_2 = 1436428351;
        const static std::string BITCOIN_BLOCK_HEADER_1_STR =
            "{\n"
            "    \"version\": \"1\",\n"
            "    \"hashPrevBlock\": \"000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f\",\n"
            "    \"hashMerkleRoot\": \"0e3e2357e806b6cdb1f70b54c3a3a17b6714ee1f0e68bebb44a74b1efd512098\",\n"
            "    \"time\": \"1231469665\",\n"
            "    \"bits\": \"486604799\",\n"
            "    \"nonce\": \"2573394689\"\n"
            "}\n";
        const static Hash256 BLOCK_1_HASH("00000000839a8e6886ab5951d76f411475428afc90947ee320161bbf18eb6048");
        const static Hash256 BLOCK_2_HASH("0000000000000000000859830058f91753af64468073d16d47fe4fce57d3f04e");
        const static std::vector<BYTE> BLOCK_1_SERIALIZED = HEX_TO_BYTE_VECTOR(
            "010000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000982051fd1e4ba744b"
            "bbe680e1fee14677ba1a3c3540bf7b1cdb606e857233e0e61bc6649ffff001d01e36299");
        const static std::vector<BYTE> BLOCK_2_SERIALIZED = HEX_TO_BYTE_VECTOR(
            "00e0ff3f6b2e4b790477e30ad776629c9f733bea72379f1ffe180c00000000000000000004874359def91cba5"
            "9f60fffa8cb395395c14093c7c8e4a9d6de4932df6a0ae3f44e765e190111173f289e55");
        
        pt::ptree makeBitcoinBlockHeaderTree()
        {
            std::stringstream ss(BITCOIN_BLOCK_HEADER_1_STR);
            pt::ptree tree;
            pt::read_json(ss, tree);
            return tree;
        }
        
        BOOST_AUTO_TEST_SUITE(BitcoinBlockHeaderTestSuite)
            
            BOOST_AUTO_TEST_CASE(BitcoinBlockHeaderConstructorsTest)
            {
                BitcoinBlockHeader bitcoinBlockHeaderFromProps(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1,
                                                               TIME_1, BITS_1, NONCE_1);
                BitcoinBlockHeader bitcoinBlockHeaderFromPtree(makeBitcoinBlockHeaderTree());
                BOOST_TEST((bitcoinBlockHeaderFromProps.version == VERSION_1));
                BOOST_TEST((bitcoinBlockHeaderFromProps.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((bitcoinBlockHeaderFromProps.hashMerkleRoot == HASH_MERKLE_ROOT_1));
                BOOST_TEST((bitcoinBlockHeaderFromProps.time == TIME_1));
                BOOST_TEST((bitcoinBlockHeaderFromProps.bits == BITS_1));
                BOOST_TEST((bitcoinBlockHeaderFromProps.nonce == NONCE_1));
                BOOST_TEST((bitcoinBlockHeaderFromPtree.version == VERSION_1));
                BOOST_TEST((bitcoinBlockHeaderFromPtree.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((bitcoinBlockHeaderFromPtree.hashMerkleRoot == HASH_MERKLE_ROOT_1));
                BOOST_TEST((bitcoinBlockHeaderFromPtree.time == TIME_1));
                BOOST_TEST((bitcoinBlockHeaderFromPtree.bits == BITS_1));
                BOOST_TEST((bitcoinBlockHeaderFromPtree.nonce == NONCE_1));
            }
            
            BOOST_AUTO_TEST_CASE(BitcoinBlockHeaderCopyAndMoveConstructorTest)
            {
                BitcoinBlockHeader bitcoinBlockHeaderFromProps(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1,
                                                               TIME_1, BITS_1, NONCE_1);
                BitcoinBlockHeader bitcoinBlockHeaderCopy(bitcoinBlockHeaderFromProps);
                BitcoinBlockHeader bitcoinBlockHeaderMove(std::move(
                    BitcoinBlockHeader(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1, TIME_1, BITS_1, NONCE_1)
                ));
                BOOST_TEST((bitcoinBlockHeaderCopy.version == VERSION_1));
                BOOST_TEST((bitcoinBlockHeaderCopy.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((bitcoinBlockHeaderCopy.hashMerkleRoot == HASH_MERKLE_ROOT_1));
                BOOST_TEST((bitcoinBlockHeaderCopy.time == TIME_1));
                BOOST_TEST((bitcoinBlockHeaderCopy.bits == BITS_1));
                BOOST_TEST((bitcoinBlockHeaderCopy.nonce == NONCE_1));
                BOOST_TEST((bitcoinBlockHeaderMove.version == VERSION_1));
                BOOST_TEST((bitcoinBlockHeaderMove.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((bitcoinBlockHeaderMove.hashMerkleRoot == HASH_MERKLE_ROOT_1));
                BOOST_TEST((bitcoinBlockHeaderMove.time == TIME_1));
                BOOST_TEST((bitcoinBlockHeaderMove.bits == BITS_1));
                BOOST_TEST((bitcoinBlockHeaderMove.nonce == NONCE_1));
            }
            
            BOOST_AUTO_TEST_CASE(BitcoinBlockHeaderHash256Test)
            {
                BitcoinBlockHeader bitcoinBlockHeader1(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1,
                                                       TIME_1, BITS_1, NONCE_1);
                BitcoinBlockHeader bitcoinBlockHeader2(VERSION_2, HASH_PREV_BLOCK_2, HASH_MERKLE_ROOT_2,
                                                       TIME_2, BITS_2, NONCE_2);
                BOOST_TEST((bitcoinBlockHeader1.getHash256() == BLOCK_1_HASH));
                BOOST_TEST((bitcoinBlockHeader2.getHash256() == BLOCK_2_HASH));
            }
            
            BOOST_AUTO_TEST_CASE(BitcoinBlockHeaderSerializeTest)
            {
                BitcoinBlockHeader bitcoinBlockHeader1(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1,
                                                       TIME_1, BITS_1, NONCE_1);
                BitcoinBlockHeader bitcoinBlockHeader2(VERSION_2, HASH_PREV_BLOCK_2, HASH_MERKLE_ROOT_2,
                                                       TIME_2, BITS_2, NONCE_2);
                BOOST_TEST((bitcoinBlockHeader1.serialize() == BLOCK_1_SERIALIZED));
                BOOST_TEST((bitcoinBlockHeader2.serialize() == BLOCK_2_SERIALIZED));
            }
            
            BOOST_AUTO_TEST_CASE(BitcoinBlockHeaderCastToPtreeTest)
            {
                BitcoinBlockHeader bitcoinBlockHeader(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1,
                                                      TIME_1, BITS_1, NONCE_1);
                BOOST_TEST((makeBitcoinBlockHeaderTree() == static_cast<pt::ptree>(bitcoinBlockHeader)));
            }
            
            BOOST_AUTO_TEST_CASE(BitcoinBlockHeaderCastToStringTest)
            {
                BitcoinBlockHeader bitcoinBlockHeader(VERSION_1, HASH_PREV_BLOCK_1, HASH_MERKLE_ROOT_1,
                                                      TIME_1, BITS_1, NONCE_1);
                BOOST_TEST((BITCOIN_BLOCK_HEADER_1_STR == static_cast<std::string>(bitcoinBlockHeader)));
            }
        
        BOOST_AUTO_TEST_SUITE_END()
    }
}
