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

#include "hash256.h"

namespace conclave
{
    /***
     * Representation of ECDSA signature.
     *
     * NOTE: Converting to a byte array or byte vector gives a fixed-length
     * representation of r and s concatenated. Calling the serialize() method
     * returns the DER serialization. DER serialization is needed in bitcoin
     * script, but going forward is not the favoured format.
     */
    class EcdsaSignature final
    {
        public:
        // Factories
        static EcdsaSignature deserialize(const std::vector<BYTE>&, size_t&);
        static EcdsaSignature deserialize(const std::vector<BYTE>&);
        // Constructors
        EcdsaSignature(const Hash256& r, const Hash256& s);
        EcdsaSignature(Hash256&& r, Hash256&& s);
        EcdsaSignature(const std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES>&);
        EcdsaSignature(const EcdsaSignature&);
        EcdsaSignature(EcdsaSignature&&) noexcept;
        // Public Functions
        const std::vector<BYTE> serialize() const;
        // Conversions
        operator std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES>() const;
        operator std::vector<BYTE>() const;
        operator std::string() const;
        // Operator Overloads
        EcdsaSignature& operator=(const EcdsaSignature&);
        EcdsaSignature& operator=(EcdsaSignature&&) noexcept;
        bool operator==(const EcdsaSignature&) const;
        bool operator!=(const EcdsaSignature&) const;
        friend std::ostream& operator<<(std::ostream&, const EcdsaSignature&);
        private:
        // Properties
        Hash256 r;
        Hash256 s;
    };
}
