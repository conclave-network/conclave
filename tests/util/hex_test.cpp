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

#define BOOST_TEST_MODULE Hex_Test

#include "../../src/util/hex.h"
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <vector>
#include <array>

const static std::vector<BYTE> EMPTY_BYTE_VECTOR;
const static std::array<BYTE, 0> EMPTY_BYTE_ARRAY;
const static std::vector<BYTE> ONE_BYTE_VECTOR{0xfe};
const static std::array<BYTE, 1> ONE_BYTE_ARRAY{0xfe};
const static std::vector<BYTE> TWO_BYTE_VECTOR{0x02, 0xe7};
const static std::array<BYTE, 2> TWO_BYTE_ARRAY{0x02, 0xe7};
const static std::vector<BYTE> THREE_BYTE_VECTOR{0xff, 0x00, 0x80};
const static std::array<BYTE, 3> THREE_BYTE_ARRAY{0xff, 0x00, 0x80};
const static std::string EMPTY_HEX_STRING = "";
const static std::string ONE_BYTE_HEX_STRING = "fe";
const static std::string TWO_BYTE_HEX_STRING = "02e7";
const static std::string THREE_BYTE_HEX_STRING = "ff0080";

BOOST_AUTO_TEST_CASE(HexTest)
{
    std::vector<BYTE> emptyByteVector = hexStringToByteVector(EMPTY_HEX_STRING);
    std::array<BYTE, 0> emptyByteArray = hexStringToByteArray<0>(EMPTY_HEX_STRING);
    std::vector<BYTE> oneByteVector = hexStringToByteVector(ONE_BYTE_HEX_STRING);
    std::array<BYTE, 1> oneByteArray = hexStringToByteArray<1>(ONE_BYTE_HEX_STRING);
    std::vector<BYTE> twoByteVector = hexStringToByteVector(TWO_BYTE_HEX_STRING);
    std::array<BYTE, 2> twoByteArray = hexStringToByteArray<2>(TWO_BYTE_HEX_STRING);
    std::vector<BYTE> threeByteVector = hexStringToByteVector(THREE_BYTE_HEX_STRING);
    std::array<BYTE, 3> threeByteArray = hexStringToByteArray<3>(THREE_BYTE_HEX_STRING);
    std::string emptyHexStringFromByteVector = byteVectorToHexString(EMPTY_BYTE_VECTOR);
    std::string emptyHexStringFromByteArray = byteArrayToHexString(EMPTY_BYTE_ARRAY);
    std::string oneByteHexStringFromByteVector = byteVectorToHexString(ONE_BYTE_VECTOR);
    std::string oneByteHexStringFromByteArray = byteArrayToHexString(ONE_BYTE_ARRAY);
    std::string twoByteHexStringFromByteVector = byteVectorToHexString(TWO_BYTE_VECTOR);
    std::string twoByteHexStringFromByteArray = byteArrayToHexString(TWO_BYTE_ARRAY);
    std::string threeByteHexStringFromByteVector = byteVectorToHexString(THREE_BYTE_VECTOR);
    std::string threeByteHexStringFromByteArray = byteArrayToHexString(THREE_BYTE_ARRAY);
    BOOST_TEST(emptyByteVector == EMPTY_BYTE_VECTOR);
    BOOST_TEST(emptyByteArray == EMPTY_BYTE_ARRAY);
    BOOST_TEST(oneByteVector == ONE_BYTE_VECTOR);
    BOOST_TEST(oneByteArray == ONE_BYTE_ARRAY);
    BOOST_TEST(twoByteVector == TWO_BYTE_VECTOR);
    BOOST_TEST(twoByteArray == TWO_BYTE_ARRAY);
    BOOST_TEST(threeByteVector == THREE_BYTE_VECTOR);
    BOOST_TEST(threeByteArray == THREE_BYTE_ARRAY);
    BOOST_TEST(emptyHexStringFromByteVector == EMPTY_HEX_STRING);
    BOOST_TEST(emptyHexStringFromByteArray == EMPTY_HEX_STRING);
    BOOST_TEST(oneByteHexStringFromByteVector == ONE_BYTE_HEX_STRING);
    BOOST_TEST(oneByteHexStringFromByteArray == ONE_BYTE_HEX_STRING);
    BOOST_TEST(twoByteHexStringFromByteVector == TWO_BYTE_HEX_STRING);
    BOOST_TEST(twoByteHexStringFromByteArray == TWO_BYTE_HEX_STRING);
    BOOST_TEST(threeByteHexStringFromByteVector == THREE_BYTE_HEX_STRING);
    BOOST_TEST(threeByteHexStringFromByteArray == THREE_BYTE_HEX_STRING);
}
