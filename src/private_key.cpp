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

#include "private_key.h"
#include <bitcoin/system/math/elliptic_curve.hpp>

namespace conclave
{
    using namespace libbitcoin::system;
    
    static PublicKey derivePublicKey(const Hash256& privateKeyData)
    {
        ec_uncompressed ecu;
        secret_to_public(ecu, privateKeyData);
        return PublicKey(ecu);
    }
    
    PrivateKey::PrivateKey(const Hash256& data)
        : data(data), publicKey(derivePublicKey(data))
    {
    }
    
    [[nodiscard]] const PublicKey PrivateKey::getPublicKey() const
    {
        return publicKey;
    }
    
    [[nodiscard]] const EcdsaSignature PrivateKey::sign(const Hash256& message) const
    {
        ec_signature sig;
        libbitcoin::system::sign(sig, data, static_cast<hash_digest>(message));
        return EcdsaSignature(sig);
    }
}
