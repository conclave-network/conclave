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

#define BOOST_TEST_MODULE Hash160_Test

#include <boost/test/included/unit_test.hpp>
#include "../src/hash160.h"
#include "../src/conclave.h"
#include <array>

namespace conclave
{
    const static std::array<BYTE, SMALL_HASH_SIZE_BYTES> ALL_ZEROS_BA_1{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    const static std::array<BYTE, SMALL_HASH_SIZE_BYTES> ALL_ZEROS_BA_2{
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    const static std::array<BYTE, SMALL_HASH_SIZE_BYTES> ALL_ONES_BA_1{
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    const static std::array<BYTE, SMALL_HASH_SIZE_BYTES> ALL_ONES_BA_2{
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    const static std::string ALL_ZEROS_STR_1 = "0000000000000000000000000000000000000000";
    const static std::string ALL_ZEROS_STR_2 = "0000000000000000000000000000000000000000";
    const static std::string ALL_ONES_STR_1 = "ffffffffffffffffffffffffffffffffffffffff";
    const static std::string ALL_ONES_STR_2 = "ffffffffffffffffffffffffffffffffffffffff";
    const static char* ALL_ZEROS_CSTR_1 = "0000000000000000000000000000000000000000";
    const static char* ALL_ZEROS_CSTR_2 = "0000000000000000000000000000000000000000";
    const static char* ALL_ONES_CSTR_1 = "ffffffffffffffffffffffffffffffffffffffff";
    const static char* ALL_ONES_CSTR_2 = "ffffffffffffffffffffffffffffffffffffffff";
    const static Hash160 RANDOM_HASH_1;
    const static Hash160 RANDOM_HASH_2;
    
    BOOST_AUTO_TEST_CASE(Hash160DefaultConstructorTest)
    {
        // Default constructor should initialize with random data
        BOOST_TEST(RANDOM_HASH_1 != RANDOM_HASH_2);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160ByteArrayConstructorTest)
    {
        Hash160 zeros1(ALL_ZEROS_BA_1);
        Hash160 zeros2(ALL_ZEROS_BA_1);
        Hash160 zeros3(ALL_ZEROS_BA_2);
        Hash160 zeros4(ALL_ZEROS_BA_2);
        Hash160 ones1(ALL_ONES_BA_1);
        Hash160 ones2(ALL_ONES_BA_1);
        Hash160 ones3(ALL_ONES_BA_2);
        Hash160 ones4(ALL_ONES_BA_2);
        BOOST_TEST(zeros1 == zeros1);
        BOOST_TEST(zeros1 == zeros2);
        BOOST_TEST(zeros1 == zeros3);
        BOOST_TEST(zeros1 == zeros4);
        BOOST_TEST(zeros1 != ones1);
        BOOST_TEST(zeros1 != ones2);
        BOOST_TEST(zeros1 != ones3);
        BOOST_TEST(zeros1 != ones4);
        BOOST_TEST(zeros2 == zeros1);
        BOOST_TEST(zeros2 == zeros2);
        BOOST_TEST(zeros2 == zeros3);
        BOOST_TEST(zeros2 == zeros4);
        BOOST_TEST(zeros2 != ones1);
        BOOST_TEST(zeros2 != ones2);
        BOOST_TEST(zeros2 != ones3);
        BOOST_TEST(zeros2 != ones4);
        BOOST_TEST(zeros3 == zeros1);
        BOOST_TEST(zeros3 == zeros2);
        BOOST_TEST(zeros3 == zeros3);
        BOOST_TEST(zeros3 == zeros4);
        BOOST_TEST(zeros3 != ones1);
        BOOST_TEST(zeros3 != ones2);
        BOOST_TEST(zeros3 != ones3);
        BOOST_TEST(zeros3 != ones4);
        BOOST_TEST(zeros4 == zeros1);
        BOOST_TEST(zeros4 == zeros2);
        BOOST_TEST(zeros4 == zeros3);
        BOOST_TEST(zeros4 == zeros4);
        BOOST_TEST(zeros4 != ones1);
        BOOST_TEST(zeros4 != ones2);
        BOOST_TEST(zeros4 != ones3);
        BOOST_TEST(zeros4 != ones4);
        BOOST_TEST(ones1 != zeros1);
        BOOST_TEST(ones1 != zeros2);
        BOOST_TEST(ones1 != zeros3);
        BOOST_TEST(ones1 != zeros4);
        BOOST_TEST(ones1 == ones1);
        BOOST_TEST(ones1 == ones2);
        BOOST_TEST(ones1 == ones3);
        BOOST_TEST(ones1 == ones4);
        BOOST_TEST(ones2 != zeros1);
        BOOST_TEST(ones2 != zeros2);
        BOOST_TEST(ones2 != zeros3);
        BOOST_TEST(ones2 != zeros4);
        BOOST_TEST(ones2 == ones1);
        BOOST_TEST(ones2 == ones2);
        BOOST_TEST(ones2 == ones3);
        BOOST_TEST(ones2 == ones4);
        BOOST_TEST(ones3 != zeros1);
        BOOST_TEST(ones3 != zeros2);
        BOOST_TEST(ones3 != zeros3);
        BOOST_TEST(ones3 != zeros4);
        BOOST_TEST(ones3 == ones1);
        BOOST_TEST(ones3 == ones2);
        BOOST_TEST(ones3 == ones3);
        BOOST_TEST(ones3 == ones4);
        BOOST_TEST(ones4 != zeros1);
        BOOST_TEST(ones4 != zeros2);
        BOOST_TEST(ones4 != zeros3);
        BOOST_TEST(ones4 != zeros4);
        BOOST_TEST(ones4 == ones1);
        BOOST_TEST(ones4 == ones2);
        BOOST_TEST(ones4 == ones3);
        BOOST_TEST(ones4 == ones4);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160StringConstructorTest)
    {
        Hash160 zeros1(ALL_ZEROS_STR_1);
        Hash160 zeros2(ALL_ZEROS_STR_1);
        Hash160 zeros3(ALL_ZEROS_STR_2);
        Hash160 zeros4(ALL_ZEROS_STR_2);
        Hash160 ones1(ALL_ONES_STR_1);
        Hash160 ones2(ALL_ONES_STR_1);
        Hash160 ones3(ALL_ONES_STR_2);
        Hash160 ones4(ALL_ONES_STR_2);
        BOOST_TEST(zeros1 == zeros1);
        BOOST_TEST(zeros1 == zeros2);
        BOOST_TEST(zeros1 == zeros3);
        BOOST_TEST(zeros1 == zeros4);
        BOOST_TEST(zeros1 != ones1);
        BOOST_TEST(zeros1 != ones2);
        BOOST_TEST(zeros1 != ones3);
        BOOST_TEST(zeros1 != ones4);
        BOOST_TEST(zeros2 == zeros1);
        BOOST_TEST(zeros2 == zeros2);
        BOOST_TEST(zeros2 == zeros3);
        BOOST_TEST(zeros2 == zeros4);
        BOOST_TEST(zeros2 != ones1);
        BOOST_TEST(zeros2 != ones2);
        BOOST_TEST(zeros2 != ones3);
        BOOST_TEST(zeros2 != ones4);
        BOOST_TEST(zeros3 == zeros1);
        BOOST_TEST(zeros3 == zeros2);
        BOOST_TEST(zeros3 == zeros3);
        BOOST_TEST(zeros3 == zeros4);
        BOOST_TEST(zeros3 != ones1);
        BOOST_TEST(zeros3 != ones2);
        BOOST_TEST(zeros3 != ones3);
        BOOST_TEST(zeros3 != ones4);
        BOOST_TEST(zeros4 == zeros1);
        BOOST_TEST(zeros4 == zeros2);
        BOOST_TEST(zeros4 == zeros3);
        BOOST_TEST(zeros4 == zeros4);
        BOOST_TEST(zeros4 != ones1);
        BOOST_TEST(zeros4 != ones2);
        BOOST_TEST(zeros4 != ones3);
        BOOST_TEST(zeros4 != ones4);
        BOOST_TEST(ones1 != zeros1);
        BOOST_TEST(ones1 != zeros2);
        BOOST_TEST(ones1 != zeros3);
        BOOST_TEST(ones1 != zeros4);
        BOOST_TEST(ones1 == ones1);
        BOOST_TEST(ones1 == ones2);
        BOOST_TEST(ones1 == ones3);
        BOOST_TEST(ones1 == ones4);
        BOOST_TEST(ones2 != zeros1);
        BOOST_TEST(ones2 != zeros2);
        BOOST_TEST(ones2 != zeros3);
        BOOST_TEST(ones2 != zeros4);
        BOOST_TEST(ones2 == ones1);
        BOOST_TEST(ones2 == ones2);
        BOOST_TEST(ones2 == ones3);
        BOOST_TEST(ones2 == ones4);
        BOOST_TEST(ones3 != zeros1);
        BOOST_TEST(ones3 != zeros2);
        BOOST_TEST(ones3 != zeros3);
        BOOST_TEST(ones3 != zeros4);
        BOOST_TEST(ones3 == ones1);
        BOOST_TEST(ones3 == ones2);
        BOOST_TEST(ones3 == ones3);
        BOOST_TEST(ones3 == ones4);
        BOOST_TEST(ones4 != zeros1);
        BOOST_TEST(ones4 != zeros2);
        BOOST_TEST(ones4 != zeros3);
        BOOST_TEST(ones4 != zeros4);
        BOOST_TEST(ones4 == ones1);
        BOOST_TEST(ones4 == ones2);
        BOOST_TEST(ones4 == ones3);
        BOOST_TEST(ones4 == ones4);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160CStringConstructorTest)
    {
        Hash160 zeros1(ALL_ZEROS_CSTR_1);
        Hash160 zeros2(ALL_ZEROS_CSTR_1);
        Hash160 zeros3(ALL_ZEROS_CSTR_2);
        Hash160 zeros4(ALL_ZEROS_CSTR_2);
        Hash160 ones1(ALL_ONES_CSTR_1);
        Hash160 ones2(ALL_ONES_CSTR_1);
        Hash160 ones3(ALL_ONES_CSTR_2);
        Hash160 ones4(ALL_ONES_CSTR_2);
        BOOST_TEST(zeros1 == zeros1);
        BOOST_TEST(zeros1 == zeros2);
        BOOST_TEST(zeros1 == zeros3);
        BOOST_TEST(zeros1 == zeros4);
        BOOST_TEST(zeros1 != ones1);
        BOOST_TEST(zeros1 != ones2);
        BOOST_TEST(zeros1 != ones3);
        BOOST_TEST(zeros1 != ones4);
        BOOST_TEST(zeros2 == zeros1);
        BOOST_TEST(zeros2 == zeros2);
        BOOST_TEST(zeros2 == zeros3);
        BOOST_TEST(zeros2 == zeros4);
        BOOST_TEST(zeros2 != ones1);
        BOOST_TEST(zeros2 != ones2);
        BOOST_TEST(zeros2 != ones3);
        BOOST_TEST(zeros2 != ones4);
        BOOST_TEST(zeros3 == zeros1);
        BOOST_TEST(zeros3 == zeros2);
        BOOST_TEST(zeros3 == zeros3);
        BOOST_TEST(zeros3 == zeros4);
        BOOST_TEST(zeros3 != ones1);
        BOOST_TEST(zeros3 != ones2);
        BOOST_TEST(zeros3 != ones3);
        BOOST_TEST(zeros3 != ones4);
        BOOST_TEST(zeros4 == zeros1);
        BOOST_TEST(zeros4 == zeros2);
        BOOST_TEST(zeros4 == zeros3);
        BOOST_TEST(zeros4 == zeros4);
        BOOST_TEST(zeros4 != ones1);
        BOOST_TEST(zeros4 != ones2);
        BOOST_TEST(zeros4 != ones3);
        BOOST_TEST(zeros4 != ones4);
        BOOST_TEST(ones1 != zeros1);
        BOOST_TEST(ones1 != zeros2);
        BOOST_TEST(ones1 != zeros3);
        BOOST_TEST(ones1 != zeros4);
        BOOST_TEST(ones1 == ones1);
        BOOST_TEST(ones1 == ones2);
        BOOST_TEST(ones1 == ones3);
        BOOST_TEST(ones1 == ones4);
        BOOST_TEST(ones2 != zeros1);
        BOOST_TEST(ones2 != zeros2);
        BOOST_TEST(ones2 != zeros3);
        BOOST_TEST(ones2 != zeros4);
        BOOST_TEST(ones2 == ones1);
        BOOST_TEST(ones2 == ones2);
        BOOST_TEST(ones2 == ones3);
        BOOST_TEST(ones2 == ones4);
        BOOST_TEST(ones3 != zeros1);
        BOOST_TEST(ones3 != zeros2);
        BOOST_TEST(ones3 != zeros3);
        BOOST_TEST(ones3 != zeros4);
        BOOST_TEST(ones3 == ones1);
        BOOST_TEST(ones3 == ones2);
        BOOST_TEST(ones3 == ones3);
        BOOST_TEST(ones3 == ones4);
        BOOST_TEST(ones4 != zeros1);
        BOOST_TEST(ones4 != zeros2);
        BOOST_TEST(ones4 != zeros3);
        BOOST_TEST(ones4 != zeros4);
        BOOST_TEST(ones4 == ones1);
        BOOST_TEST(ones4 == ones2);
        BOOST_TEST(ones4 == ones3);
        BOOST_TEST(ones4 == ones4);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160CopyConstructorTest)
    {
        Hash160 copy1(RANDOM_HASH_1);
        Hash160 copy2(RANDOM_HASH_2);
        BOOST_TEST(copy1 == copy1);
        BOOST_TEST(copy1 != copy2);
        BOOST_TEST(copy1 == RANDOM_HASH_1);
        BOOST_TEST(copy1 != RANDOM_HASH_2);
        BOOST_TEST(copy2 != copy1);
        BOOST_TEST(copy2 == copy2);
        BOOST_TEST(copy2 != RANDOM_HASH_1);
        BOOST_TEST(copy2 == RANDOM_HASH_2);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160MoveConstructorTest)
    {
        Hash160 move1(std::move(Hash160(RANDOM_HASH_1)));
        Hash160 move2(std::move(Hash160(RANDOM_HASH_2)));
        BOOST_TEST(move1 == move1);
        BOOST_TEST(move1 != move2);
        BOOST_TEST(move1 == RANDOM_HASH_1);
        BOOST_TEST(move1 != RANDOM_HASH_2);
        BOOST_TEST(move2 != move1);
        BOOST_TEST(move2 == move2);
        BOOST_TEST(move2 != RANDOM_HASH_1);
        BOOST_TEST(move2 == RANDOM_HASH_2);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160AllConstructorsTest)
    {
        Hash160 zerosFromBA(ALL_ZEROS_BA_1);
        Hash160 zerosFromStr(ALL_ZEROS_STR_1);
        Hash160 zerosFromCStr(ALL_ZEROS_CSTR_1);
        Hash160 zerosFromCopyOfZerosFromBA(zerosFromBA);
        Hash160 zerosFromCopyOfZerosFromStr(zerosFromStr);
        Hash160 zerosFromCopyOfZerosFromCStr(zerosFromCStr);
        Hash160 zerosFromMoveOfZerosFromBA(std::move(Hash160(zerosFromBA)));
        Hash160 zerosFromMoveOfZerosFromStr(std::move(Hash160(zerosFromStr)));
        Hash160 zerosFromMoveOfZerosFromCStr(std::move(Hash160(zerosFromCStr)));
        Hash160 onesFromBA(ALL_ONES_BA_1);
        Hash160 onesFromStr(ALL_ONES_STR_1);
        Hash160 onesFromCStr(ALL_ONES_CSTR_1);
        Hash160 onesFromCopyOfOnesFromBA(onesFromBA);
        Hash160 onesFromCopyOfOnesFromStr(onesFromStr);
        Hash160 onesFromCopyOfOnesFromCStr(onesFromCStr);
        Hash160 onesFromMoveOfOnesFromBA(std::move(Hash160(onesFromBA)));
        Hash160 onesFromMoveOfOnesFromStr(std::move(Hash160(onesFromStr)));
        Hash160 onesFromMoveOfOnesFromCStr(std::move(Hash160(onesFromCStr)));
        BOOST_TEST(zerosFromBA == zerosFromBA);
        BOOST_TEST(zerosFromBA == zerosFromStr);
        BOOST_TEST(zerosFromBA == zerosFromCStr);
        BOOST_TEST(zerosFromBA == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromBA == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromBA == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromBA == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromBA == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromBA == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromBA != onesFromBA);
        BOOST_TEST(zerosFromBA != onesFromStr);
        BOOST_TEST(zerosFromBA != onesFromCStr);
        BOOST_TEST(zerosFromBA != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromBA != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromBA != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromBA != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromBA != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromBA != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(zerosFromStr == zerosFromBA);
        BOOST_TEST(zerosFromStr == zerosFromStr);
        BOOST_TEST(zerosFromStr == zerosFromCStr);
        BOOST_TEST(zerosFromStr == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromStr == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromStr == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromStr == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromStr == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromStr == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromStr != onesFromBA);
        BOOST_TEST(zerosFromStr != onesFromStr);
        BOOST_TEST(zerosFromStr != onesFromCStr);
        BOOST_TEST(zerosFromStr != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromStr != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromStr != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromStr != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromStr != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromStr != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(zerosFromCStr == zerosFromBA);
        BOOST_TEST(zerosFromCStr == zerosFromStr);
        BOOST_TEST(zerosFromCStr == zerosFromCStr);
        BOOST_TEST(zerosFromCStr == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromCStr == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromCStr == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromCStr == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromCStr == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromCStr == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromCStr != onesFromBA);
        BOOST_TEST(zerosFromCStr != onesFromStr);
        BOOST_TEST(zerosFromCStr != onesFromCStr);
        BOOST_TEST(zerosFromCStr != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromCStr != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromCStr != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromCStr != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromCStr != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromCStr != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromBA != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromCopyOfZerosFromStr != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromBA != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromCopyOfZerosFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromCopyOfZerosFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromMoveOfZerosFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromMoveOfZerosFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr == zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromCopyOfOnesFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromCopyOfOnesFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromCopyOfOnesFromCStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromMoveOfOnesFromBA);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromMoveOfOnesFromStr);
        BOOST_TEST(zerosFromMoveOfZerosFromStr != onesFromMoveOfOnesFromCStr);
        BOOST_TEST(onesFromBA != zerosFromBA);
        BOOST_TEST(onesFromBA != zerosFromStr);
        BOOST_TEST(onesFromBA != zerosFromCStr);
        BOOST_TEST(onesFromBA != zerosFromCopyOfZerosFromBA);
        BOOST_TEST(onesFromBA != zerosFromCopyOfZerosFromStr);
        BOOST_TEST(onesFromBA != zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(onesFromBA != zerosFromMoveOfZerosFromBA);
        BOOST_TEST(onesFromBA != zerosFromMoveOfZerosFromStr);
        BOOST_TEST(onesFromBA != zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(onesFromBA == onesFromBA);
        BOOST_TEST(onesFromBA == onesFromStr);
        BOOST_TEST(onesFromBA == onesFromCStr);
        BOOST_TEST(onesFromBA == onesFromCopyOfOnesFromBA);
        BOOST_TEST(onesFromBA == onesFromCopyOfOnesFromStr);
        BOOST_TEST(onesFromBA == onesFromCopyOfOnesFromCStr);
        BOOST_TEST(onesFromBA == onesFromMoveOfOnesFromBA);
        BOOST_TEST(onesFromBA == onesFromMoveOfOnesFromStr);
        BOOST_TEST(onesFromBA == onesFromMoveOfOnesFromCStr);
        BOOST_TEST(onesFromStr != zerosFromBA);
        BOOST_TEST(onesFromStr != zerosFromStr);
        BOOST_TEST(onesFromStr != zerosFromCStr);
        BOOST_TEST(onesFromStr != zerosFromCopyOfZerosFromBA);
        BOOST_TEST(onesFromStr != zerosFromCopyOfZerosFromStr);
        BOOST_TEST(onesFromStr != zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(onesFromStr != zerosFromMoveOfZerosFromBA);
        BOOST_TEST(onesFromStr != zerosFromMoveOfZerosFromStr);
        BOOST_TEST(onesFromStr != zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(onesFromStr == onesFromBA);
        BOOST_TEST(onesFromStr == onesFromStr);
        BOOST_TEST(onesFromStr == onesFromCStr);
        BOOST_TEST(onesFromStr == onesFromCopyOfOnesFromBA);
        BOOST_TEST(onesFromStr == onesFromCopyOfOnesFromStr);
        BOOST_TEST(onesFromStr == onesFromCopyOfOnesFromCStr);
        BOOST_TEST(onesFromStr == onesFromMoveOfOnesFromBA);
        BOOST_TEST(onesFromStr == onesFromMoveOfOnesFromStr);
        BOOST_TEST(onesFromStr == onesFromMoveOfOnesFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromCopyOfZerosFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromCopyOfZerosFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromMoveOfZerosFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromMoveOfZerosFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA != zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromCopyOfOnesFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromCopyOfOnesFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromCopyOfOnesFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromMoveOfOnesFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromMoveOfOnesFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromBA == onesFromMoveOfOnesFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromCopyOfZerosFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromCopyOfZerosFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromMoveOfZerosFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromMoveOfZerosFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr != zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromCopyOfOnesFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromCopyOfOnesFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromCopyOfOnesFromCStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromMoveOfOnesFromBA);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromMoveOfOnesFromStr);
        BOOST_TEST(onesFromCopyOfOnesFromStr == onesFromMoveOfOnesFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromCopyOfZerosFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromCopyOfZerosFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromMoveOfZerosFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromMoveOfZerosFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA != zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromCopyOfOnesFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromCopyOfOnesFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromCopyOfOnesFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromMoveOfOnesFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromMoveOfOnesFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromBA == onesFromMoveOfOnesFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromCopyOfZerosFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromCopyOfZerosFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromCopyOfZerosFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromMoveOfZerosFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromMoveOfZerosFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr != zerosFromMoveOfZerosFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromCopyOfOnesFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromCopyOfOnesFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromCopyOfOnesFromCStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromMoveOfOnesFromBA);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromMoveOfOnesFromStr);
        BOOST_TEST(onesFromMoveOfOnesFromStr == onesFromMoveOfOnesFromCStr);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160ReverseTest)
    {
        Hash160 forward;
        Hash160 reverse = forward.reversed();
        BOOST_TEST(forward[0] == reverse[19]);
        BOOST_TEST(forward[1] == reverse[18]);
        BOOST_TEST(forward[2] == reverse[17]);
        BOOST_TEST(forward[3] == reverse[16]);
        BOOST_TEST(forward[4] == reverse[15]);
        BOOST_TEST(forward[5] == reverse[14]);
        BOOST_TEST(forward[6] == reverse[13]);
        BOOST_TEST(forward[7] == reverse[12]);
        BOOST_TEST(forward[8] == reverse[11]);
        BOOST_TEST(forward[9] == reverse[10]);
        BOOST_TEST(forward[10] == reverse[9]);
        BOOST_TEST(forward[11] == reverse[8]);
        BOOST_TEST(forward[12] == reverse[7]);
        BOOST_TEST(forward[13] == reverse[6]);
        BOOST_TEST(forward[14] == reverse[5]);
        BOOST_TEST(forward[15] == reverse[4]);
        BOOST_TEST(forward[16] == reverse[3]);
        BOOST_TEST(forward[17] == reverse[2]);
        BOOST_TEST(forward[18] == reverse[1]);
        BOOST_TEST(forward[19] == reverse[0]);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160IterationTest)
    {
        Hash160 hash;
        BOOST_TEST(&hash[0] == hash.begin());
        BOOST_TEST(&hash[19] + 1 == hash.end());
    }
    
    BOOST_AUTO_TEST_CASE(Hash160EqualityOperatorsTest)
    {
        Hash160 hash1("0000000000000000000000000000000000000000");
        Hash160 hash2("8000000000000000000000000000000000000000");
        Hash160 hash3("0000000000000000000000000000000000000001");
        Hash160 hash4("8000000000000000000000000000000000000001");
        Hash160 hash5("0000000000000000000000000000000000000000");
        Hash160 hash6("8000000000000000000000000000000000000000");
        Hash160 hash7("0000000000000000000000000000000000000001");
        Hash160 hash8("8000000000000000000000000000000000000001");
        BOOST_TEST(hash1 == hash1);
        BOOST_TEST(hash1 != hash2);
        BOOST_TEST(hash1 != hash3);
        BOOST_TEST(hash1 != hash4);
        BOOST_TEST(hash1 == hash5);
        BOOST_TEST(hash1 != hash6);
        BOOST_TEST(hash1 != hash7);
        BOOST_TEST(hash1 != hash8);
        BOOST_TEST(hash2 != hash1);
        BOOST_TEST(hash2 == hash2);
        BOOST_TEST(hash2 != hash3);
        BOOST_TEST(hash2 != hash4);
        BOOST_TEST(hash2 != hash5);
        BOOST_TEST(hash2 == hash6);
        BOOST_TEST(hash2 != hash7);
        BOOST_TEST(hash2 != hash8);
        BOOST_TEST(hash3 != hash1);
        BOOST_TEST(hash3 != hash2);
        BOOST_TEST(hash3 == hash3);
        BOOST_TEST(hash3 != hash4);
        BOOST_TEST(hash3 != hash5);
        BOOST_TEST(hash3 != hash6);
        BOOST_TEST(hash3 == hash7);
        BOOST_TEST(hash3 != hash8);
        BOOST_TEST(hash4 != hash1);
        BOOST_TEST(hash4 != hash2);
        BOOST_TEST(hash4 != hash3);
        BOOST_TEST(hash4 == hash4);
        BOOST_TEST(hash4 != hash5);
        BOOST_TEST(hash4 != hash6);
        BOOST_TEST(hash4 != hash7);
        BOOST_TEST(hash4 == hash8);
        BOOST_TEST(hash5 == hash1);
        BOOST_TEST(hash5 != hash2);
        BOOST_TEST(hash5 != hash3);
        BOOST_TEST(hash5 != hash4);
        BOOST_TEST(hash5 == hash5);
        BOOST_TEST(hash5 != hash6);
        BOOST_TEST(hash5 != hash7);
        BOOST_TEST(hash5 != hash8);
        BOOST_TEST(hash6 != hash1);
        BOOST_TEST(hash6 == hash2);
        BOOST_TEST(hash6 != hash3);
        BOOST_TEST(hash6 != hash4);
        BOOST_TEST(hash6 != hash5);
        BOOST_TEST(hash6 == hash6);
        BOOST_TEST(hash6 != hash7);
        BOOST_TEST(hash6 != hash8);
        BOOST_TEST(hash7 != hash1);
        BOOST_TEST(hash7 != hash2);
        BOOST_TEST(hash7 == hash3);
        BOOST_TEST(hash7 != hash4);
        BOOST_TEST(hash7 != hash5);
        BOOST_TEST(hash7 != hash6);
        BOOST_TEST(hash7 == hash7);
        BOOST_TEST(hash7 != hash8);
        BOOST_TEST(hash8 != hash1);
        BOOST_TEST(hash8 != hash2);
        BOOST_TEST(hash8 != hash3);
        BOOST_TEST(hash8 == hash4);
        BOOST_TEST(hash8 != hash5);
        BOOST_TEST(hash8 != hash6);
        BOOST_TEST(hash8 != hash7);
        BOOST_TEST(hash8 == hash8);
    }
    
    BOOST_AUTO_TEST_CASE(Hash160IndexOperatorTest)
    {
        Hash160 hash("97f6c464f25e32f444912fb4a9a53f3c62f2ef62");
        BOOST_TEST(hash[0] == 0x97);
        BOOST_TEST(hash[1] == 0xf6);
        BOOST_TEST(hash[2] == 0xc4);
        BOOST_TEST(hash[3] == 0x64);
        BOOST_TEST(hash[4] == 0xf2);
        BOOST_TEST(hash[5] == 0x5e);
        BOOST_TEST(hash[6] == 0x32);
        BOOST_TEST(hash[7] == 0xf4);
        BOOST_TEST(hash[8] == 0x44);
        BOOST_TEST(hash[9] == 0x91);
        BOOST_TEST(hash[10] == 0x2f);
        BOOST_TEST(hash[11] == 0xb4);
        BOOST_TEST(hash[12] == 0xa9);
        BOOST_TEST(hash[13] == 0xa5);
        BOOST_TEST(hash[14] == 0x3f);
        BOOST_TEST(hash[15] == 0x3c);
        BOOST_TEST(hash[16] == 0x62);
        BOOST_TEST(hash[17] == 0xf2);
        BOOST_TEST(hash[18] == 0xef);
        BOOST_TEST(hash[19] == 0x62);
    }
}
