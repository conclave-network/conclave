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
#include <bitcoin/system.hpp>

namespace bc_system = libbitcoin::system;
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
    
    EcdsaSignature::EcdsaSignature(EcdsaSignature&& other) noexcept
        : r(std::move(other.r)), s(std::move(other.s))
    {
    }
    
    //
    // Public Functions
    //
    
    const std::vector<BYTE> EcdsaSignature::serialize() const
    {
        // DER Serialization
        bc_system::der_signature der;
        bc_system::encode_signature(der, *this);
        return der;
    }
    
    //
    // Conversions
    //
    
    EcdsaSignature::operator std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES>() const
    {
        std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES> arr;
        std::copy(r.begin(), r.end(), arr.begin());
        std::copy(s.begin(), s.end(), arr.begin() + EC_GROUP_ELEMENT_SIZE_BYTES);
        return arr;
    }
    
    EcdsaSignature::operator std::vector<BYTE>() const
    {
        const auto array = static_cast<std::array<BYTE, ECDSA_SIGNATURE_SIZE_BYTES>>(*this);
        std::vector<BYTE> vector(ECDSA_SIGNATURE_SIZE_BYTES);
        std::copy(array.begin(), array.end(), vector.begin());
        return vector;
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
    
    EcdsaSignature& EcdsaSignature::operator=(EcdsaSignature&& other) noexcept
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
        return (r != other.r) || (s != other.s);
    }
}
