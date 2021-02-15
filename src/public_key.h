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

#include "conclave.h"
#include "hash256.h"
#include "hash160.h"
#include <array>
#include <string>

namespace conclave
{
    class PublicKey
    {
        public:
        // Factories
        static PublicKey deserialize(const std::vector<BYTE>&, size_t&);
        static PublicKey deserialize(const std::vector<BYTE>&);
        // Constructors
        PublicKey(const PublicKey&);
        PublicKey(PublicKey&&) noexcept;
        PublicKey(Hash256, Hash256);
        PublicKey(Hash256, bool);
        explicit PublicKey(const std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>&);
        explicit PublicKey(const std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>&);
        explicit PublicKey(const std::string&);
        explicit PublicKey(const char*);
        // Public Functions
        [[nodiscard]] std::string asHexStringUncompressed() const;
        [[nodiscard]] std::string asHexStringCompressed() const;
        [[nodiscard]] Hash160 getHash160Uncompressed() const;
        [[nodiscard]] Hash160 getHash160Compressed() const;
        [[nodiscard]] Hash256 getHash256Uncompressed() const;
        [[nodiscard]] Hash256 getHash256Compressed() const;
        [[nodiscard]] std::vector<BYTE> serialize() const;
        [[nodiscard]] bool yIsEven() const;
        // Conversions
        explicit operator std::array<BYTE, SECP256K1_PUBKEY_COMPRESSED_SIZE_BYTES>() const;
        explicit operator std::array<BYTE, SECP256K1_PUBKEY_UNCOMPRESSED_SIZE_BYTES>() const;
        explicit operator std::vector<BYTE>() const;
        explicit operator std::string() const;
        // Operator Overloads
        PublicKey& operator=(const PublicKey&);
        PublicKey& operator=(PublicKey&&);
        bool operator==(const PublicKey&) const;
        bool operator!=(const PublicKey&) const;
        friend std::ostream& operator<<(std::ostream&, const PublicKey&);
        private:
        // Properties
        Hash256 x;
        Hash256 y;
    };
}
