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

#include "util/hex.h"
#include <array>
#include <vector>
#include <ostream>

typedef unsigned char BYTE;
#define BYTE_ARRAY_TO_VECTOR(byteArray) std::vector<BYTE>(byteArray.begin(), byteArray.end())
#define BYTE_VECTOR_TO_HEX(byteVector) byteVectorToHexString(byteVector)
#define HEX_TO_BYTE_VECTOR(hex) hexStringToByteVector(hex)
// Sizes of things
const static size_t UINT8_SIZE = 1;
const static size_t UINT16_SIZE = 2;
const static size_t UINT32_SIZE = 4;
const static size_t UINT64_SIZE = 8;
const static size_t PRIVKEY_SIZE_BYTES = 32;
const static size_t EC_POINT_SIZE_BYTES = 32;
const static size_t COMPRESSED_PUBKEY_SIZE_BYTES = 33;
const static size_t UNCOMPRESSED_PUBKEY_SIZE_BYTES = 65;
const static size_t SMALL_HASH_SIZE_BYTES = 20;
const static size_t LARGE_HASH_SIZE_BYTES = 32;

template<size_t size>
inline const std::array<BYTE, size> bytePointerToByteArray(const BYTE* ptr)
{
    std::array<BYTE, size> arr;
    std::copy(ptr, ptr + size, arr.begin());
    return arr;
}

template<size_t size>
std::ostream& operator<<(std::ostream& os, const std::array<BYTE, size>& ba)
{
    os << byteArrayToHexString(ba);
    return os;
}

