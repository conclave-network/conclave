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
#define BOOST_TEST_MODULE Private_Key_Test

#include <boost/test/included/unit_test.hpp>
#include "../src/private_key.h"

namespace conclave
{
    const static Hash256 KEY_DATA_1("017b8511ce04f889d3ef08df1c4497794a2fce1c92a84562bbe5c6d572bfc67c");
    const static Hash256 KEY_DATA_2("bb76e7bfc8248e47f59dd63534171cc54e6b49dd70cf16e45027bbd4e429e60a");
    
    BOOST_AUTO_TEST_CASE(PrivateKeyConstructorTest)
    {
        PrivateKey privateKey1(KEY_DATA_1);
        PublicKey publicKey = privateKey1.getPublicKey();
        std::cout << publicKey << std::endl;
    }
}
