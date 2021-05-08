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
#include <array>
#include <string>

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
        EcdsaSignature(const EcdsaSignature&);
        EcdsaSignature(EcdsaSignature&&) noexcept;
        EcdsaSignature(Hash256, Hash256);
        explicit EcdsaSignature(const std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES>&);
        explicit EcdsaSignature(const std::string&);
        explicit EcdsaSignature(const char*);
        // Public Functions
        [[nodiscard]] std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES>() const;
        explicit operator std::vector<BYTE>() const;
        explicit operator std::string() const;
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
