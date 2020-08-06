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

#include "ecdsa_signature.h"

namespace conclave
{
    //
    // Constructors
    //
    
    EcdsaSignature::EcdsaSignature(const Hash256& r, const Hash256& s)
        : r(r), s(s)
    {
    }
    
    EcdsaSignature::EcdsaSignature(Hash256&& r, Hash256&& s)
        : r(std::move(r)), s(std::move(s))
    {
    }
    
    EcdsaSignature::EcdsaSignature(const EcdsaSignature& other)
        : r(other.r), s(other.s)
    {
    }
    
    EcdsaSignature::EcdsaSignature(EcdsaSignature&& other)
        : r(std::move(other.r)), s(std::move(other.s))
    {
    }
    
    //
    // Conversions
    //
    
    EcdsaSignature::operator std::vector<BYTE>() const
    {
        return std::vector<BYTE>(); // TODO
    }
    
    //
    // Operator Overloads
    //
    
    EcdsaSignature& EcdsaSignature::operator=(const EcdsaSignature& other)
    {
        r = other.r;
        s = other.s;
        return *this;
    }
    
    EcdsaSignature& EcdsaSignature::operator=(EcdsaSignature&& other)
    {
        r = std::move(other.r);
        s = std::move(other.s);
        return *this;
    }
    
    bool EcdsaSignature::operator==(const EcdsaSignature& other) const
    {
        return (r == other.r) && (s == other.s);
    }
    
    bool EcdsaSignature::operator!=(const EcdsaSignature& other) const
    {
        return (r != other.r) || (s == other.s);
    }
}
