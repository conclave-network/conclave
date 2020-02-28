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

#include "hash160.h"
#include "util/random.h"
#include "util/hex.h"
#include <algorithm>
#include <string>

namespace conclave
{
    Hash160::Hash160()
        : data(makeRandomByteArray<SMALL_HASH_SIZE_BYTES>())
    {
    }
    
    Hash160::Hash160(const Hash160& other)
        : data(other.data)
    {
    }
    
    Hash160::Hash160(Hash160&& other)
        : data(std::move(other.data))
    {
    }
    
    Hash160::Hash160(const std::array<BYTE, SMALL_HASH_SIZE_BYTES>& data)
        : data(data)
    {
    }
    
    Hash160::Hash160(const std::string& hex)
        : Hash160(hexStringToByteArray<SMALL_HASH_SIZE_BYTES>(hex))
    {
    }
    
    Hash160::Hash160(const char* hex)
        : Hash160(std::string(hex))
    {
    }
    
    const Hash160 Hash160::reversed() const
    {
        std::array<BYTE, SMALL_HASH_SIZE_BYTES> revData = data;
        std::reverse(revData.begin(), revData.end());
        return Hash160(revData);
    }
    
    std::array<BYTE, SMALL_HASH_SIZE_BYTES>::const_iterator Hash160::begin() const
    {
        return data.begin();
    }
    
    std::array<BYTE, SMALL_HASH_SIZE_BYTES>::const_iterator Hash160::end() const
    {
        return data.end();
    }
    
    Hash160::operator std::string() const
    {
        return byteArrayToHexString(data);
    }
    
    Hash160::operator std::array<BYTE, SMALL_HASH_SIZE_BYTES>() const
    {
        return data;
    }
    
    BYTE& Hash160::operator[](const size_t index) const
    {
        return const_cast<BYTE&>(data[index]);
    }
    
    bool Hash160::operator==(const Hash160& other) const
    {
        return data == other.data;
    }
    
    bool Hash160::operator!=(const Hash160& other) const
    {
        return data != other.data;
    }
    
    std::ostream& operator<<(std::ostream& os, const Hash160& hash160)
    {
        os << static_cast<std::string>(hash160);
        return os;
    }
}
