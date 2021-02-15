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

#include "private_key.h"
#include <bitcoin/system/math/elliptic_curve.hpp>
#include <bitcoin/system/math/ec_scalar.hpp>

namespace conclave
{
    using namespace libbitcoin::system;
    
    static inline std::array<PublicKey, 2> derivePublicKeys(const Hash256& privateKeyData)
    {
        ec_scalar pk1(privateKeyData);
        ec_scalar pk2(ec_scalar(SECP256K1_ORDER_BA) - pk1);
        ec_uncompressed ecu1, ecu2;
        secret_to_public(ecu1, pk1);
        secret_to_public(ecu2, pk2);
        return {
            PublicKey(ecu1),
            PublicKey(ecu2)
        };
    }
    
    ///
    /// Constructors
    ///
    
    PrivateKey::PrivateKey(const Hash256& data)
        : data(data), publicKeys(derivePublicKeys(data)), evenYPublicKeyIndex(!this->publicKeys[0].yIsEven())
    {
    }
    
    ///
    /// Public Functions
    ///
    
    [[nodiscard]] PublicKey PrivateKey::getPublicKey() const
    {
        return publicKeys[0];
    }
    
    [[nodiscard]] PublicKey PrivateKey::getEvenYPublicKey() const
    {
        return publicKeys[evenYPublicKeyIndex];
    }
    
    [[nodiscard]] EcdsaSignature PrivateKey::sign(const Hash256& message) const
    {
        ec_signature sig;
        libbitcoin::system::sign(sig, data, static_cast<hash_digest>(message));
        return EcdsaSignature(sig);
    }
}
