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

#define BOOST_TEST_MODULE Serialization_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/util/serialization.h"
#include <cstdint>
#include <optional>

namespace conclave
{
    BOOST_AUTO_TEST_SUITE(SerializationTestSuite)
        
        BOOST_AUTO_TEST_CASE(SerializeIntegralTest)
        {
            BOOST_TEST((serializeIntegral(static_cast<uint8_t>(0)) == std::vector<BYTE>{0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint8_t>(1)) == std::vector<BYTE>{0x01}));
            BOOST_TEST((serializeIntegral(static_cast<uint8_t>(0xff)) == std::vector<BYTE>{0xff}));
            BOOST_TEST((serializeIntegral(static_cast<uint8_t>(0xfe)) == std::vector<BYTE>{0xfe}));
            BOOST_TEST((serializeIntegral(static_cast<int8_t>(0)) == std::vector<BYTE>{0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int8_t>(1)) == std::vector<BYTE>{0x01}));
            BOOST_TEST((serializeIntegral(static_cast<int8_t>(-0x7f)) == std::vector<BYTE>{0x81}));
            BOOST_TEST((serializeIntegral(static_cast<int8_t>(0x7f)) == std::vector<BYTE>{0x7f}));
            BOOST_TEST((serializeIntegral(static_cast<uint16_t>(0)) == std::vector<BYTE>{0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint16_t>(1)) == std::vector<BYTE>{0x01, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint16_t>(0xffff)) == std::vector<BYTE>{0xff, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<uint16_t>(0xfffe)) == std::vector<BYTE>{0xfe, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<int16_t>(0)) == std::vector<BYTE>{0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int16_t>(1)) == std::vector<BYTE>{0x01, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int16_t>(-0x7fff)) == std::vector<BYTE>{0x01, 0x80}));
            BOOST_TEST((serializeIntegral(static_cast<int16_t>(0x7fff)) == std::vector<BYTE>{0xff, 0x7f}));
            BOOST_TEST((serializeIntegral(static_cast<uint32_t>(0)) == std::vector<BYTE>{0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint32_t>(1)) == std::vector<BYTE>{0x01, 0x00, 0x00, 0x00}));
            BOOST_TEST(
                (serializeIntegral(static_cast<uint32_t>(0xffffffff)) == std::vector<BYTE>{0xff, 0xff, 0xff, 0xff}));
            BOOST_TEST(
                (serializeIntegral(static_cast<uint32_t>(0xfffffffe)) == std::vector<BYTE>{0xfe, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<int32_t>(0)) == std::vector<BYTE>{0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int32_t>(1)) == std::vector<BYTE>{0x01, 0x00, 0x00, 0x00}));
            BOOST_TEST(
                (serializeIntegral(static_cast<int32_t>(-0x7fffffff)) == std::vector<BYTE>{0x01, 0x00, 0x00, 0x80}));
            BOOST_TEST(
                (serializeIntegral(static_cast<int32_t>(0x7fffffff)) == std::vector<BYTE>{0xff, 0xff, 0xff, 0x7f}));
            BOOST_TEST((serializeIntegral(static_cast<uint64_t>(0)) == std::vector<BYTE>{0x00, 0x00, 0x00, 0x00,
                                                                                         0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint64_t>(1)) == std::vector<BYTE>{0x01, 0x00, 0x00, 0x00,
                                                                                         0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint64_t>(0xffffffffffffffff)) ==
                        std::vector<BYTE>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<uint64_t>(0xfffffffffffffffe)) ==
                        std::vector<BYTE>{0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<int64_t>(0)) ==
                        std::vector<BYTE>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int64_t>(1)) ==
                        std::vector<BYTE>{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int64_t>(-0x7fffffffffffffff)) ==
                        std::vector<BYTE>{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80}));
            BOOST_TEST((serializeIntegral(static_cast<int64_t>(0x7fffffffffffffff)) ==
                        std::vector<BYTE>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f}));
        }
        
        BOOST_AUTO_TEST_CASE(SerializeVarIntTest)
        {
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0x00)) == std::vector<BYTE>{0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0x00)) == std::vector<BYTE>{0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0x00)) == std::vector<BYTE>{0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x00)) == std::vector<BYTE>{0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0x01)) == std::vector<BYTE>{0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0x01)) == std::vector<BYTE>{0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0x01)) == std::vector<BYTE>{0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x01)) == std::vector<BYTE>{0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0xfb)) == std::vector<BYTE>{0xfb}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0xfb)) == std::vector<BYTE>{0xfb}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xfb)) == std::vector<BYTE>{0xfb}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xfb)) == std::vector<BYTE>{0xfb}));
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0xfc)) == std::vector<BYTE>{0xfc}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0xfc)) == std::vector<BYTE>{0xfc}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xfc)) == std::vector<BYTE>{0xfc}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xfc)) == std::vector<BYTE>{0xfc}));
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0xfd)) == std::vector<BYTE>{0xfd, 0xfd, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0xfd)) == std::vector<BYTE>{0xfd, 0xfd, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xfd)) == std::vector<BYTE>{0xfd, 0xfd, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xfd)) == std::vector<BYTE>{0xfd, 0xfd, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0xfe)) == std::vector<BYTE>{0xfd, 0xfe, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0xfe)) == std::vector<BYTE>{0xfd, 0xfe, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xfe)) == std::vector<BYTE>{0xfd, 0xfe, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xfe)) == std::vector<BYTE>{0xfd, 0xfe, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint8_t>(0xff)) == std::vector<BYTE>{0xfd, 0xff, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0xff)) == std::vector<BYTE>{0xfd, 0xff, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xff)) == std::vector<BYTE>{0xfd, 0xff, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xff)) == std::vector<BYTE>{0xfd, 0xff, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0x100)) == std::vector<BYTE>{0xfd, 0x00, 0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0x100)) == std::vector<BYTE>{0xfd, 0x00, 0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x100)) == std::vector<BYTE>{0xfd, 0x00, 0x01}));
            BOOST_TEST((serializeVarInt(static_cast<uint16_t>(0xffff)) == std::vector<BYTE>{0xfd, 0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xffff)) == std::vector<BYTE>{0xfd, 0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xffff)) == std::vector<BYTE>{0xfd, 0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0x10000)) == std::vector<BYTE>{0xfe, 0x00, 0x00, 0x01,
                                                                                             0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x10000)) == std::vector<BYTE>{0xfe, 0x00, 0x00, 0x01,
                                                                                             0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xffffffff)) == std::vector<BYTE>{0xfe, 0xff, 0xff,
                                                                                                0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xffffffff)) == std::vector<BYTE>{0xfe, 0xff, 0xff,
                                                                                                0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x100000000)) == std::vector<BYTE>{0xff, 0x00, 0x00,
                                                                                                 0x00, 0x00, 0x01, 0x00,
                                                                                                 0x00, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xffffffffffffffff)) == std::vector<BYTE>{0xff, 0xff,
                                                                                                        0xff, 0xff,
                                                                                                        0xff, 0xff,
                                                                                                        0xff, 0xff,
                                                                                                        0xff}));
        }
        
        BOOST_AUTO_TEST_CASE(SerializeOptionalTest)
        {
            struct Foo
            {
                const std::vector<BYTE> serialize() const
                {
                    return std::vector<BYTE>{'f', 'o', 'o'};
                }
            };
            std::optional<Foo> present = Foo();
            std::optional<Foo> absent = std::nullopt;
            std::vector<BYTE> presentSerialized = serializeOptionalObject(present);
            std::vector<BYTE> absentSerialized = serializeOptionalObject(absent);
            BOOST_TEST((presentSerialized == std::vector<BYTE>{0x03, 'f', 'o', 'o'}));
            BOOST_TEST((absentSerialized == std::vector<BYTE>{0x00}));
        }
        
        BOOST_AUTO_TEST_CASE(SerializeVectorOfObjectsTest)
        {
            struct Thingy
            {
                size_t size;
                
                const std::vector<BYTE> serialize() const
                {
                    std::vector<BYTE> serialization(size);
                    for (size_t i = 0; i < size; i++) {
                        serialization[i] = size;
                    }
                    return serialization;
                }
            };
            std::vector<Thingy> thingies{
                Thingy{2},
                Thingy{3},
                Thingy{5}
            };
            std::vector<BYTE> thingiesSerialized = serializeVectorOfObjects(thingies);
            BOOST_TEST((thingiesSerialized ==
                        std::vector<BYTE>{0x03, 0x02, 0x02, 0x03, 0x03, 0x03, 0x05, 0x05, 0x05, 0x05, 0x05}));
        }
        
        BOOST_AUTO_TEST_CASE(DeserializeIntegralTest)
        {
            std::vector<BYTE> data{
                0xd8, 0xd4, 0xff, 0x43, 0xcc, 0x8b, 0xbb, 0x7e, 0x97, 0x09, 0x53
            };
            size_t pos = 0;
            // [pos] = 0xd8
            uint8_t u8 = deserializeIntegral<uint8_t>(data, pos);
            BOOST_TEST((u8 == 216));
            // [pos] = 0xd4
            int8_t i8 = deserializeIntegral<uint8_t>(data, pos);
            BOOST_TEST((i8 == -44));
            // [pos] = 0xff
            uint16_t u16 = deserializeIntegral<uint16_t>(data, pos);
            BOOST_TEST((u16 == 17407));
            // [pos] = 0xcc
            int16_t i16 = deserializeIntegral<uint16_t>(data, pos);
            BOOST_TEST((i16 == -29748));
            // [pos] = 0xbb
            uint32_t u32 = deserializeIntegral<uint32_t>(data, pos);
            BOOST_TEST((u32 == 160923323));
            pos -= 9;
            // [pos] = 0xd4
            int32_t i32 = deserializeIntegral<int32_t>(data, pos);
            BOOST_TEST((i32 == -867958828));
            pos -= 2;
            // [pos] = 0x43
            uint64_t u64 = deserializeIntegral<uint64_t>(data, pos);
            BOOST_TEST((u64 == 5983480150506589251));
            pos -= 10;
            // [pos] = 0xd4
            int64_t i64 = deserializeIntegral<int64_t>(data, pos);
            BOOST_TEST((i64 == -7530375317815033900));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
