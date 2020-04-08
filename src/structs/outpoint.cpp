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
#include "../util/json.h"
#include "../util/serialization.h"

namespace pt = boost::property_tree;
namespace conclave
{
    //
    // JSON keys
    //
    
    const std::string Outpoint::JSONKEY_TXID = "txId";
    const std::string Outpoint::JSONKEY_INDEX = "index";
    
    //
    // Factories
    //
    
    Outpoint Outpoint::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        Hash256 txId = Hash256::deserialize(data, pos);
        uint32_t index = deserializeIntegral<uint32_t>(data, pos);
        return Outpoint(txId, index);
    }
    
    Outpoint Outpoint::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    Outpoint::Outpoint(const Hash256& txId, const uint32_t index)
        : txId(txId), index(index)
    {
    }
    
    Outpoint::Outpoint(const pt::ptree& tree)
        : Outpoint(getPrimitiveFromJson<std::string>(tree, JSONKEY_TXID),
                   getPrimitiveFromJson<uint32_t>(tree, JSONKEY_INDEX))
    {
    }
    
    Outpoint::Outpoint(const std::vector<BYTE>& data)
        : Outpoint(deserialize(data))
    {
    }
    
    Outpoint::Outpoint(const Outpoint& other)
        : Outpoint(other.txId, other.index)
    {
    }
    
    Outpoint::Outpoint(Outpoint&& other)
        : Outpoint(std::move(other.txId), other.index)
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 Outpoint::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> Outpoint::serialize() const
    {
        std::vector<BYTE> txIdSerialized = txId.serialize();
        std::vector<BYTE> indexSerialized = serializeIntegral(index);
        std::vector<BYTE> serialized(txIdSerialized.size() + indexSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, txIdSerialized, pos);
        writeToByteVector(serialized, indexSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    Outpoint::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<std::string>(JSONKEY_TXID, txId);
        tree.add<uint32_t>(JSONKEY_INDEX, index);
        return tree;
    }
    
    Outpoint::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    Outpoint::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    Outpoint& Outpoint::operator=(const Outpoint& other)
    {
        txId = other.txId;
        index = other.index;
        return *this;
    }
    
    Outpoint& Outpoint::operator=(Outpoint&& other)
    {
        txId = std::move(other.txId);
        index = other.index;
        return *this;
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
