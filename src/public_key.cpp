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

#include "public_key.h"
#include "util/serialization.h"
#include <bitcoin/system/math/elliptic_curve.hpp>
#include <algorithm>
#include <utility>

namespace conclave
{
    using namespace libbitcoin::system;
    
    ///
    /// Helpers
    ///
    
    inline const static Hash256 getY(const Hash256& x, const bool odd)
    {
        ec_compressed ecc;
        ec_uncompressed ecu;
        ecc[0] = (odd & 1u) + 2u;
        std::copy(x.begin(), x.end(), &ecc[1]);
        decompress(ecu, ecc);
        return Hash256(&ecu[1 + SECP256K1_SCALAR_SIZE_BYTES]);
    }
    
    ///
    /// Factories
    ///
    
    PublicKey PublicKey::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const uint8_t leadingByte = deserializeIntegral<uint8_t>(data, pos);
        if (leadingByte < 0x04) {
            // Compressed
            const Hash256 x = Hash256::deserialize(data, pos).reversed();
            return PublicKey(std::move(x), leadingByte == 0x03);
        } else {
            // Uncompressed
            const Hash256 x = Hash256::deserialize(data, pos).reversed();
            const Hash256 y = Hash256::deserialize(data, pos).reversed();
            return PublicKey(std::move(x), std::move(y));
        }
    }
    
    PublicKey PublicKey::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    ///
    /// Constructors
    ///
    
    PublicKey::PublicKey(const PublicKey& other) = default;
    
    PublicKey::PublicKey(PublicKey&& other) noexcept
        : x(std::move(other.x)), y(std::move(other.y))
    {
    }
    
    PublicKey::PublicKey(Hash256 x, Hash256 y)
        : x(std::move(x)), y(std::move(y))
    {
    }
    
    PublicKey::PublicKey(Hash256 x, bool odd)
        : x(std::move(x)), y(getY(x, odd))
    {
    }
    
    PublicKey::PublicKey(const std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>& data)
        : x(&data[1]), y(&data[1 + SECP256K1_SCALAR_SIZE_BYTES])
    {
    }
    
    PublicKey::PublicKey(const std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>& data)
        : x(&data[1]), y(getY(Hash256(&data[1]), data[0] == 0x03))
    {
    }
    
    PublicKey::PublicKey(const std::string& hex)
        : PublicKey(hexStringToByteArray<SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>(hex))
    {
    }
    
    PublicKey::PublicKey(const char* hex)
        : PublicKey(std::string(hex))
    {
    }
    
    ///
    /// Public Functions
    ///
    
    std::string PublicKey::asHexStringUncompressed() const
    {
        return byteArrayToHexString(static_cast<std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>>(*this));
    }
    
    std::string PublicKey::asHexStringCompressed() const
    {
        return byteArrayToHexString(static_cast<std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>>(*this));
    }
    
    Hash160 PublicKey::getHash160Uncompressed() const
    {
        return bitcoin_short_hash(static_cast<std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>>(*this));
    }
    
    Hash160 PublicKey::getHash160Compressed() const
    {
        return bitcoin_short_hash(static_cast<std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>>(*this));
    }
    
    Hash256 PublicKey::getHash256Uncompressed() const
    {
        return static_cast<Hash256>(bitcoin_hash(
            static_cast<std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>>(*this)))
            .reversed();
    }
    
    Hash256 PublicKey::getHash256Compressed() const
    {
        return static_cast<Hash256>(bitcoin_hash(
            static_cast<std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>>(*this)))
            .reversed();
    }
    
    std::vector<BYTE> PublicKey::serialize() const
    {
        return static_cast<std::vector<BYTE>>(*this);
    }
    
    bool PublicKey::yIsEven() const
    {
        return y[sizeof(y) - 1] % 2;
    }
    
    ///
    /// Conversions
    ///
    
    PublicKey::operator std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>() const
    {
        std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES> arr{};
        arr[0] = 0x04;
        std::copy(x.begin(), x.end(), arr.begin() + 1);
        std::copy(y.begin(), y.end(), arr.begin() + 1 + SECP256K1_SCALAR_SIZE_BYTES);
        return arr;
    }
    
    PublicKey::operator std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>() const
    {
        std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES> arr{};
        arr[0] = (y[SECP256K1_SCALAR_SIZE_BYTES - 1] & 1u) + 2u;
        std::copy(x.begin(), x.end(), arr.begin() + 1);
        return arr;
    }
    
    PublicKey::operator std::vector<BYTE>() const
    {
        // Represent as a compressed pubkey by default
        const auto array = static_cast<std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>>(*this);
        std::vector<BYTE> vector(SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES);
        std::copy(array.begin(), array.end(), vector.begin());
        return vector;
    }
    
    PublicKey::operator std::string() const
    {
        // Stringify as a compressed pubkey by default
        return asHexStringCompressed();
    }
    
    ///
    /// Operator Overloads
    ///
    
    PublicKey& PublicKey::operator=(const PublicKey& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    
    PublicKey& PublicKey::operator=(PublicKey&& other)
    {
        x = std::move(other.x);
        y = std::move(other.y);
        return *this;
    }
    
    bool PublicKey::operator==(const PublicKey& other) const
    {
        return (x == other.x) && (y == other.y);
    }
    
    bool PublicKey::operator!=(const PublicKey& other) const
    {
        return (x != other.x) || (y != other.y);
    }
    
    std::ostream& operator<<(std::ostream& os, const PublicKey& publicKey)
    {
        os << static_cast<std::string>(publicKey);
        return os;
    }
}
