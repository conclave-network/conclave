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

#define BOOST_TEST_MODULE Ecdsa_Signature_Test

#include <boost/test/included/unit_test.hpp>
#include "../src/ecdsa_signature.h"

namespace conclave
{
    const static Hash256 R_1("99b0b07b06f9687e0ae214c717d32998f46e7c085966d9d92a3840d597249f06");
    const static Hash256 S_1("5dfeb8c8d701087e81b08b22170da77cd2b4fdc752971961afd9e141707a7683");
    const static Hash256 R_2("13e36927f48cecd5d56e6b4f924fffbed1b871929fc9fe9ee7af518f92eb406e");
    const static Hash256 S_2("f3dfd48975511a0e340241b32420e9e5e44cb79c8626ddbe7c94a95e1190e2a6");
    const static std::string R1S1_STR = "99b0b07b06f9687e0ae214c717d32998f46e7c085966d9d92a3840d597249f06"
                                        "5dfeb8c8d701087e81b08b22170da77cd2b4fdc752971961afd9e141707a7683";
    const static std::string R1S2_STR = "99b0b07b06f9687e0ae214c717d32998f46e7c085966d9d92a3840d597249f06"
                                        "f3dfd48975511a0e340241b32420e9e5e44cb79c8626ddbe7c94a95e1190e2a6";
    const static std::string R2S1_STR = "13e36927f48cecd5d56e6b4f924fffbed1b871929fc9fe9ee7af518f92eb406e"
                                        "5dfeb8c8d701087e81b08b22170da77cd2b4fdc752971961afd9e141707a7683";
    const static std::string R2S2_STR = "13e36927f48cecd5d56e6b4f924fffbed1b871929fc9fe9ee7af518f92eb406e"
                                        "f3dfd48975511a0e340241b32420e9e5e44cb79c8626ddbe7c94a95e1190e2a6";
    const static std::string R1S1_DER_STR =
        "30450220069f2497d540382ad9d96659087c6ef49829d317c714e20a7e68f9067bb0b0990221"
        "0083767a7041e1d9af61199752c7fdb4d27ca70d17228bb0817e0801d7c8b8fe5d";
    const static std::string R1S2_DER_STR =
        "30450220069f2497d540382ad9d96659087c6ef49829d317c714e20a7e68f9067bb0b0990221"
        "00a6e290115ea9947cbedd26869cb74ce4e5e92024b34102340e1a517589d4dff3";
    const static std::string R2S1_DER_STR =
        "304502206e40eb928f51afe79efec99f9271b8d1beff4f924f6b6ed5d5ec8cf42769e3130221"
        "0083767a7041e1d9af61199752c7fdb4d27ca70d17228bb0817e0801d7c8b8fe5d";
    const static std::string R2S2_DER_STR =
        "304502206e40eb928f51afe79efec99f9271b8d1beff4f924f6b6ed5d5ec8cf42769e3130221"
        "00a6e290115ea9947cbedd26869cb74ce4e5e92024b34102340e1a517589d4dff3";
    const static auto R1S1_BA = hexStringToByteArray<ECDSA_SIGNATURE_SIZE_BYTES>(R1S1_STR);
    const static auto R1S2_BA = hexStringToByteArray<ECDSA_SIGNATURE_SIZE_BYTES>(R1S2_STR);
    const static auto R2S1_BA = hexStringToByteArray<ECDSA_SIGNATURE_SIZE_BYTES>(R2S1_STR);
    const static auto R2S2_BA = hexStringToByteArray<ECDSA_SIGNATURE_SIZE_BYTES>(R2S2_STR);
    const static auto R1S1_DER_BV = HEX_TO_BYTE_VECTOR(R1S1_DER_STR);
    const static auto R1S2_DER_BV = HEX_TO_BYTE_VECTOR(R1S2_DER_STR);
    const static auto R2S1_DER_BV = HEX_TO_BYTE_VECTOR(R2S1_DER_STR);
    const static auto R2S2_DER_BV = HEX_TO_BYTE_VECTOR(R2S2_DER_STR);
    const static auto R1S1_DER_BV_ONELEADING = HEX_TO_BYTE_VECTOR("00" + R1S1_DER_STR);
    const static auto R1S2_DER_BV_TWOLEADING = HEX_TO_BYTE_VECTOR("ff00" + R1S2_DER_STR);
    const static auto R2S1_DER_BV_THREELEADING = HEX_TO_BYTE_VECTOR("a8c289" + R2S1_DER_STR);
    const static auto R2S2_DER_BV_FOURLEADING = HEX_TO_BYTE_VECTOR("00d618ff" + R2S2_DER_STR);
    BOOST_AUTO_TEST_SUITE(EcdsaSignatureTestSuite)
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureDeserializeFactoryTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature r1s1Deserialized = EcdsaSignature::deserialize(R1S1_DER_BV);
            EcdsaSignature r1s2Deserialized = EcdsaSignature::deserialize(R1S2_DER_BV);
            EcdsaSignature r2s1Deserialized = EcdsaSignature::deserialize(R2S1_DER_BV);
            EcdsaSignature r2s2Deserialized = EcdsaSignature::deserialize(R2S2_DER_BV);
            BOOST_TEST((r1s1 == r1s1Deserialized));
            BOOST_TEST((r1s1 != r1s2Deserialized));
            BOOST_TEST((r1s1 != r2s1Deserialized));
            BOOST_TEST((r1s1 != r2s2Deserialized));
            BOOST_TEST((r1s2 != r1s1Deserialized));
            BOOST_TEST((r1s2 == r1s2Deserialized));
            BOOST_TEST((r1s2 != r2s1Deserialized));
            BOOST_TEST((r1s2 != r2s2Deserialized));
            BOOST_TEST((r2s1 != r1s1Deserialized));
            BOOST_TEST((r2s1 != r1s2Deserialized));
            BOOST_TEST((r2s1 == r2s1Deserialized));
            BOOST_TEST((r2s1 != r2s2Deserialized));
            BOOST_TEST((r2s2 != r1s1Deserialized));
            BOOST_TEST((r2s2 != r1s2Deserialized));
            BOOST_TEST((r2s2 != r2s1Deserialized));
            BOOST_TEST((r2s2 == r2s2Deserialized));
            BOOST_TEST((r1s1Deserialized == r1s1));
            BOOST_TEST((r1s1Deserialized != r1s2));
            BOOST_TEST((r1s1Deserialized != r2s1));
            BOOST_TEST((r1s1Deserialized != r2s2));
            BOOST_TEST((r1s1Deserialized == r1s1Deserialized));
            BOOST_TEST((r1s1Deserialized != r1s2Deserialized));
            BOOST_TEST((r1s1Deserialized != r2s1Deserialized));
            BOOST_TEST((r1s1Deserialized != r2s2Deserialized));
            BOOST_TEST((r1s2Deserialized != r1s1));
            BOOST_TEST((r1s2Deserialized == r1s2));
            BOOST_TEST((r1s2Deserialized != r2s1));
            BOOST_TEST((r1s2Deserialized != r2s2));
            BOOST_TEST((r1s2Deserialized != r1s1Deserialized));
            BOOST_TEST((r1s2Deserialized == r1s2Deserialized));
            BOOST_TEST((r1s2Deserialized != r2s1Deserialized));
            BOOST_TEST((r1s2Deserialized != r2s2Deserialized));
            BOOST_TEST((r2s1Deserialized != r1s1));
            BOOST_TEST((r2s1Deserialized != r1s2));
            BOOST_TEST((r2s1Deserialized == r2s1));
            BOOST_TEST((r2s1Deserialized != r2s2));
            BOOST_TEST((r2s1Deserialized != r1s1Deserialized));
            BOOST_TEST((r2s1Deserialized != r1s2Deserialized));
            BOOST_TEST((r2s1Deserialized == r2s1Deserialized));
            BOOST_TEST((r2s1Deserialized != r2s2Deserialized));
            BOOST_TEST((r2s2Deserialized != r1s1));
            BOOST_TEST((r2s2Deserialized != r1s2));
            BOOST_TEST((r2s2Deserialized != r2s1));
            BOOST_TEST((r2s2Deserialized == r2s2));
            BOOST_TEST((r2s2Deserialized != r1s1Deserialized));
            BOOST_TEST((r2s2Deserialized != r1s2Deserialized));
            BOOST_TEST((r2s2Deserialized != r2s1Deserialized));
            BOOST_TEST((r2s2Deserialized == r2s2Deserialized));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureDeserializeAtPosFactoryTest)
        {
            size_t pos1 = 1, pos2 = 2, pos3 = 3, pos4 = 4;
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature r1s1Deserialized = EcdsaSignature::deserialize(R1S1_DER_BV_ONELEADING, pos1);
            EcdsaSignature r1s2Deserialized = EcdsaSignature::deserialize(R1S2_DER_BV_TWOLEADING, pos2);
            EcdsaSignature r2s1Deserialized = EcdsaSignature::deserialize(R2S1_DER_BV_THREELEADING, pos3);
            EcdsaSignature r2s2Deserialized = EcdsaSignature::deserialize(R2S2_DER_BV_FOURLEADING, pos4);
            BOOST_TEST((r1s1 == r1s1Deserialized));
            BOOST_TEST((r1s1 != r1s2Deserialized));
            BOOST_TEST((r1s1 != r2s1Deserialized));
            BOOST_TEST((r1s1 != r2s2Deserialized));
            BOOST_TEST((r1s2 != r1s1Deserialized));
            BOOST_TEST((r1s2 == r1s2Deserialized));
            BOOST_TEST((r1s2 != r2s1Deserialized));
            BOOST_TEST((r1s2 != r2s2Deserialized));
            BOOST_TEST((r2s1 != r1s1Deserialized));
            BOOST_TEST((r2s1 != r1s2Deserialized));
            BOOST_TEST((r2s1 == r2s1Deserialized));
            BOOST_TEST((r2s1 != r2s2Deserialized));
            BOOST_TEST((r2s2 != r1s1Deserialized));
            BOOST_TEST((r2s2 != r1s2Deserialized));
            BOOST_TEST((r2s2 != r2s1Deserialized));
            BOOST_TEST((r2s2 == r2s2Deserialized));
            BOOST_TEST((r1s1Deserialized == r1s1));
            BOOST_TEST((r1s1Deserialized != r1s2));
            BOOST_TEST((r1s1Deserialized != r2s1));
            BOOST_TEST((r1s1Deserialized != r2s2));
            BOOST_TEST((r1s1Deserialized == r1s1Deserialized));
            BOOST_TEST((r1s1Deserialized != r1s2Deserialized));
            BOOST_TEST((r1s1Deserialized != r2s1Deserialized));
            BOOST_TEST((r1s1Deserialized != r2s2Deserialized));
            BOOST_TEST((r1s2Deserialized != r1s1));
            BOOST_TEST((r1s2Deserialized == r1s2));
            BOOST_TEST((r1s2Deserialized != r2s1));
            BOOST_TEST((r1s2Deserialized != r2s2));
            BOOST_TEST((r1s2Deserialized != r1s1Deserialized));
            BOOST_TEST((r1s2Deserialized == r1s2Deserialized));
            BOOST_TEST((r1s2Deserialized != r2s1Deserialized));
            BOOST_TEST((r1s2Deserialized != r2s2Deserialized));
            BOOST_TEST((r2s1Deserialized != r1s1));
            BOOST_TEST((r2s1Deserialized != r1s2));
            BOOST_TEST((r2s1Deserialized == r2s1));
            BOOST_TEST((r2s1Deserialized != r2s2));
            BOOST_TEST((r2s1Deserialized != r1s1Deserialized));
            BOOST_TEST((r2s1Deserialized != r1s2Deserialized));
            BOOST_TEST((r2s1Deserialized == r2s1Deserialized));
            BOOST_TEST((r2s1Deserialized != r2s2Deserialized));
            BOOST_TEST((r2s2Deserialized != r1s1));
            BOOST_TEST((r2s2Deserialized != r1s2));
            BOOST_TEST((r2s2Deserialized != r2s1));
            BOOST_TEST((r2s2Deserialized == r2s2));
            BOOST_TEST((r2s2Deserialized != r1s1Deserialized));
            BOOST_TEST((r2s2Deserialized != r1s2Deserialized));
            BOOST_TEST((r2s2Deserialized != r2s1Deserialized));
            BOOST_TEST((r2s2Deserialized == r2s2Deserialized));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureRSConstructorsTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            BOOST_TEST((r1s1 == r1s1));
            BOOST_TEST((r1s1 != r1s2));
            BOOST_TEST((r1s1 != r2s1));
            BOOST_TEST((r1s1 != r2s2));
            BOOST_TEST((r1s2 != r1s1));
            BOOST_TEST((r1s2 == r1s2));
            BOOST_TEST((r1s2 != r2s1));
            BOOST_TEST((r1s2 != r2s2));
            BOOST_TEST((r2s1 != r1s1));
            BOOST_TEST((r2s1 != r1s2));
            BOOST_TEST((r2s1 == r2s1));
            BOOST_TEST((r2s1 != r2s2));
            BOOST_TEST((r2s2 != r1s1));
            BOOST_TEST((r2s2 != r1s2));
            BOOST_TEST((r2s2 != r2s1));
            BOOST_TEST((r2s2 == r2s2));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureRSMoveConstructorsTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature r1s1Move(std::move(Hash256(R_1)), std::move(Hash256(S_1)));
            EcdsaSignature r1s2Move(std::move(Hash256(R_1)), std::move(Hash256(S_2)));
            EcdsaSignature r2s1Move(std::move(Hash256(R_2)), std::move(Hash256(S_1)));
            EcdsaSignature r2s2Move(std::move(Hash256(R_2)), std::move(Hash256(S_2)));
            BOOST_TEST((r1s1 == r1s1Move));
            BOOST_TEST((r1s1 != r1s2Move));
            BOOST_TEST((r1s1 != r2s1Move));
            BOOST_TEST((r1s1 != r2s2Move));
            BOOST_TEST((r1s2 != r1s1Move));
            BOOST_TEST((r1s2 == r1s2Move));
            BOOST_TEST((r1s2 != r2s1Move));
            BOOST_TEST((r1s2 != r2s2Move));
            BOOST_TEST((r2s1 != r1s1Move));
            BOOST_TEST((r2s1 != r1s2Move));
            BOOST_TEST((r2s1 == r2s1Move));
            BOOST_TEST((r2s1 != r2s2Move));
            BOOST_TEST((r2s2 != r1s1Move));
            BOOST_TEST((r2s2 != r1s2Move));
            BOOST_TEST((r2s2 != r2s1Move));
            BOOST_TEST((r2s2 == r2s2Move));
            BOOST_TEST((r1s1Move == r1s1));
            BOOST_TEST((r1s1Move != r1s2));
            BOOST_TEST((r1s1Move != r2s1));
            BOOST_TEST((r1s1Move != r2s2));
            BOOST_TEST((r1s1Move == r1s1Move));
            BOOST_TEST((r1s1Move != r1s2Move));
            BOOST_TEST((r1s1Move != r2s1Move));
            BOOST_TEST((r1s1Move != r2s2Move));
            BOOST_TEST((r1s2Move != r1s1));
            BOOST_TEST((r1s2Move == r1s2));
            BOOST_TEST((r1s2Move != r2s1));
            BOOST_TEST((r1s2Move != r2s2));
            BOOST_TEST((r1s2Move != r1s1Move));
            BOOST_TEST((r1s2Move == r1s2Move));
            BOOST_TEST((r1s2Move != r2s1Move));
            BOOST_TEST((r1s2Move != r2s2Move));
            BOOST_TEST((r2s1Move != r1s1));
            BOOST_TEST((r2s1Move != r1s2));
            BOOST_TEST((r2s1Move == r2s1));
            BOOST_TEST((r2s1Move != r2s2));
            BOOST_TEST((r2s1Move != r1s1Move));
            BOOST_TEST((r2s1Move != r1s2Move));
            BOOST_TEST((r2s1Move == r2s1Move));
            BOOST_TEST((r2s1Move != r2s2Move));
            BOOST_TEST((r2s2Move != r1s1));
            BOOST_TEST((r2s2Move != r1s2));
            BOOST_TEST((r2s2Move != r2s1));
            BOOST_TEST((r2s2Move == r2s2));
            BOOST_TEST((r2s2Move != r1s1Move));
            BOOST_TEST((r2s2Move != r1s2Move));
            BOOST_TEST((r2s2Move != r2s1Move));
            BOOST_TEST((r2s2Move == r2s2Move));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureByteArrayConstructorTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature r1s1FromBA(R1S1_BA);
            EcdsaSignature r1s2FromBA(R1S2_BA);
            EcdsaSignature r2s1FromBA(R2S1_BA);
            EcdsaSignature r2s2FromBA(R2S2_BA);
            BOOST_TEST((r1s1 == r1s1FromBA));
            BOOST_TEST((r1s1 != r1s2FromBA));
            BOOST_TEST((r1s1 != r2s1FromBA));
            BOOST_TEST((r1s1 != r2s2FromBA));
            BOOST_TEST((r1s2 != r1s1FromBA));
            BOOST_TEST((r1s2 == r1s2FromBA));
            BOOST_TEST((r1s2 != r2s1FromBA));
            BOOST_TEST((r1s2 != r2s2FromBA));
            BOOST_TEST((r2s1 != r1s1FromBA));
            BOOST_TEST((r2s1 != r1s2FromBA));
            BOOST_TEST((r2s1 == r2s1FromBA));
            BOOST_TEST((r2s1 != r2s2FromBA));
            BOOST_TEST((r2s2 != r1s1FromBA));
            BOOST_TEST((r2s2 != r1s2FromBA));
            BOOST_TEST((r2s2 != r2s1FromBA));
            BOOST_TEST((r2s2 == r2s2FromBA));
            BOOST_TEST((r1s1FromBA == r1s1));
            BOOST_TEST((r1s1FromBA != r1s2));
            BOOST_TEST((r1s1FromBA != r2s1));
            BOOST_TEST((r1s1FromBA != r2s2));
            BOOST_TEST((r1s1FromBA == r1s1FromBA));
            BOOST_TEST((r1s1FromBA != r1s2FromBA));
            BOOST_TEST((r1s1FromBA != r2s1FromBA));
            BOOST_TEST((r1s1FromBA != r2s2FromBA));
            BOOST_TEST((r1s2FromBA != r1s1));
            BOOST_TEST((r1s2FromBA == r1s2));
            BOOST_TEST((r1s2FromBA != r2s1));
            BOOST_TEST((r1s2FromBA != r2s2));
            BOOST_TEST((r1s2FromBA != r1s1FromBA));
            BOOST_TEST((r1s2FromBA == r1s2FromBA));
            BOOST_TEST((r1s2FromBA != r2s1FromBA));
            BOOST_TEST((r1s2FromBA != r2s2FromBA));
            BOOST_TEST((r2s1FromBA != r1s1));
            BOOST_TEST((r2s1FromBA != r1s2));
            BOOST_TEST((r2s1FromBA == r2s1));
            BOOST_TEST((r2s1FromBA != r2s2));
            BOOST_TEST((r2s1FromBA != r1s1FromBA));
            BOOST_TEST((r2s1FromBA != r1s2FromBA));
            BOOST_TEST((r2s1FromBA == r2s1FromBA));
            BOOST_TEST((r2s1FromBA != r2s2FromBA));
            BOOST_TEST((r2s2FromBA != r1s1));
            BOOST_TEST((r2s2FromBA != r1s2));
            BOOST_TEST((r2s2FromBA != r2s1));
            BOOST_TEST((r2s2FromBA == r2s2));
            BOOST_TEST((r2s2FromBA != r1s1FromBA));
            BOOST_TEST((r2s2FromBA != r1s2FromBA));
            BOOST_TEST((r2s2FromBA != r2s1FromBA));
            BOOST_TEST((r2s2FromBA == r2s2FromBA));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureCopyConstructorTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature copyOfR1s1(r1s1);
            EcdsaSignature copyOfR1s2(r1s2);
            EcdsaSignature copyOfR2s1(r2s1);
            EcdsaSignature copyOfR2s2(r2s2);
            BOOST_TEST((r1s1 == copyOfR1s1));
            BOOST_TEST((r1s1 != copyOfR1s2));
            BOOST_TEST((r1s1 != copyOfR2s1));
            BOOST_TEST((r1s1 != copyOfR2s2));
            BOOST_TEST((r1s2 != copyOfR1s1));
            BOOST_TEST((r1s2 == copyOfR1s2));
            BOOST_TEST((r1s2 != copyOfR2s1));
            BOOST_TEST((r1s2 != copyOfR2s2));
            BOOST_TEST((r2s1 != copyOfR1s1));
            BOOST_TEST((r2s1 != copyOfR1s2));
            BOOST_TEST((r2s1 == copyOfR2s1));
            BOOST_TEST((r2s1 != copyOfR2s2));
            BOOST_TEST((r2s2 != copyOfR1s1));
            BOOST_TEST((r2s2 != copyOfR1s2));
            BOOST_TEST((r2s2 != copyOfR2s1));
            BOOST_TEST((r2s2 == copyOfR2s2));
            BOOST_TEST((copyOfR1s1 == r1s1));
            BOOST_TEST((copyOfR1s1 != r1s2));
            BOOST_TEST((copyOfR1s1 != r2s1));
            BOOST_TEST((copyOfR1s1 != r2s2));
            BOOST_TEST((copyOfR1s1 == copyOfR1s1));
            BOOST_TEST((copyOfR1s1 != copyOfR1s2));
            BOOST_TEST((copyOfR1s1 != copyOfR2s1));
            BOOST_TEST((copyOfR1s1 != copyOfR2s2));
            BOOST_TEST((copyOfR1s2 != r1s1));
            BOOST_TEST((copyOfR1s2 == r1s2));
            BOOST_TEST((copyOfR1s2 != r2s1));
            BOOST_TEST((copyOfR1s2 != r2s2));
            BOOST_TEST((copyOfR1s2 != copyOfR1s1));
            BOOST_TEST((copyOfR1s2 == copyOfR1s2));
            BOOST_TEST((copyOfR1s2 != copyOfR2s1));
            BOOST_TEST((copyOfR1s2 != copyOfR2s2));
            BOOST_TEST((copyOfR2s1 != r1s1));
            BOOST_TEST((copyOfR2s1 != r1s2));
            BOOST_TEST((copyOfR2s1 == r2s1));
            BOOST_TEST((copyOfR2s1 != r2s2));
            BOOST_TEST((copyOfR2s1 != copyOfR1s1));
            BOOST_TEST((copyOfR2s1 != copyOfR1s2));
            BOOST_TEST((copyOfR2s1 == copyOfR2s1));
            BOOST_TEST((copyOfR2s1 != copyOfR2s2));
            BOOST_TEST((copyOfR2s2 != r1s1));
            BOOST_TEST((copyOfR2s2 != r1s2));
            BOOST_TEST((copyOfR2s2 != r2s1));
            BOOST_TEST((copyOfR2s2 == r2s2));
            BOOST_TEST((copyOfR2s2 != copyOfR1s1));
            BOOST_TEST((copyOfR2s2 != copyOfR1s2));
            BOOST_TEST((copyOfR2s2 != copyOfR2s1));
            BOOST_TEST((copyOfR2s2 == copyOfR2s2));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureMoveConstructorTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature moveOfR1s1(std::move(EcdsaSignature(r1s1)));
            EcdsaSignature moveOfR1s2(std::move(EcdsaSignature(r1s2)));
            EcdsaSignature moveOfR2s1(std::move(EcdsaSignature(r2s1)));
            EcdsaSignature moveOfR2s2(std::move(EcdsaSignature(r2s2)));
            BOOST_TEST((r1s1 == moveOfR1s1));
            BOOST_TEST((r1s1 != moveOfR1s2));
            BOOST_TEST((r1s1 != moveOfR2s1));
            BOOST_TEST((r1s1 != moveOfR2s2));
            BOOST_TEST((r1s2 != moveOfR1s1));
            BOOST_TEST((r1s2 == moveOfR1s2));
            BOOST_TEST((r1s2 != moveOfR2s1));
            BOOST_TEST((r1s2 != moveOfR2s2));
            BOOST_TEST((r2s1 != moveOfR1s1));
            BOOST_TEST((r2s1 != moveOfR1s2));
            BOOST_TEST((r2s1 == moveOfR2s1));
            BOOST_TEST((r2s1 != moveOfR2s2));
            BOOST_TEST((r2s2 != moveOfR1s1));
            BOOST_TEST((r2s2 != moveOfR1s2));
            BOOST_TEST((r2s2 != moveOfR2s1));
            BOOST_TEST((r2s2 == moveOfR2s2));
            BOOST_TEST((moveOfR1s1 == r1s1));
            BOOST_TEST((moveOfR1s1 != r1s2));
            BOOST_TEST((moveOfR1s1 != r2s1));
            BOOST_TEST((moveOfR1s1 != r2s2));
            BOOST_TEST((moveOfR1s1 == moveOfR1s1));
            BOOST_TEST((moveOfR1s1 != moveOfR1s2));
            BOOST_TEST((moveOfR1s1 != moveOfR2s1));
            BOOST_TEST((moveOfR1s1 != moveOfR2s2));
            BOOST_TEST((moveOfR1s2 != r1s1));
            BOOST_TEST((moveOfR1s2 == r1s2));
            BOOST_TEST((moveOfR1s2 != r2s1));
            BOOST_TEST((moveOfR1s2 != r2s2));
            BOOST_TEST((moveOfR1s2 != moveOfR1s1));
            BOOST_TEST((moveOfR1s2 == moveOfR1s2));
            BOOST_TEST((moveOfR1s2 != moveOfR2s1));
            BOOST_TEST((moveOfR1s2 != moveOfR2s2));
            BOOST_TEST((moveOfR2s1 != r1s1));
            BOOST_TEST((moveOfR2s1 != r1s2));
            BOOST_TEST((moveOfR2s1 == r2s1));
            BOOST_TEST((moveOfR2s1 != r2s2));
            BOOST_TEST((moveOfR2s1 != moveOfR1s1));
            BOOST_TEST((moveOfR2s1 != moveOfR1s2));
            BOOST_TEST((moveOfR2s1 == moveOfR2s1));
            BOOST_TEST((moveOfR2s1 != moveOfR2s2));
            BOOST_TEST((moveOfR2s2 != r1s1));
            BOOST_TEST((moveOfR2s2 != r1s2));
            BOOST_TEST((moveOfR2s2 != r2s1));
            BOOST_TEST((moveOfR2s2 == r2s2));
            BOOST_TEST((moveOfR2s2 != moveOfR1s1));
            BOOST_TEST((moveOfR2s2 != moveOfR1s2));
            BOOST_TEST((moveOfR2s2 != moveOfR2s1));
            BOOST_TEST((moveOfR2s2 == moveOfR2s2));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureSerializeTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            BOOST_TEST((r1s1.serialize() == R1S1_DER_BV));
            BOOST_TEST((r1s2.serialize() == R1S2_DER_BV));
            BOOST_TEST((r2s1.serialize() == R2S1_DER_BV));
            BOOST_TEST((r2s2.serialize() == R2S2_DER_BV));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
