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

#define BOOST_TEST_MODULE Serialization_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/util/serialization.h"
#include <cstdint>
#include <optional>

namespace conclave
{
    struct Thingy
    {
        uint8_t size;
        
        static Thingy deserialize(const std::vector<BYTE>& data, size_t& pos)
        {
            uint8_t size = data[pos];
            pos += size;
            return Thingy(size);
        }
        
        static Thingy deserialize(const std::vector<BYTE>& data)
        {
            size_t pos = 0;
            return deserialize(data, pos);
        }
        
        Thingy(uint8_t size)
            : size(size)
        {
        }
        
        Thingy(const std::vector<BYTE>& data, size_t& pos)
        {
            if (data.size() - pos <= 0) {
                size = 0;
            }
            size = data[pos];
            pos += size;
        }
        
        const std::vector<BYTE> serialize() const
        {
            std::vector<BYTE> serialization(size);
            for (size_t i = 0; i < size; i++) {
                serialization[i] = size;
            }
            return serialization;
        }
        
        bool operator==(const Thingy& other) const
        {
            return (size == other.size);
        }
        
        bool operator!=(const Thingy& other) const
        {
            return (size != other.size);
        }
    };
    
    const static std::vector<Thingy> THINGIES{
        Thingy(2),
        Thingy(3),
        Thingy(5)
    };
    const static std::vector<BYTE>
        THINGIES_SERIALIZED{0x03, 0x02, 0x02, 0x03, 0x03, 0x03, 0x05, 0x05, 0x05, 0x05, 0x05};
    const static std::optional<Thingy> OPTIONAL_PRESENT = Thingy(3);
    const static std::optional<Thingy> OPTIONAL_ABSENT = std::nullopt;
    const static std::vector<BYTE> OPTIONAL_PRESENT_SERIALIZED = std::vector<BYTE>{0x03, 0x03, 0x03, 0x03};
    const static std::vector<BYTE> OPTIONAL_ABSENT_SERIALIZED = std::vector<BYTE>{0x00};
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
            BOOST_TEST((serializeIntegral(static_cast<uint32_t>(0xffffffff)) ==
                        std::vector<BYTE>{0xff, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<uint32_t>(0xfffffffe)) ==
                        std::vector<BYTE>{0xfe, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeIntegral(static_cast<int32_t>(0)) == std::vector<BYTE>{0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int32_t>(1)) == std::vector<BYTE>{0x01, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<int32_t>(-0x7fffffff)) ==
                        std::vector<BYTE>{0x01, 0x00, 0x00, 0x80}));
            BOOST_TEST((serializeIntegral(static_cast<int32_t>(0x7fffffff)) ==
                        std::vector<BYTE>{0xff, 0xff, 0xff, 0x7f}));
            BOOST_TEST((serializeIntegral(static_cast<uint64_t>(0)) ==
                        std::vector<BYTE>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeIntegral(static_cast<uint64_t>(1)) ==
                        std::vector<BYTE>{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
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
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0x10000)) ==
                        std::vector<BYTE>{0xfe, 0x00, 0x00, 0x01, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x10000)) ==
                        std::vector<BYTE>{0xfe, 0x00, 0x00, 0x01, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint32_t>(0xffffffff)) ==
                        std::vector<BYTE>{0xfe, 0xff, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xffffffff)) ==
                        std::vector<BYTE>{0xfe, 0xff, 0xff, 0xff, 0xff}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0x100000000)) ==
                        std::vector<BYTE>{0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00}));
            BOOST_TEST((serializeVarInt(static_cast<uint64_t>(0xffffffffffffffff)) ==
                        std::vector<BYTE>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}));
        }
        
        BOOST_AUTO_TEST_CASE(SerializeOptionalTest)
        {
            std::vector<BYTE> optionalPresentSerialized = serializeOptionalObject(OPTIONAL_PRESENT);
            std::vector<BYTE> optionalAbsentSerialized = serializeOptionalObject(OPTIONAL_ABSENT);
            BOOST_TEST((optionalPresentSerialized == OPTIONAL_PRESENT_SERIALIZED));
            BOOST_TEST((optionalAbsentSerialized == OPTIONAL_ABSENT_SERIALIZED));
        }
        
        BOOST_AUTO_TEST_CASE(SerializeVectorOfObjectsTest)
        {
            BOOST_TEST((serializeVectorOfObjects(THINGIES) == THINGIES_SERIALIZED));
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
            int8_t i8 = deserializeIntegral<int8_t>(data, pos);
            BOOST_TEST((i8 == -44));
            // [pos] = 0xff
            uint16_t u16 = deserializeIntegral<uint16_t>(data, pos);
            BOOST_TEST((u16 == 17407));
            // [pos] = 0xcc
            int16_t i16 = deserializeIntegral<int16_t>(data, pos);
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
        
        BOOST_AUTO_TEST_CASE(DeserializeVarIntTest)
        {
            std::vector<BYTE> data{
                0x00,                                                  // 0
                0x01,                                                  // 1
                0xfb,                                                  // 251
                0xfc,                                                  // 252
                0xfd, 0xfd, 0x00,                                      // 253
                0xfd, 0xfe, 0x00,                                      // 254
                0xfd, 0xff, 0x00,                                      // 255
                0xfd, 0x00, 0x01,                                      // 256
                0xfd, 0xff, 0xff,                                      // 65535
                0xfe, 0x00, 0x00, 0x01, 0x00,                          // 65536
                0xfe, 0xff, 0xff, 0xff, 0xff,                          // 4294967295
                0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,  // 4294967296
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff   // 18446744073709551615
                
            };
            size_t pos = 0;
            uint64_t res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 0ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 1ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 251ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 252ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 253ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 254ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 255ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 256ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 65535ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 65536ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 4294967295ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 4294967296ULL));
            res = deserializeVarInt(data, pos);
            BOOST_TEST((res == 18446744073709551615ULL));
        }
        
        BOOST_AUTO_TEST_CASE(DeserializeVectorOfObjectsTest)
        {
            size_t pos = 0;
            BOOST_TEST((deserializeVectorOfObjects<Thingy>(THINGIES_SERIALIZED, pos) == THINGIES));
            BOOST_TEST((pos == THINGIES_SERIALIZED.size()));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}
