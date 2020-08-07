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
    const static Hash256 R_1("699b0b07b06f9687e0ae214c717d32998f46e7c085966d9d92a3840d597249f0");
    const static Hash256 S_1("5dfeb8c8d701087e81b08b22170da77cd2b4fdc752971961afd9e141707a7683");
    const static Hash256 R_2("13e36927f48cecd5d56e6b4f924fffbed1b871929fc9fe9ee7af518f92eb406e");
    const static Hash256 S_2("f3dfd48975511a0e340241b32420e9e5e44cb79c8626ddbe7c94a95e1190e2a6");
    BOOST_AUTO_TEST_SUITE(EcdsaSignatureTestSuite)
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureRSConstructorsTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
            EcdsaSignature r1s1Move(std::move(Hash256(R_1)), std::move(Hash256(S_1)));
            EcdsaSignature r1s2Move(std::move(Hash256(R_1)), std::move(Hash256(S_2)));
            EcdsaSignature r2s1Move(std::move(Hash256(R_2)), std::move(Hash256(S_1)));
            EcdsaSignature r2s2Move(std::move(Hash256(R_2)), std::move(Hash256(S_2)));
            BOOST_ASSERT((r1s1 == r1s1));
            BOOST_ASSERT((r1s1 != r1s2));
            BOOST_ASSERT((r1s1 != r2s1));
            BOOST_ASSERT((r1s1 != r2s2));
            BOOST_ASSERT((r1s1 == r1s1Move));
            BOOST_ASSERT((r1s1 != r1s2Move));
            BOOST_ASSERT((r1s1 != r2s1Move));
            BOOST_ASSERT((r1s1 != r2s2Move));
            BOOST_ASSERT((r1s2 != r1s1));
            BOOST_ASSERT((r1s2 == r1s2));
            BOOST_ASSERT((r1s2 != r2s1));
            BOOST_ASSERT((r1s2 != r2s2));
            BOOST_ASSERT((r1s2 != r1s1Move));
            BOOST_ASSERT((r1s2 == r1s2Move));
            BOOST_ASSERT((r1s2 != r2s1Move));
            BOOST_ASSERT((r1s2 != r2s2Move));
            BOOST_ASSERT((r2s1 != r1s1));
            BOOST_ASSERT((r2s1 != r1s2));
            BOOST_ASSERT((r2s1 == r2s1));
            BOOST_ASSERT((r2s1 != r2s2));
            BOOST_ASSERT((r2s1 != r1s1Move));
            BOOST_ASSERT((r2s1 != r1s2Move));
            BOOST_ASSERT((r2s1 == r2s1Move));
            BOOST_ASSERT((r2s1 != r2s2Move));
            BOOST_ASSERT((r2s2 != r1s1));
            BOOST_ASSERT((r2s2 != r1s2));
            BOOST_ASSERT((r2s2 != r2s1));
            BOOST_ASSERT((r2s2 == r2s2));
            BOOST_ASSERT((r2s2 != r1s1Move));
            BOOST_ASSERT((r2s2 != r1s2Move));
            BOOST_ASSERT((r2s2 != r2s1Move));
            BOOST_ASSERT((r2s2 == r2s2Move));
            BOOST_ASSERT((r1s1Move == r1s1));
            BOOST_ASSERT((r1s1Move != r1s2));
            BOOST_ASSERT((r1s1Move != r2s1));
            BOOST_ASSERT((r1s1Move != r2s2));
            BOOST_ASSERT((r1s1Move == r1s1Move));
            BOOST_ASSERT((r1s1Move != r1s2Move));
            BOOST_ASSERT((r1s1Move != r2s1Move));
            BOOST_ASSERT((r1s1Move != r2s2Move));
            BOOST_ASSERT((r1s2Move != r1s1));
            BOOST_ASSERT((r1s2Move == r1s2));
            BOOST_ASSERT((r1s2Move != r2s1));
            BOOST_ASSERT((r1s2Move != r2s2));
            BOOST_ASSERT((r1s2Move != r1s1Move));
            BOOST_ASSERT((r1s2Move == r1s2Move));
            BOOST_ASSERT((r1s2Move != r2s1Move));
            BOOST_ASSERT((r1s2Move != r2s2Move));
            BOOST_ASSERT((r2s1Move != r1s1));
            BOOST_ASSERT((r2s1Move != r1s2));
            BOOST_ASSERT((r2s1Move == r2s1));
            BOOST_ASSERT((r2s1Move != r2s2));
            BOOST_ASSERT((r2s1Move != r1s1Move));
            BOOST_ASSERT((r2s1Move != r1s2Move));
            BOOST_ASSERT((r2s1Move == r2s1Move));
            BOOST_ASSERT((r2s1Move != r2s2Move));
            BOOST_ASSERT((r2s2Move != r1s1));
            BOOST_ASSERT((r2s2Move != r1s2));
            BOOST_ASSERT((r2s2Move != r2s1));
            BOOST_ASSERT((r2s2Move == r2s2));
            BOOST_ASSERT((r2s2Move != r1s1Move));
            BOOST_ASSERT((r2s2Move != r1s2Move));
            BOOST_ASSERT((r2s2Move != r2s1Move));
            BOOST_ASSERT((r2s2Move == r2s2Move));
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
            BOOST_ASSERT((r1s1 == r1s1));
            BOOST_ASSERT((r1s1 != r1s2));
            BOOST_ASSERT((r1s1 != r2s1));
            BOOST_ASSERT((r1s1 != r2s2));
            BOOST_ASSERT((r1s1 == copyOfR1s1));
            BOOST_ASSERT((r1s1 != copyOfR1s2));
            BOOST_ASSERT((r1s1 != copyOfR2s1));
            BOOST_ASSERT((r1s1 != copyOfR2s2));
            BOOST_ASSERT((r1s2 != r1s1));
            BOOST_ASSERT((r1s2 == r1s2));
            BOOST_ASSERT((r1s2 != r2s1));
            BOOST_ASSERT((r1s2 != r2s2));
            BOOST_ASSERT((r1s2 != copyOfR1s1));
            BOOST_ASSERT((r1s2 == copyOfR1s2));
            BOOST_ASSERT((r1s2 != copyOfR2s1));
            BOOST_ASSERT((r1s2 != copyOfR2s2));
            BOOST_ASSERT((r2s1 != r1s1));
            BOOST_ASSERT((r2s1 != r1s2));
            BOOST_ASSERT((r2s1 == r2s1));
            BOOST_ASSERT((r2s1 != r2s2));
            BOOST_ASSERT((r2s1 != copyOfR1s1));
            BOOST_ASSERT((r2s1 != copyOfR1s2));
            BOOST_ASSERT((r2s1 == copyOfR2s1));
            BOOST_ASSERT((r2s1 != copyOfR2s2));
            BOOST_ASSERT((r2s2 != r1s1));
            BOOST_ASSERT((r2s2 != r1s2));
            BOOST_ASSERT((r2s2 != r2s1));
            BOOST_ASSERT((r2s2 == r2s2));
            BOOST_ASSERT((r2s2 != copyOfR1s1));
            BOOST_ASSERT((r2s2 != copyOfR1s2));
            BOOST_ASSERT((r2s2 != copyOfR2s1));
            BOOST_ASSERT((r2s2 == copyOfR2s2));
            BOOST_ASSERT((copyOfR1s1 == r1s1));
            BOOST_ASSERT((copyOfR1s1 != r1s2));
            BOOST_ASSERT((copyOfR1s1 != r2s1));
            BOOST_ASSERT((copyOfR1s1 != r2s2));
            BOOST_ASSERT((copyOfR1s1 == copyOfR1s1));
            BOOST_ASSERT((copyOfR1s1 != copyOfR1s2));
            BOOST_ASSERT((copyOfR1s1 != copyOfR2s1));
            BOOST_ASSERT((copyOfR1s1 != copyOfR2s2));
            BOOST_ASSERT((copyOfR1s2 != r1s1));
            BOOST_ASSERT((copyOfR1s2 == r1s2));
            BOOST_ASSERT((copyOfR1s2 != r2s1));
            BOOST_ASSERT((copyOfR1s2 != r2s2));
            BOOST_ASSERT((copyOfR1s2 != copyOfR1s1));
            BOOST_ASSERT((copyOfR1s2 == copyOfR1s2));
            BOOST_ASSERT((copyOfR1s2 != copyOfR2s1));
            BOOST_ASSERT((copyOfR1s2 != copyOfR2s2));
            BOOST_ASSERT((copyOfR2s1 != r1s1));
            BOOST_ASSERT((copyOfR2s1 != r1s2));
            BOOST_ASSERT((copyOfR2s1 == r2s1));
            BOOST_ASSERT((copyOfR2s1 != r2s2));
            BOOST_ASSERT((copyOfR2s1 != copyOfR1s1));
            BOOST_ASSERT((copyOfR2s1 != copyOfR1s2));
            BOOST_ASSERT((copyOfR2s1 == copyOfR2s1));
            BOOST_ASSERT((copyOfR2s1 != copyOfR2s2));
            BOOST_ASSERT((copyOfR2s2 != r1s1));
            BOOST_ASSERT((copyOfR2s2 != r1s2));
            BOOST_ASSERT((copyOfR2s2 != r2s1));
            BOOST_ASSERT((copyOfR2s2 == r2s2));
            BOOST_ASSERT((copyOfR2s2 != copyOfR1s1));
            BOOST_ASSERT((copyOfR2s2 != copyOfR1s2));
            BOOST_ASSERT((copyOfR2s2 != copyOfR2s1));
            BOOST_ASSERT((copyOfR2s2 == copyOfR2s2));
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
            BOOST_ASSERT((r1s1 == r1s1));
            BOOST_ASSERT((r1s1 != r1s2));
            BOOST_ASSERT((r1s1 != r2s1));
            BOOST_ASSERT((r1s1 != r2s2));
            BOOST_ASSERT((r1s1 == moveOfR1s1));
            BOOST_ASSERT((r1s1 != moveOfR1s2));
            BOOST_ASSERT((r1s1 != moveOfR2s1));
            BOOST_ASSERT((r1s1 != moveOfR2s2));
            BOOST_ASSERT((r1s2 != r1s1));
            BOOST_ASSERT((r1s2 == r1s2));
            BOOST_ASSERT((r1s2 != r2s1));
            BOOST_ASSERT((r1s2 != r2s2));
            BOOST_ASSERT((r1s2 != moveOfR1s1));
            BOOST_ASSERT((r1s2 == moveOfR1s2));
            BOOST_ASSERT((r1s2 != moveOfR2s1));
            BOOST_ASSERT((r1s2 != moveOfR2s2));
            BOOST_ASSERT((r2s1 != r1s1));
            BOOST_ASSERT((r2s1 != r1s2));
            BOOST_ASSERT((r2s1 == r2s1));
            BOOST_ASSERT((r2s1 != r2s2));
            BOOST_ASSERT((r2s1 != moveOfR1s1));
            BOOST_ASSERT((r2s1 != moveOfR1s2));
            BOOST_ASSERT((r2s1 == moveOfR2s1));
            BOOST_ASSERT((r2s1 != moveOfR2s2));
            BOOST_ASSERT((r2s2 != r1s1));
            BOOST_ASSERT((r2s2 != r1s2));
            BOOST_ASSERT((r2s2 != r2s1));
            BOOST_ASSERT((r2s2 == r2s2));
            BOOST_ASSERT((r2s2 != moveOfR1s1));
            BOOST_ASSERT((r2s2 != moveOfR1s2));
            BOOST_ASSERT((r2s2 != moveOfR2s1));
            BOOST_ASSERT((r2s2 == moveOfR2s2));
            BOOST_ASSERT((moveOfR1s1 == r1s1));
            BOOST_ASSERT((moveOfR1s1 != r1s2));
            BOOST_ASSERT((moveOfR1s1 != r2s1));
            BOOST_ASSERT((moveOfR1s1 != r2s2));
            BOOST_ASSERT((moveOfR1s1 == moveOfR1s1));
            BOOST_ASSERT((moveOfR1s1 != moveOfR1s2));
            BOOST_ASSERT((moveOfR1s1 != moveOfR2s1));
            BOOST_ASSERT((moveOfR1s1 != moveOfR2s2));
            BOOST_ASSERT((moveOfR1s2 != r1s1));
            BOOST_ASSERT((moveOfR1s2 == r1s2));
            BOOST_ASSERT((moveOfR1s2 != r2s1));
            BOOST_ASSERT((moveOfR1s2 != r2s2));
            BOOST_ASSERT((moveOfR1s2 != moveOfR1s1));
            BOOST_ASSERT((moveOfR1s2 == moveOfR1s2));
            BOOST_ASSERT((moveOfR1s2 != moveOfR2s1));
            BOOST_ASSERT((moveOfR1s2 != moveOfR2s2));
            BOOST_ASSERT((moveOfR2s1 != r1s1));
            BOOST_ASSERT((moveOfR2s1 != r1s2));
            BOOST_ASSERT((moveOfR2s1 == r2s1));
            BOOST_ASSERT((moveOfR2s1 != r2s2));
            BOOST_ASSERT((moveOfR2s1 != moveOfR1s1));
            BOOST_ASSERT((moveOfR2s1 != moveOfR1s2));
            BOOST_ASSERT((moveOfR2s1 == moveOfR2s1));
            BOOST_ASSERT((moveOfR2s1 != moveOfR2s2));
            BOOST_ASSERT((moveOfR2s2 != r1s1));
            BOOST_ASSERT((moveOfR2s2 != r1s2));
            BOOST_ASSERT((moveOfR2s2 != r2s1));
            BOOST_ASSERT((moveOfR2s2 == r2s2));
            BOOST_ASSERT((moveOfR2s2 != moveOfR1s1));
            BOOST_ASSERT((moveOfR2s2 != moveOfR1s2));
            BOOST_ASSERT((moveOfR2s2 != moveOfR2s1));
            BOOST_ASSERT((moveOfR2s2 == moveOfR2s2));
        }
        
        BOOST_AUTO_TEST_CASE(EcdsaSignatureSerializeTest)
        {
            EcdsaSignature r1s1(R_1, S_1);
            EcdsaSignature r1s2(R_1, S_2);
            EcdsaSignature r2s1(R_2, S_1);
            EcdsaSignature r2s2(R_2, S_2);
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
