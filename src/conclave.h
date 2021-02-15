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

#include "util/hex.h"
#include <array>
#include <vector>
#include <algorithm>
#include <ostream>
#include <stdexcept>

typedef unsigned char BYTE;
#define BYTE_ARRAY_TO_VECTOR(byteArray) std::vector<BYTE>(byteArray.begin(), byteArray.end())
#define BYTE_VECTOR_TO_ARRAY(byteVector, size) bytePointerToByteArray<size>(byteVector.data())
#define STRING_TO_BYTE_VECTOR(str) std::vector<BYTE>(str.begin(), str.end())
#define BYTE_VECTOR_TO_HEX(byteVector) byteVectorToHexString(byteVector)
#define HEX_TO_BYTE_VECTOR(hex) hexStringToByteVector(hex)
// Sizes of things. In bytes.
const static size_t UINT8_SIZE_BYTES = 1;
const static size_t UINT16_SIZE_BYTES = 2;
const static size_t UINT32_SIZE_BYTES = 4;
const static size_t UINT64_SIZE_BYTES = 8;
const static size_t SECP256K1_SCALAR_SIZE_BYTES = 32;
const static size_t SECP256K1_PRIVKEY_SIZE_BYTES = SECP256K1_SCALAR_SIZE_BYTES;
const static size_t SECP256K1_PUBKEY_X_ONLY_SIZE_BYTES = 32;
const static size_t SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES = 33;
const static size_t SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES = 65;
const static size_t SMALL_HASH_SIZE_BYTES = 20;
const static size_t LARGE_HASH_SIZE_BYTES = 32;
const static size_t ECDSA_SIGNATURE_SIZE_BYTES = 64; // Not DER
const static size_t ECDSA_SIGNATURE_DER_MAX_SIZE_BYTES = 72;

template<size_t size>
inline const std::array<BYTE, size> bytePointerToByteArray(const BYTE* ptr)
{
    std::array<BYTE, size> arr;
    std::copy(ptr, ptr + size, arr.begin());
    return arr;
}

template<size_t size>
inline const std::array<BYTE, size> bytePointerToByteArrayReversed(const BYTE* ptr)
{
    std::array<BYTE, size> arr;
    std::reverse_copy(ptr, ptr + size, arr.begin());
    return arr;
}

template<size_t size>
std::ostream& operator<<(std::ostream& os, const std::array<BYTE, size>& ba)
{
    os << byteArrayToHexString(ba);
    return os;
}

#define CONCLAVE_ASSERT(cond, message) if (!(cond)) throw std::runtime_error(message)
