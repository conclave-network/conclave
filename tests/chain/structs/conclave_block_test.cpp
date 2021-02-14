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

#define BOOST_TEST_MODULE Conclave_Block_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../src/chain/structs/conclave_block.h"
#include "../../../src/util/json.h"
#include <cstdint>
#include <vector>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        const static uint64_t POT_1 = 43543543545235;
        const static uint64_t POT_2 = 90234782359871;
        const static uint64_t HEIGHT_1 = 92305981724234;
        const static uint64_t HEIGHT_2 = 21489320927812;
        const static uint32_t EPOCH_1 = 453939008;
        const static uint32_t EPOCH_2 = 129082432;
        const static Hash256 HASH_PREV_BLOCK_1("5885d77d868796a32167d0f62b91a384480681fab54f42ad94db5d9da53563f6");
        const static Hash256 HASH_PREV_BLOCK_2("287a13f631b93dc344715e2e20498f11fa27a8f47ed7195cb19480c0c9b391db");
        const static Hash256 LOWEST_PARENT_BITCOIN_BLOCK_HASH_1
            ("685da7ae1ddb7cae0985638b23f1e557336880177632baad474304e11caff60b");
        const static Hash256 LOWEST_PARENT_BITCOIN_BLOCK_HASH_2
            ("05eb56b629d720308cc608767d4978d4358c821aac19a510871979eadfe1abc9");
        const static uint16_t TX_TYPE_ID_1 = 12418;
        const static uint16_t TX_TYPE_ID_2 = 46438;
        const static uint16_t TX_VERSION_1 = 10924;
        const static uint16_t TX_VERSION_2 = 61093;
        const static Hash256 TX_HASH_1("7b1a23b60a934a3775b5f451693c6c8c8b676569f4ab59566ad4b35d4a2afc6a");
        const static Hash256 TX_HASH_2("97c19b69e4aef026530beca4cbee4e2ef5225a57ad8f66b5c04922098fdffd78");
        const static std::string CONCLAVE_BLOCK_1_STR =
            "{\n"
            "    \"pot\": \"43543543545235\",\n"
            "    \"height\": \"92305981724234\",\n"
            "    \"epoch\": \"453939008\",\n"
            "    \"hashPrevBlock\": \"5885d77d868796a32167d0f62b91a384480681fab54f42ad94db5d9da53563f6\",\n"
            "    \"lowestParentBitcoinBlockHash\": \"685da7ae1ddb7cae0985638b23f1e557336880177632baad474304e11caff60b\",\n"
            "    \"txTypeId\": \"12418\",\n"
            "    \"txVersion\": \"10924\",\n"
            "    \"txHash\": \"7b1a23b60a934a3775b5f451693c6c8c8b676569f4ab59566ad4b35d4a2afc6a\"\n"
            "}\n";
        const static pt::ptree CONCLAVE_BLOCK_1_PTREE = stringToPtree(CONCLAVE_BLOCK_1_STR);
        const static Hash256 CONCLAVE_BLOCK_1_HASH("92b1f32a27f16833c657966aa799296392e7f4076466f6ec69bba87c4afb74bc");
        const static Hash256 CONCLAVE_BLOCK_2_HASH("7918d34963a93b792486a68a15a7d74819f70c1eeb7c20414ce106e60dad75e5");
        const static std::vector<BYTE> CONCLAVE_BLOCK_1_SERIALIZED = HEX_TO_BYTE_VECTOR(
            "93fd71459a2700004a5640a9f3530000408f0e1bf66335a59d5ddb94ad424fb5fa81064884a3912bf6d06721a39687867dd"
            "785580bf6af1ce1044347adba32761780683357e5f1238b638509ae7cdb1daea75d688230ac2a6afc2a4a5db3d46a5659abf4696"
            "5678b8c6c3c6951f4b575374a930ab6231a7b"
        );
        const static std::vector<BYTE> CONCLAVE_BLOCK_2_SERIALIZED = HEX_TO_BYTE_VECTOR(
            "3f21266c115200004422575f8b13000040a4b107db91b3c9c08094b15c19d77ef4a827fa118f49202e5e7144c33db931f61"
            "37a28c9abe1dfea79198710a519ac1a828c35d478497d7608c68c3020d729b656eb0566b5a5ee78fddf8f092249c0b5668fad575"
            "a22f52e4eeecba4ec0b5326f0aee4699bc197");
        BOOST_AUTO_TEST_SUITE(ConclaveBlockTestSuite)
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockDeserializeFactoryTest)
            {
                size_t pos = 0;
                ConclaveBlock conclaveBlock1 = ConclaveBlock::deserialize(CONCLAVE_BLOCK_1_SERIALIZED, pos);
                ConclaveBlock conclaveBlock2 = ConclaveBlock::deserialize(CONCLAVE_BLOCK_2_SERIALIZED);
                BOOST_TEST((conclaveBlock1.pot == POT_1));
                BOOST_TEST((conclaveBlock1.height == HEIGHT_1));
                BOOST_TEST((conclaveBlock1.epoch == EPOCH_1));
                BOOST_TEST((conclaveBlock1.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((conclaveBlock1.lowestParentBitcoinBlockHash == LOWEST_PARENT_BITCOIN_BLOCK_HASH_1));
                BOOST_TEST((conclaveBlock1.txTypeId == TX_TYPE_ID_1));
                BOOST_TEST((conclaveBlock1.txVersion == TX_VERSION_1));
                BOOST_TEST((conclaveBlock1.txHash == TX_HASH_1));
                BOOST_TEST((conclaveBlock2.pot == POT_2));
                BOOST_TEST((conclaveBlock2.height == HEIGHT_2));
                BOOST_TEST((conclaveBlock2.epoch == EPOCH_2));
                BOOST_TEST((conclaveBlock2.hashPrevBlock == HASH_PREV_BLOCK_2));
                BOOST_TEST((conclaveBlock2.lowestParentBitcoinBlockHash == LOWEST_PARENT_BITCOIN_BLOCK_HASH_2));
                BOOST_TEST((conclaveBlock2.txTypeId == TX_TYPE_ID_2));
                BOOST_TEST((conclaveBlock2.txVersion == TX_VERSION_2));
                BOOST_TEST((conclaveBlock2.txHash == TX_HASH_2));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockConstructorsTest)
            {
                ConclaveBlock conclaveBlockFromProps(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                                     LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                                     TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlockFromPtree(CONCLAVE_BLOCK_1_PTREE);
                ConclaveBlock conclaveBlockFromByteVector(CONCLAVE_BLOCK_1_SERIALIZED);
                BOOST_TEST((conclaveBlockFromProps.pot == POT_1));
                BOOST_TEST((conclaveBlockFromProps.height == HEIGHT_1));
                BOOST_TEST((conclaveBlockFromProps.epoch == EPOCH_1));
                BOOST_TEST((conclaveBlockFromProps.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((conclaveBlockFromProps.lowestParentBitcoinBlockHash == LOWEST_PARENT_BITCOIN_BLOCK_HASH_1));
                BOOST_TEST((conclaveBlockFromProps.txTypeId == TX_TYPE_ID_1));
                BOOST_TEST((conclaveBlockFromProps.txVersion == TX_VERSION_1));
                BOOST_TEST((conclaveBlockFromProps.txHash == TX_HASH_1));
                BOOST_TEST((conclaveBlockFromPtree.pot == POT_1));
                BOOST_TEST((conclaveBlockFromPtree.height == HEIGHT_1));
                BOOST_TEST((conclaveBlockFromPtree.epoch == EPOCH_1));
                BOOST_TEST((conclaveBlockFromPtree.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((conclaveBlockFromPtree.lowestParentBitcoinBlockHash == LOWEST_PARENT_BITCOIN_BLOCK_HASH_1));
                BOOST_TEST((conclaveBlockFromPtree.txTypeId == TX_TYPE_ID_1));
                BOOST_TEST((conclaveBlockFromPtree.txVersion == TX_VERSION_1));
                BOOST_TEST((conclaveBlockFromPtree.txHash == TX_HASH_1));
                BOOST_TEST((conclaveBlockFromByteVector.pot == POT_1));
                BOOST_TEST((conclaveBlockFromByteVector.height == HEIGHT_1));
                BOOST_TEST((conclaveBlockFromByteVector.epoch == EPOCH_1));
                BOOST_TEST((conclaveBlockFromByteVector.hashPrevBlock == HASH_PREV_BLOCK_1));
                BOOST_TEST((conclaveBlockFromByteVector.lowestParentBitcoinBlockHash ==
                            LOWEST_PARENT_BITCOIN_BLOCK_HASH_1));
                BOOST_TEST((conclaveBlockFromByteVector.txTypeId == TX_TYPE_ID_1));
                BOOST_TEST((conclaveBlockFromByteVector.txVersion == TX_VERSION_1));
                BOOST_TEST((conclaveBlockFromByteVector.txHash == TX_HASH_1));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockCopyAndMoveConstructorTest)
            {
                ConclaveBlock conclaveBlockFromProps(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                                     LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                                     TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlockCopy(conclaveBlockFromProps);
                ConclaveBlock conclaveBlockMove(std::move(
                    ConclaveBlock(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1, LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                  TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1)
                ));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockHash256Test)
            {
                ConclaveBlock conclaveBlock(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                            LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                            TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                BOOST_TEST((conclaveBlock.getHash256() == CONCLAVE_BLOCK_1_HASH));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockSerializeTest)
            {
                ConclaveBlock conclaveBlock(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                            LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                            TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                BOOST_TEST((conclaveBlock.serialize() == CONCLAVE_BLOCK_1_SERIALIZED));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockCastToPtreeTest)
            {
                ConclaveBlock conclaveBlock(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                            LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                            TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                BOOST_TEST((CONCLAVE_BLOCK_1_PTREE == static_cast<pt::ptree>(conclaveBlock)));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockCastToStringTest)
            {
                ConclaveBlock conclaveBlock(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                            LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                            TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                BOOST_TEST((CONCLAVE_BLOCK_1_STR == static_cast<std::string>(conclaveBlock)));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockAssignmentOperatorsTest)
            {
                const ConclaveBlock conclaveBlock1(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                                   LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                                   TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock2(POT_2, HEIGHT_2, EPOCH_2, HASH_PREV_BLOCK_2,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_2,
                                             TX_TYPE_ID_2, TX_VERSION_2, TX_HASH_2);
                ConclaveBlock conclaveBlock3(POT_2, HEIGHT_2, EPOCH_2, HASH_PREV_BLOCK_2,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_2,
                                             TX_TYPE_ID_2, TX_VERSION_2, TX_HASH_2);
                conclaveBlock2 = conclaveBlock1;
                conclaveBlock3 = std::move(ConclaveBlock(conclaveBlock1));
                BOOST_TEST((conclaveBlock1 == conclaveBlock2));
                BOOST_TEST((conclaveBlock2 == conclaveBlock3));
                BOOST_TEST((conclaveBlock3 == conclaveBlock1));
            }
            
            BOOST_AUTO_TEST_CASE(ConclaveBlockComparisonOperatorsTest)
            {
                ConclaveBlock conclaveBlock1(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock2(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock3(POT_2, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock4(POT_1, HEIGHT_2, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock5(POT_1, HEIGHT_1, EPOCH_2, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock6(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_2,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock7(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_2,
                                             TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock8(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_2, TX_VERSION_1, TX_HASH_1);
                ConclaveBlock conclaveBlock9(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                             LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                             TX_TYPE_ID_1, TX_VERSION_2, TX_HASH_1);
                ConclaveBlock conclaveBlock10(POT_1, HEIGHT_1, EPOCH_1, HASH_PREV_BLOCK_1,
                                              LOWEST_PARENT_BITCOIN_BLOCK_HASH_1,
                                              TX_TYPE_ID_1, TX_VERSION_1, TX_HASH_2);
                BOOST_TEST((conclaveBlock1 == conclaveBlock1));
                BOOST_TEST((conclaveBlock1 == conclaveBlock2));
                BOOST_TEST((conclaveBlock1 != conclaveBlock3));
                BOOST_TEST((conclaveBlock1 != conclaveBlock4));
                BOOST_TEST((conclaveBlock1 != conclaveBlock5));
                BOOST_TEST((conclaveBlock1 != conclaveBlock6));
                BOOST_TEST((conclaveBlock1 != conclaveBlock7));
                BOOST_TEST((conclaveBlock1 != conclaveBlock8));
                BOOST_TEST((conclaveBlock1 != conclaveBlock9));
                BOOST_TEST((conclaveBlock1 != conclaveBlock10));
                BOOST_TEST((conclaveBlock2 == conclaveBlock1));
                BOOST_TEST((conclaveBlock2 == conclaveBlock2));
                BOOST_TEST((conclaveBlock2 != conclaveBlock3));
                BOOST_TEST((conclaveBlock2 != conclaveBlock4));
                BOOST_TEST((conclaveBlock2 != conclaveBlock5));
                BOOST_TEST((conclaveBlock2 != conclaveBlock6));
                BOOST_TEST((conclaveBlock2 != conclaveBlock7));
                BOOST_TEST((conclaveBlock2 != conclaveBlock8));
                BOOST_TEST((conclaveBlock2 != conclaveBlock9));
                BOOST_TEST((conclaveBlock2 != conclaveBlock10));
                BOOST_TEST((conclaveBlock3 != conclaveBlock1));
                BOOST_TEST((conclaveBlock3 != conclaveBlock2));
                BOOST_TEST((conclaveBlock3 == conclaveBlock3));
                BOOST_TEST((conclaveBlock3 != conclaveBlock4));
                BOOST_TEST((conclaveBlock3 != conclaveBlock5));
                BOOST_TEST((conclaveBlock3 != conclaveBlock6));
                BOOST_TEST((conclaveBlock3 != conclaveBlock7));
                BOOST_TEST((conclaveBlock3 != conclaveBlock8));
                BOOST_TEST((conclaveBlock3 != conclaveBlock9));
                BOOST_TEST((conclaveBlock3 != conclaveBlock10));
                BOOST_TEST((conclaveBlock4 != conclaveBlock1));
                BOOST_TEST((conclaveBlock4 != conclaveBlock2));
                BOOST_TEST((conclaveBlock4 != conclaveBlock3));
                BOOST_TEST((conclaveBlock4 == conclaveBlock4));
                BOOST_TEST((conclaveBlock4 != conclaveBlock5));
                BOOST_TEST((conclaveBlock4 != conclaveBlock6));
                BOOST_TEST((conclaveBlock4 != conclaveBlock7));
                BOOST_TEST((conclaveBlock4 != conclaveBlock8));
                BOOST_TEST((conclaveBlock4 != conclaveBlock9));
                BOOST_TEST((conclaveBlock4 != conclaveBlock10));
                BOOST_TEST((conclaveBlock5 != conclaveBlock1));
                BOOST_TEST((conclaveBlock5 != conclaveBlock2));
                BOOST_TEST((conclaveBlock5 != conclaveBlock3));
                BOOST_TEST((conclaveBlock5 != conclaveBlock4));
                BOOST_TEST((conclaveBlock5 == conclaveBlock5));
                BOOST_TEST((conclaveBlock5 != conclaveBlock6));
                BOOST_TEST((conclaveBlock5 != conclaveBlock7));
                BOOST_TEST((conclaveBlock5 != conclaveBlock8));
                BOOST_TEST((conclaveBlock5 != conclaveBlock9));
                BOOST_TEST((conclaveBlock5 != conclaveBlock10));
                BOOST_TEST((conclaveBlock6 != conclaveBlock1));
                BOOST_TEST((conclaveBlock6 != conclaveBlock2));
                BOOST_TEST((conclaveBlock6 != conclaveBlock3));
                BOOST_TEST((conclaveBlock6 != conclaveBlock4));
                BOOST_TEST((conclaveBlock6 != conclaveBlock5));
                BOOST_TEST((conclaveBlock6 == conclaveBlock6));
                BOOST_TEST((conclaveBlock6 != conclaveBlock7));
                BOOST_TEST((conclaveBlock6 != conclaveBlock8));
                BOOST_TEST((conclaveBlock6 != conclaveBlock9));
                BOOST_TEST((conclaveBlock6 != conclaveBlock10));
                BOOST_TEST((conclaveBlock7 != conclaveBlock1));
                BOOST_TEST((conclaveBlock7 != conclaveBlock2));
                BOOST_TEST((conclaveBlock7 != conclaveBlock3));
                BOOST_TEST((conclaveBlock7 != conclaveBlock4));
                BOOST_TEST((conclaveBlock7 != conclaveBlock5));
                BOOST_TEST((conclaveBlock7 != conclaveBlock6));
                BOOST_TEST((conclaveBlock7 == conclaveBlock7));
                BOOST_TEST((conclaveBlock7 != conclaveBlock8));
                BOOST_TEST((conclaveBlock7 != conclaveBlock9));
                BOOST_TEST((conclaveBlock7 != conclaveBlock10));
                BOOST_TEST((conclaveBlock8 != conclaveBlock1));
                BOOST_TEST((conclaveBlock8 != conclaveBlock2));
                BOOST_TEST((conclaveBlock8 != conclaveBlock3));
                BOOST_TEST((conclaveBlock8 != conclaveBlock4));
                BOOST_TEST((conclaveBlock8 != conclaveBlock5));
                BOOST_TEST((conclaveBlock8 != conclaveBlock6));
                BOOST_TEST((conclaveBlock8 != conclaveBlock7));
                BOOST_TEST((conclaveBlock8 == conclaveBlock8));
                BOOST_TEST((conclaveBlock8 != conclaveBlock9));
                BOOST_TEST((conclaveBlock8 != conclaveBlock10));
                BOOST_TEST((conclaveBlock9 != conclaveBlock1));
                BOOST_TEST((conclaveBlock9 != conclaveBlock2));
                BOOST_TEST((conclaveBlock9 != conclaveBlock3));
                BOOST_TEST((conclaveBlock9 != conclaveBlock4));
                BOOST_TEST((conclaveBlock9 != conclaveBlock5));
                BOOST_TEST((conclaveBlock9 != conclaveBlock6));
                BOOST_TEST((conclaveBlock9 != conclaveBlock7));
                BOOST_TEST((conclaveBlock9 != conclaveBlock8));
                BOOST_TEST((conclaveBlock9 == conclaveBlock9));
                BOOST_TEST((conclaveBlock9 != conclaveBlock10));
                BOOST_TEST((conclaveBlock10 != conclaveBlock1));
                BOOST_TEST((conclaveBlock10 != conclaveBlock2));
                BOOST_TEST((conclaveBlock10 != conclaveBlock3));
                BOOST_TEST((conclaveBlock10 != conclaveBlock4));
                BOOST_TEST((conclaveBlock10 != conclaveBlock5));
                BOOST_TEST((conclaveBlock10 != conclaveBlock6));
                BOOST_TEST((conclaveBlock10 != conclaveBlock7));
                BOOST_TEST((conclaveBlock10 != conclaveBlock8));
                BOOST_TEST((conclaveBlock10 != conclaveBlock9));
                BOOST_TEST((conclaveBlock10 == conclaveBlock10));
            }
        
        BOOST_AUTO_TEST_SUITE_END()
    }
}
