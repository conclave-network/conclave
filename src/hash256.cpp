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

#include "hash256.h"
#include "util/random.h"
#include "util/hex.h"
#include <bitcoin/system.hpp>
#include <algorithm>
#include <string>

namespace conclave
{
    //
    // Factories
    //
    
    Hash256 Hash256::digest(const std::vector<BYTE>& data)
    {
        return static_cast<Hash256>(bc::system::bitcoin_hash(data)).reversed();
    }
    
    Hash256 Hash256::digest(const std::string& str)
    {
        return digest(STRING_TO_BYTE_VECTOR(str));
    }
    
    Hash256 Hash256::digest(const char* cStr)
    {
        return digest(std::string(cStr));
    }
    
    Hash256 Hash256::deserialize(const std::vector<BYTE>& data, const size_t pos)
    {
        std::array<BYTE, LARGE_HASH_SIZE_BYTES> arr;
        std::reverse_copy(data.begin() + pos, data.begin() + pos + LARGE_HASH_SIZE_BYTES, arr.begin());
        return Hash256(arr);
    }
    
    //
    // Constructors
    //
    
    Hash256::Hash256()
        : data(makeRandomByteArray<LARGE_HASH_SIZE_BYTES>())
    {
    }
    
    Hash256::Hash256(const std::array<BYTE, LARGE_HASH_SIZE_BYTES>& data)
        : data(data)
    {
    }
    
    Hash256::Hash256(std::array<BYTE, LARGE_HASH_SIZE_BYTES>&& data)
        : data(std::move(data))
    {
    }
    
    Hash256::Hash256(const std::string& hex)
        : Hash256(hexStringToByteArray<LARGE_HASH_SIZE_BYTES>(hex))
    {
    }
    
    Hash256::Hash256(const BYTE* data)
        : data(bytePointerToByteArray<LARGE_HASH_SIZE_BYTES>(data))
    {
    }
    
    Hash256::Hash256(const char* hex)
        : Hash256(std::string(hex))
    {
    }
    
    Hash256::Hash256(const Hash256& other)
        : data(other.data)
    {
    }
    
    Hash256::Hash256(Hash256&& other)
        : data(std::move(other.data))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 Hash256::reversed() const
    {
        std::array<BYTE, LARGE_HASH_SIZE_BYTES> revData = data;
        std::reverse(revData.begin(), revData.end());
        return Hash256(revData);
    }
    
    std::array<BYTE, LARGE_HASH_SIZE_BYTES>::const_iterator Hash256::begin() const
    {
        return data.begin();
    }
    
    std::array<BYTE, LARGE_HASH_SIZE_BYTES>::const_iterator Hash256::end() const
    {
        return data.end();
    }
    
    const std::vector<BYTE> Hash256::serialize() const
    {
        return static_cast<std::vector<BYTE>>(this->reversed());
    }
    
    //
    // Conversions
    //
    
    Hash256::operator std::string() const
    {
        return byteArrayToHexString(data);
    }
    
    Hash256::operator std::array<BYTE, LARGE_HASH_SIZE_BYTES>() const
    {
        return data;
    }
    
    Hash256::operator std::vector<BYTE>() const
    {
        std::vector<BYTE> vector(LARGE_HASH_SIZE_BYTES);
        std::copy(data.begin(), data.end(), vector.begin());
        return vector;
    }
    
    Hash256::operator const unsigned char*() const
    {
        return data.data();
    }
    
    //
    // Operator Overloads
    //
    
    Hash256& Hash256::operator=(const Hash256& other)
    {
        data = other.data;
        return *this;
    }
    
    Hash256& Hash256::operator=(Hash256&& other)
    {
        data = std::move(other.data);
        return *this;
    }
    
    bool Hash256::operator==(const Hash256& other) const
    {
        return data == other.data;
    }
    
    bool Hash256::operator!=(const Hash256& other) const
    {
        return data != other.data;
    }
    
    BYTE& Hash256::operator[](const size_t index) const
    {
        return const_cast<BYTE&>(data[index]);
    }
    
    std::ostream& operator<<(std::ostream& os, const Hash256& hash256)
    {
        os << static_cast<std::string>(hash256);
        return os;
    }
}
