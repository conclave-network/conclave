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
#include <array>

namespace conclave
{
    class Hash160 final
    {
        public:
        // Constructors
        Hash160();
        Hash160(const Hash160& other);
        Hash160(Hash160&& other);
        Hash160(const std::array<BYTE, SMALL_HASH_SIZE_BYTES>&);
        Hash160(const std::string&);
        Hash160(const char*);
        // Public Functions
        const Hash160 reversed() const;
        std::array<BYTE, SMALL_HASH_SIZE_BYTES>::const_iterator begin() const;
        std::array<BYTE, SMALL_HASH_SIZE_BYTES>::const_iterator end() const;
        // Operators
        operator std::string() const;
        operator std::array<BYTE, SMALL_HASH_SIZE_BYTES>() const;
        BYTE& operator[](const size_t) const;
        Hash160& operator=(const Hash160&) = delete;
        Hash160& operator=(Hash160&&) = delete;
        bool operator==(const Hash160&) const;
        bool operator!=(const Hash160&) const;
        friend std::ostream& operator<<(std::ostream&, const Hash160&);
        private:
        // Properties
        const std::array<BYTE, SMALL_HASH_SIZE_BYTES> data;
    };
}
