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

#include "util/hex.h"
#include "public_key.h"
#include <vector>
#include <stdexcept>
#include <string>

PublicKey::PublicKey(const std::string& hex)
    : PublicKey(hexStringToByteVector(hex))
{
}

PublicKey::PublicKey(const std::vector<BYTE>& bytes)
{
    if (bytes.size() != 33) {
        throw std::invalid_argument("PublicKey should be 33 bytes, not " + std::to_string(bytes.size()));
    }
    if (bytes[0] != 0x02 && bytes[0] != 0x03) {
        throw std::invalid_argument("PublicKey should start with 0x02 or 0x03");
    }
    this->bytes = bytes;
    this->string = byteVectorToHexString(bytes);
}

bool PublicKey::operator==(const PublicKey& other) const
{
    return bytes == other.bytes;
}

bool PublicKey::operator!=(const PublicKey& other) const
{
    return bytes != other.bytes;
}

PublicKey::operator std::string() const
{
    return string;
}

std::ostream& operator<<(std::ostream& os, const PublicKey& publicKey)
{
    os << static_cast<std::string>(publicKey);
    return os;
}
