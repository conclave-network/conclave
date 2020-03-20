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
     * Representation of DER-encoded ECDSA signature with sighash byte
     */
    class EcdsaSignature final
    {
        public:
        // Factories
        // Constructors
        EcdsaSignature(const Hash256& r, const Hash256& s);
        EcdsaSignature(Hash256&& r, Hash256&& s);
        EcdsaSignature(const EcdsaSignature&);
        EcdsaSignature(EcdsaSignature&&);
        // Public Functions
        // Conversions
        operator std::vector<BYTE>() const;
        // Operator Overloads
        EcdsaSignature& operator=(const EcdsaSignature&);
        EcdsaSignature& operator=(EcdsaSignature&&);
        bool operator==(const EcdsaSignature&) const;
        bool operator!=(const EcdsaSignature&) const;
        private:
        // Properties
        Hash256 r;
        Hash256 s;
    };
}
