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

#include "outpoint.h"
#include <cstring>

namespace pt = boost::property_tree;
namespace conclave
{
    const std::string Outpoint::JSONKEY_TXID = "txId";
    const std::string Outpoint::JSONKEY_INDEX = "index";
    
    Outpoint::Outpoint(const Hash256& txId, const uint32_t index)
        : txId(txId), index(index)
    {
    }
    
    Outpoint::Outpoint(const pt::ptree& tree)
        : Outpoint(getPrimitiveFromJson<std::string>(tree, JSONKEY_TXID),
                   getPrimitiveFromJson<uint32_t>(tree, JSONKEY_INDEX))
    {
    }
    
    const std::vector<BYTE> Outpoint::serialize() const
    {
        std::vector<BYTE> serialized(LARGE_HASH_SIZE_BYTES + sizeof(index));
        Hash256 revTxid = txId.reversed();
        std::memcpy(&serialized[0], &revTxid[0], LARGE_HASH_SIZE_BYTES);
        std::memcpy(&serialized[LARGE_HASH_SIZE_BYTES], &index, sizeof(index));
        return serialized;
    }
    
    Outpoint::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<std::string>(JSONKEY_TXID, txId);
        tree.add<uint32_t>(JSONKEY_INDEX, index);
        return tree;
    }
    
    Outpoint::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool Outpoint::operator==(const Outpoint& other) const
    {
        return (txId == other.txId) && (index == other.index);
    }
    
    bool Outpoint::operator!=(const Outpoint& other) const
    {
        return (txId != other.txId) || (index != other.index);
    }
    
    std::ostream& operator<<(std::ostream& os, const Outpoint& outpoint)
    {
        os << static_cast<std::string>(outpoint);
        return os;
    }
}
