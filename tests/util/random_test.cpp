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

#define BOOST_TEST_MODULE Random_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/util/random.h"

namespace conclave
{
    const static size_t RANDOM_ARRAY_SIZE = 10000;
    
    /**
     * Very basic test to make sure makeRandomByteArray doesn't return the same value on two subsequent callings.
     */
    BOOST_AUTO_TEST_CASE(MakeRandomByteArrayDifferentValueSanityTest)
    {
        const std::array<BYTE, RANDOM_ARRAY_SIZE> arr1 = makeRandomByteArray<RANDOM_ARRAY_SIZE>();
        const std::array<BYTE, RANDOM_ARRAY_SIZE> arr2 = makeRandomByteArray<RANDOM_ARRAY_SIZE>();
        BOOST_TEST(arr1 != arr2);
    }
    
    /**
     * Very basic test to make sure the mean byte value of an array returned by makeRandomByteArray isn't crazy.
     */
    BOOST_AUTO_TEST_CASE(MakeRandomByteArrayMeanSanityTest)
    {
        const std::array<BYTE, RANDOM_ARRAY_SIZE> arr = makeRandomByteArray<RANDOM_ARRAY_SIZE>();
        double tot = 0;
        for (BYTE b: arr) {
            tot += (double) b;
        }
        double mean = tot / (double) RANDOM_ARRAY_SIZE;
        BOOST_TEST(mean > 96.0);
        BOOST_TEST(mean < 160.0);
        BOOST_TEST(mean != 128.0);
    }
}
