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
#include <array>
#include <string>
#include <vector>

namespace conclave
{
    class Hash160 final
    {
        public:
        // Factories
        static Hash160 digest(const std::vector<BYTE>&);
        static Hash160 digest(const std::string&);
        static Hash160 digest(const char*);
        static Hash160 deserialize(const std::vector<BYTE>&, size_t&);
        // Constructors
        Hash160();
        Hash160(const std::array<BYTE, SMALL_HASH_SIZE_BYTES>&);
        Hash160(std::array<BYTE, SMALL_HASH_SIZE_BYTES>&&);
        Hash160(const std::vector<BYTE>&);
        Hash160(const std::string&);
        Hash160(const BYTE*);
        Hash160(const char*);
        Hash160(const Hash160& other);
        Hash160(Hash160&& other);
        // Public Functions
        const Hash160 reversed() const;
        std::array<BYTE, SMALL_HASH_SIZE_BYTES>::const_iterator begin() const;
        std::array<BYTE, SMALL_HASH_SIZE_BYTES>::const_iterator end() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        operator std::string() const;
        operator std::array<BYTE, SMALL_HASH_SIZE_BYTES>() const;
        operator std::vector<BYTE>() const;
        operator const unsigned char*() const;
        // Operator Overloads
        Hash160& operator=(const Hash160&);
        Hash160& operator=(Hash160&&);
        bool operator==(const Hash160&) const;
        bool operator!=(const Hash160&) const;
        BYTE& operator[](const size_t) const;
        friend std::ostream& operator<<(std::ostream&, const Hash160&);
        friend Hash160 operator^(const Hash160&, const Hash160&);
        private:
        // Properties
        std::array<BYTE, SMALL_HASH_SIZE_BYTES> data;
    };
}
