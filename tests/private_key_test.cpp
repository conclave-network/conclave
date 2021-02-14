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
#define BOOST_TEST_MODULE Private_Key_Test

#include <boost/test/included/unit_test.hpp>
#include "../src/private_key.h"

namespace conclave
{
    const static Hash256 KEY_DATA_1("017b8511ce04f889d3ef08df1c4497794a2fce1c92a84562bbe5c6d572bfc67c");
    const static Hash256 MESSAGE_1("c48dc09b1e0495d33f6af7493fa10d050d5ffd5fc9a4f6bdaee0d1f1680f0feb");
    const static PublicKey K1_PUB("03f50ca6f27d1c1f461160ae4cc3588141dc048b262101b5faa8a22e9de4c99c08");
    const static EcdsaSignature K1_M1_SIG(hexStringToByteArray<ECDSA_SIGNATURE_SIZE_BYTES>(
        "ba19d85bb520ba8d05c8f2c2805868962ccda6d121ad76286c9431a04eaea91f"
        "af5cbf2333b156e13de4f638014f9785ce6e85d287b0b89712a6ba1cc5834f27"));
    BOOST_AUTO_TEST_SUITE(PrivateKeyTestSuite)
        
        BOOST_AUTO_TEST_CASE(PrivateKeyConstructorTest)
        {
            PrivateKey privateKey1(KEY_DATA_1); // Shouldn't throw
        }
        
        BOOST_AUTO_TEST_CASE(PrivateKeyGetPublicKeyTest)
        {
            PrivateKey privateKey1(KEY_DATA_1);
            PublicKey publicKey1 = privateKey1.getPublicKey();
            BOOST_TEST((publicKey1 == K1_PUB));
        }
        
        BOOST_AUTO_TEST_CASE(PrivateKeyEcdsaSignTest)
        {
            PrivateKey privateKey1(KEY_DATA_1);
            EcdsaSignature k1m1Sig = privateKey1.sign(MESSAGE_1);
            BOOST_TEST((k1m1Sig == K1_M1_SIG));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
