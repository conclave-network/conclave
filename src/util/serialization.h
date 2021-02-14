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
    inline void writeToByteVector(std::vector<BYTE>& dest, const std::vector<BYTE>& data, size_t& pos)
    {
        std::copy(data.begin(), data.end(), dest.begin() + pos);
        pos += data.size();
    }
    
    //
    // Serialization Functions
    //
    
    /**
     * Serialize an integral
     *
     * @tparam T - Integral type being serialized
     * @param value - The value being serialized
     * @return - Byte vector of the serialization
     */
    template<typename T>
    inline const std::vector<BYTE> serializeIntegral(const T value)
    {
        static_assert(std::is_integral<T>::value, "Integral type required");
        std::vector<BYTE> ret(sizeof(T));
        std::memcpy(&ret[0], &value, sizeof(T));
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
            ret.resize(ret.size() + objectSerialized.size());
            writeToByteVector(ret, objectSerialized, pos);
        }
        return ret;
    }
    
    //
    // Deserialization Functions
    //
    
    /**
     * Deserialize integral values
     *
     * @tparam T - Integral type being deserialized
     * @param data - Data stream
     * @param pos - Position within data stream where first byte appears
     * @return - Deserialized value
     */
    template<typename T>
    inline const T deserializeIntegral(const std::vector<BYTE>& data, size_t& pos)
    {
        static_assert(std::is_integral<T>::value, "Integral type required");
        T ret = *((T*) (&data[pos]));
        pos += sizeof(T);
        return ret;
    }
    
    /**
     * Deserialize a varint
     *
     * @param data - Data stream
     * @param pos - Position within data stream where first byte appears
     * @return - Deserialized value
     */
    inline const uint64_t deserializeVarInt(const std::vector<BYTE>& data, size_t& pos)
    {
        uint8_t size = data[pos++];
        uint64_t ret;
        if (size <= 0xfc) {
            ret = size;
        } else if (size == 0xfd) {
            ret = deserializeIntegral<uint16_t>(data, pos);
        } else if (size == 0xfe) {
            ret = deserializeIntegral<uint32_t>(data, pos);
        } else {
            ret = deserializeIntegral<uint64_t>(data, pos);
        }
        return ret;
    }
    
    /**
     * Deserialize an optional<T>, which is prefixed with a varint indicating the object's side
     *
     * @tparam T - Type of thing being deserialized - must have a constructor which takes a std::vector<BYTE> and size_t
     * @param data - Data stream
     * @param pos - Position within data stream where first byte appears
     * @return - Optional which is either a nullopt or the deserialized object
     */
    template<class T>
    inline const std::optional<T> deserializeOptionalObject(const std::vector<BYTE>& data, size_t& pos)
    {
        if (deserializeVarInt(data, pos) == 0) {
            return std::nullopt;
        } else {
            return T::deserialize(data, pos);
        }
    }
    
    /**
     * Serialize a vector of objects prepended with a varint
     *
     * @tparam T - Type which must have appropriate deserializing constructor
     * @param data - Data stream
     * @param pos - Position within data stream where first byte appears
     * @return
     */
    template<class T>
    inline const std::vector<T> deserializeVectorOfObjects(const std::vector<BYTE>& data, size_t& pos)
    {
        uint64_t nObjects = deserializeVarInt(data, pos);
        std::vector<T> objects;
        objects.reserve(nObjects);
        for (uint64_t i = 0; i < nObjects; i++) {
            objects.emplace_back(T::deserialize(data, pos));
        }
        return objects;
    }
}
