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

namespace conclave
{
    using namespace libbitcoin::system;
    
    inline const static Hash256 getY(const Hash256& x, const bool odd)
    {
        ec_compressed ecc;
        ec_uncompressed ecu;
        decompress(ecu, ecc);
        return x;
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
        : x(x), y(std::move(getY(x, odd)))
    {
    }
    
    PublicKey::PublicKey(Hash256&& x, const bool odd)
        : x(std::move(x)), y(std::move(getY(x, odd)))
    {
    }
    
    PublicKey::PublicKey(const std::array <BYTE, UNCOMPRESSED_PUBKEY_SIZE_BYTES>& data)
        : x(&data[1]), y(&data[1 + EC_POINT_SIZE_BYTES])
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
    
    PublicKey::operator std::string() const
    {
        return (std::string) x + (std::string) y;
    }
    
    std::ostream& operator<<(std::ostream& os, const PublicKey& publicKey)
    {
        os << static_cast<std::string>(publicKey);
        return os;
    }
}
