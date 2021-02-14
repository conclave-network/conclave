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
#include <string>
#include <vector>
#include <array>

typedef unsigned char BYTE;
static const char HEX_CHARACTERS[] = "0123456789abcdef";
#define HEX_NIBBLE_TO_VAL(hex_nibble) \
        if ('0' <= hex_nibble && hex_nibble <= '9') { \
            hex_nibble -= '0'; \
        } else if ('a' <= hex_nibble && hex_nibble <= 'f') { \
            hex_nibble -= 'a' - 10; \
        } else  { \
            hex_nibble -= 'A' - 10; \
        } \

#define HEX_STRING_TO_BYTE_CONTAINER(hexString) \
    const BYTE* chars = (BYTE*) hexString.data(); \
    BYTE* buf = res.data(); \
    for (size_t i = 0; i < size; i++) { \
        BYTE hi = chars[i * 2];\
        BYTE lo = chars[i * 2 + 1]; \
        HEX_NIBBLE_TO_VAL(hi) \
        HEX_NIBBLE_TO_VAL(lo) \
        buf[i] = (((BYTE) hi) << 4u) + (BYTE) lo; \
    } \
    return res; \

#define BYTE_CONTAINER_TO_HEX_STRING(byteContainer) \
    size_t nBytes = byteContainer.size(); \
    const BYTE* bytes = byteContainer.data(); \
    std::string ret(nBytes * 2, 0); \
    char* buf = ret.data(); \
    for (size_t i = 0; i < nBytes; i++) { \
        *buf++ = HEX_CHARACTERS[bytes[i] >> 4]; \
        *buf++ = HEX_CHARACTERS[bytes[i] & 0x0F]; \
    } \
    return ret; \


/**
 * Convert a hex string to a byte vector
 * @param hexString - Input hex string
 * @return
 */
inline const std::vector<BYTE> hexStringToByteVector(const std::string& hexString)
{
    size_t size = hexString.length() / 2;
    std::vector<BYTE> res(size);
    HEX_STRING_TO_BYTE_CONTAINER(hexString)
}

/**
 * Convert a hex string to a byte array
 * @param hexString - Input hex string
 * @return
 */
template<size_t size>
inline const std::array<BYTE, size> hexStringToByteArray(const std::string& hexString)
{
    std::array<BYTE, size> res;
    HEX_STRING_TO_BYTE_CONTAINER(hexString)
}

/**
 * Convert a byte vector to a hex string
 * @param byteVector - Input byte vector
 * @return
 */
inline const std::string byteVectorToHexString(const std::vector<BYTE>& byteVector)
{
    BYTE_CONTAINER_TO_HEX_STRING(byteVector)
}

/**
 * Convert a byte array to a hex string
 * @param byteArray - Input byte array
 * @return
 */
template<size_t size>
inline const std::string byteArrayToHexString(const std::array<BYTE, size>& byteArray)
{
    BYTE_CONTAINER_TO_HEX_STRING(byteArray)
}
