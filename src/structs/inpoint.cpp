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

#include "inpoint.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace pt = boost::property_tree;
namespace conclave
{
    //
    // JSON keys
    //
    
    const std::string Inpoint::JSONKEY_TXID = "txId";
    const std::string Inpoint::JSONKEY_INDEX = "index";
    
    //
    // Factories
    //
    
    Inpoint Inpoint::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        Hash256 txId = Hash256::deserialize(data, pos);
        uint32_t index = deserializeIntegral<uint32_t>(data, pos);
        return Inpoint(txId, index);
    }
    
    Inpoint Inpoint::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    Inpoint::Inpoint(const Hash256& txId, const uint32_t index)
        : txId(txId), index(index)
    {
    }
    
    Inpoint::Inpoint(const pt::ptree& tree)
        : Inpoint(getPrimitiveFromJson<std::string>(tree, JSONKEY_TXID),
                  getPrimitiveFromJson<uint32_t>(tree, JSONKEY_INDEX))
    {
    }
    
    Inpoint::Inpoint(const std::vector<BYTE>& data)
        : Inpoint(deserialize(data))
    {
    }
    
    Inpoint::Inpoint(const Inpoint& other)
        : Inpoint(other.txId, other.index)
    {
    }
    
    Inpoint::Inpoint(Inpoint&& other)
        : Inpoint(std::move(other.txId), other.index)
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 Inpoint::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> Inpoint::serialize() const
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
    
    Inpoint::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<std::string>(JSONKEY_TXID, txId);
        tree.add<uint32_t>(JSONKEY_INDEX, index);
        return tree;
    }
    
    Inpoint::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    Inpoint::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    Inpoint& Inpoint::operator=(const Inpoint& other)
    {
        txId = other.txId;
        index = other.index;
        return *this;
    }
    
    Inpoint& Inpoint::operator=(Inpoint&& other)
    {
        txId = std::move(other.txId);
        index = other.index;
        return *this;
    }
    
    bool Inpoint::operator==(const Inpoint& other) const
    {
        return (txId == other.txId) && (index == other.index);
    }
    
    bool Inpoint::operator!=(const Inpoint& other) const
    {
        return (txId != other.txId) || (index != other.index);
    }
    
    std::ostream& operator<<(std::ostream& os, const Inpoint& outpoint)
    {
        os << static_cast<std::string>(outpoint);
        return os;
    }
}

