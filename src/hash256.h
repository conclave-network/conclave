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
#include <bitcoin/system.hpp>
#include <array>

namespace conclave
{
    class Hash256 final
    {
        public:
        // Factories
        static Hash256 digest(const std::vector<BYTE>&);
        // Constructors
        Hash256();
        Hash256(const Hash256& other);
        Hash256(Hash256&& other);
        Hash256(const std::array<BYTE, LARGE_HASH_SIZE_BYTES>&);
        Hash256(std::array<BYTE, LARGE_HASH_SIZE_BYTES>&&);
        Hash256(const BYTE*);
        Hash256(const std::string&);
        Hash256(const char*);
        // Public Functions
        const Hash256 reversed() const;
        std::array<BYTE, LARGE_HASH_SIZE_BYTES>::const_iterator begin() const;
        std::array<BYTE, LARGE_HASH_SIZE_BYTES>::const_iterator end() const;
        const std::vector<BYTE> serialize() const;
        // Operators
        operator std::string() const;
        operator std::array<BYTE, LARGE_HASH_SIZE_BYTES>() const;
        operator std::vector<BYTE>() const;
        operator const unsigned char*() const;
        BYTE& operator[](const size_t) const;
        Hash256& operator=(const Hash256&) = delete;
        Hash256& operator=(Hash256&&) = delete;
        bool operator==(const Hash256&) const;
        bool operator!=(const Hash256&) const;
        friend std::ostream& operator<<(std::ostream&, const Hash256&);
        private:
        // Properties
        std::array<BYTE, LARGE_HASH_SIZE_BYTES> data;
    };
}
