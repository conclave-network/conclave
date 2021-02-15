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

#include "ecdsa_signature.h"
#include "public_key.h"
#include "hash256.h"
#include "conclave.h"

namespace conclave
{
    /**
     * Secp256k1 curve order (n).
     */
    const static std::array<BYTE, 32> SECP256K1_ORDER_BA{
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xfe,
        0xba, 0xae, 0xdc, 0xe6,
        0xaf, 0x48, 0xa0, 0x3b,
        0xbf, 0xd2, 0x5e, 0x8c,
        0xd0, 0x36, 0x41, 0x41
    };
    
    class PrivateKey
    {
        public:
        explicit PrivateKey(const Hash256&);
        [[nodiscard]] PublicKey getPublicKey() const;
        [[nodiscard]] PublicKey getEvenYPublicKey() const;
        [[nodiscard]] EcdsaSignature sign(const Hash256&) const;
        private:
        const Hash256 data;
        const std::array<PublicKey, 2> publicKeys;
        const uint8_t evenYPublicKeyIndex;
    };
}
