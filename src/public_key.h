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
        // Constructors
        PublicKey(const PublicKey&);
        PublicKey(PublicKey&&);
        PublicKey(const Hash256&, const Hash256&);
        PublicKey(Hash256&&, Hash256&&);
        PublicKey(const Hash256&, const bool);
        PublicKey(Hash256&&, const bool);
        PublicKey(const std::array<BYTE, UNCOMPRESSED_PUBKEY_SIZE_BYTES>&);
        PublicKey(const std::array<BYTE, COMPRESSED_PUBKEY_SIZE_BYTES>&);
        PublicKey(const std::string&);
        PublicKey(const char*);
        // Public functions
        const std::string asHexStringUncompressed() const;
        const std::string asHexStringCompressed() const;
        const Hash160 getHash160Uncompressed() const;
        const Hash160 getHash160Compressed() const;
        const Hash256 getHash256Uncompressed() const;
        const Hash256 getHash256Compressed() const;
        const std::vector<BYTE> serialize() const;
        // Operators
        operator std::array<BYTE, UNCOMPRESSED_PUBKEY_SIZE_BYTES>() const;
        operator std::array<BYTE, COMPRESSED_PUBKEY_SIZE_BYTES>() const;
        operator std::string() const;
        bool operator==(const PublicKey&) const;
        bool operator!=(const PublicKey&) const;
        friend std::ostream& operator<<(std::ostream&, const PublicKey&);
        private:
        // Properties
        const Hash256 x;
        const Hash256 y;
    };
}
