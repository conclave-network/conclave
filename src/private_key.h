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
    class PrivateKey
    {
        public:
        PrivateKey(const Hash256&);
        [[nodiscard]] const PublicKey getPublicKey() const;
        [[nodiscard]] const EcdsaSignature sign(const Hash256&) const;
        private:
        Hash256 data;
        PublicKey publicKey;
    };
}
