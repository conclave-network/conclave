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

#include "public_key.h"
#include <bitcoin/system/math/elliptic_curve.hpp>
#include <algorithm>

namespace conclave
{
    using namespace libbitcoin::system;
    
    inline const static Hash256 getY(const Hash256& x, const bool odd)
    {
        ec_compressed ecc;
        ec_uncompressed ecu;
        ecc[0] = (odd & 1u) + 2u;
        std::copy(x.begin(), x.end(), &ecc[1]);
        decompress(ecu, ecc);
        return Hash256(&ecu[1 + EC_POINT_SIZE_BYTES]);
    }
    
    PublicKey::PublicKey(const Hash256& x, const Hash256& y)
        : x(x), y(y)
    {
    }
    
    PublicKey::PublicKey(Hash256&& x, Hash256&& y)
        : x(std::move(x)), y(std::move(y))
    {
    }
    
    PublicKey::PublicKey(const Hash256& x, const bool odd)
        : x(x), y(getY(x, odd))
    {
    }
    
    PublicKey::PublicKey(Hash256&& x, const bool odd)
        : x(std::move(x)), y(getY(x, odd))
    {
    }
    
    PublicKey::PublicKey(const std::array<BYTE, UNCOMPRESSED_PUBKEY_SIZE_BYTES>& data)
        : x(&data[1]), y(&data[1 + EC_POINT_SIZE_BYTES])
    {
    }
    
    PublicKey::PublicKey(const std::array<BYTE, COMPRESSED_PUBKEY_SIZE_BYTES>& data)
        : x(&data[1]), y(getY(Hash256(&data[1]), data[0] == 0x03))
    {
    }
    
    bool PublicKey::operator==(const PublicKey& other) const
    {
        return (x == other.x) && (y == other.y);
    }
    
    bool PublicKey::operator!=(const PublicKey& other) const
    {
        return (x != other.x) || (y != other.y);
    }
    
    PublicKey::operator std::array<BYTE, UNCOMPRESSED_PUBKEY_SIZE_BYTES>() const
    {
        std::array<BYTE, UNCOMPRESSED_PUBKEY_SIZE_BYTES> arr;
        arr[0] = 0x04;
        std::copy(x.begin(), x.end(), arr.begin() + 1);
        std::copy(y.begin(), y.end(), arr.begin() + 1 + EC_POINT_SIZE_BYTES);
        return arr;
    }
    
    PublicKey::operator std::array<BYTE, COMPRESSED_PUBKEY_SIZE_BYTES>() const
    {
        std::array<BYTE, COMPRESSED_PUBKEY_SIZE_BYTES> arr;
        arr[0] = (y[EC_POINT_SIZE_BYTES - 1] & 1u) + 2u;
        std::copy(x.begin(), x.end(), arr.begin() + 1);
        return arr;
    }
    
    PublicKey::operator std::string() const
    {
        // Stringify as a compressed pubkey by default
        return byteArrayToHexString(static_cast<std::array<BYTE, COMPRESSED_PUBKEY_SIZE_BYTES>>(*this));
    }
    
    std::ostream& operator<<(std::ostream& os, const PublicKey& publicKey)
    {
        os << static_cast<std::string>(publicKey);
        return os;
    }
}
