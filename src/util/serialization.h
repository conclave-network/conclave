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

#pragma once

#include "../conclave.h"
#include <cstdint>
#include <cstring>
#include <optional>
#include <type_traits>

/**
 * Quick-and-dirty serialization routines which are probably not very efficient.
 */

namespace conclave
{
    /**
     * Joins two byte vectors `a` and `b` to make `c`
     *
     * @param a - a
     * @param b - b
     * @return c - Result of `a | b`
     */
    inline const std::vector<BYTE> joinByteVectors(const std::vector<BYTE>& a, const std::vector<BYTE>& b)
    {
        size_t aSize = a.size();
        size_t bSize = b.size();
        std::vector<BYTE> c(aSize + bSize);
        std::memcpy(&c[0], &a[0], aSize);
        std::memcpy(&c[aSize], &b[0], bSize);
        return c;
    }
    
    /**
     * Writes `data` to position `pos` of `dest`, resizing `dest` if necessary. Works best if `dest` has already
     * `reserve()`'d enough bytes.
     *
     * @param dest - Destination
     * @param data - Data to be copied
     * @param pos - Whence within `dest` to begin writing
     */
    inline size_t writeToByteVector(std::vector<BYTE>& dest, const std::vector<BYTE>& data, const size_t pos = 0)
    {
        size_t dataSize = data.size();
        dest.resize(pos + dataSize);
        std::memcpy(&dest[pos], &data[0], dataSize);
        return dataSize;
    }
    
    //
    // Serialization Functions
    //
    
    /**
     * Serializes a `uint32_t` in machine-native format.
     *
     * @param value - Value to serialize
     * @return - Serialized form of `value`
     */
    inline const std::vector<BYTE> serializeU32(const uint32_t value)
    {
        std::vector<BYTE> ret(UINT32_SIZE);
        std::memcpy(&ret[0], &value, UINT32_SIZE);
        return ret;
    }
    
    /**
     * Serializes a `uint64_t` in machine-native format.
     * @param value - Value to serialize
     * @return - Serialized form of `value`
     */
    inline const std::vector<BYTE> serializeU64(const uint64_t value)
    {
        std::vector<BYTE> ret(UINT64_SIZE);
        std::memcpy(&ret[0], &value, UINT64_SIZE);
        return ret;
    }
    
    /**
     * Serializes a variable-length integer. `T` should be an integer type and is cast to its unsigned form.
     * See https://learnmeabitcoin.com/guide/varint for explanation of varints
     *
     * @tparam T - Type - typically a `uint8_t`, `uint16_t`, `uint32_t` or `uint64_t`
     * @param value - Value to serialize
     * @return - Serialized form of `value`
     */
    template<typename T>
    inline const std::vector<BYTE> serializeVarInt(const T value)
    {
        static_assert(std::is_unsigned<T>::value, "Unsigned type required");
        uint64_t uvalue = value;
        std::vector<BYTE> ret;
        if (uvalue <= 0xfcu) {
            ret.resize(UINT8_SIZE);
            std::memcpy(&ret[0], &uvalue, UINT8_SIZE);
        } else if (uvalue <= 0xffffu) {
            ret.resize(1 + UINT16_SIZE);
            ret[0] = 0xfd;
            std::memcpy(&ret[1], &uvalue, UINT16_SIZE);
        } else if (uvalue <= 0xffffffffu) {
            ret.resize(1 + UINT32_SIZE);
            ret[0] = 0xfe;
            std::memcpy(&ret[1], &uvalue, UINT32_SIZE);
        } else {
            ret.resize(1 + UINT64_SIZE);
            ret[0] = 0xff;
            std::memcpy(&ret[1], &uvalue, UINT64_SIZE);
        }
        return ret;
    }
    
    /**
     * We serialize an optional<T> by prepending the `T`'s serialization with a varint-encoding of its size. If the
     * thing is not present, then its size is zero. *This does imply that anything present in the optional must have a
     * non-zero size*.
     *
     * @tparam T - Type of thing being serialized - must have a serialize() method which returns a std::vector<BYTE>
     * @param optional - Optional which may or may not have a T
     * @return - Serialized form of optional<T>
     */
    template<class T>
    inline const std::vector<BYTE> serializeOptionalObject(const std::optional<T>& optional)
    {
        if (optional.has_value()) {
            const std::vector<BYTE> objectSerialized = optional->serialize();
            std::vector<BYTE> optionalSerialized = serializeVarInt(objectSerialized.size());
            return joinByteVectors(serializeVarInt(objectSerialized.size()), objectSerialized);
        } else {
            return serializeVarInt(0u);
        }
    }
    
    /**
     * Serialize a vector of serializable objects Bitcoin-style - prepend with a varint saying the number of objects
     * then the objects themselves.
     *
     * @tparam T - Type of thing being serialized - must have a serialize() method which returns a std::vector<BYTE>
     * @param objects - The objects to be serialized
     * @return - Serialized form of vector<T>
     */
    template<class T>
    inline const std::vector<BYTE> serializeVectorOfObjects(const std::vector<T>& objects)
    {
        size_t pos = 0;
        std::vector<BYTE> ret = serializeVarInt(objects.size());
        pos += ret.size();
        for (const T& object: objects) {
            const std::vector<BYTE> objectSerialized = object.serialize();
            writeToByteVector(ret, objectSerialized, pos);
            pos += objectSerialized.size();
        }
        return ret;
    }
}
