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

#include "bitcoin_output.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON Keys
    //
    
    const std::string BitcoinOutput::JSONKEY_VALUE = "value";
    const std::string BitcoinOutput::JSONKEY_SCRIPTPUBKEY = "scriptPubKey";
    
    //
    // Factories
    //
    
    BitcoinOutput BitcoinOutput::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const uint64_t value = deserializeIntegral<uint64_t>(data, pos);
        const Script scriptPubKey = Script::deserialize(data, pos);
        return BitcoinOutput(value, std::move(scriptPubKey));
    }
    
    BitcoinOutput BitcoinOutput::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    BitcoinOutput::BitcoinOutput(const uint64_t value, const Script& scriptPubKey)
        : value(value), scriptPubKey(scriptPubKey)
    {
    }
    
    BitcoinOutput::BitcoinOutput(const uint64_t value, Script&& scriptPubKey)
        : value(value), scriptPubKey(std::move(scriptPubKey))
    {
    }
    
    BitcoinOutput::BitcoinOutput(const pt::ptree& tree)
        : BitcoinOutput(getPrimitiveFromJson<uint64_t>(tree, JSONKEY_VALUE),
                        getObjectFromJson<Script>(tree, JSONKEY_SCRIPTPUBKEY))
    {
    }
    
    BitcoinOutput::BitcoinOutput(const std::vector<BYTE>& data)
        : BitcoinOutput(deserialize(data))
    {
    }
    
    BitcoinOutput::BitcoinOutput(const BitcoinOutput& other)
        : BitcoinOutput(other.value, other.scriptPubKey)
    {
    }
    
    BitcoinOutput::BitcoinOutput(BitcoinOutput&& other)
        : BitcoinOutput(other.value, std::move(other.scriptPubKey))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 BitcoinOutput::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> BitcoinOutput::serialize() const
    {
        const std::vector<BYTE> valueSerialized = serializeIntegral(value);
        const std::vector<BYTE> scriptPubKeySerialized = scriptPubKey.serialize();
        std::vector<BYTE> serialized(valueSerialized.size() + scriptPubKeySerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, valueSerialized, pos);
        writeToByteVector(serialized, scriptPubKeySerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    BitcoinOutput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<uint64_t>(JSONKEY_VALUE, value);
        tree.add_child(JSONKEY_SCRIPTPUBKEY, (pt::ptree) scriptPubKey);
        return tree;
    }
    
    BitcoinOutput::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    BitcoinOutput::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    BitcoinOutput& BitcoinOutput::operator=(const BitcoinOutput& other)
    {
        value = other.value;
        scriptPubKey = other.scriptPubKey;
        return *this;
    }
    
    BitcoinOutput& BitcoinOutput::operator=(BitcoinOutput&& other)
    {
        value = other.value;
        scriptPubKey = std::move(other.scriptPubKey);
        return *this;
    }
    
    bool BitcoinOutput::operator==(const BitcoinOutput& other) const
    {
        return (value == other.value) && (scriptPubKey == other.scriptPubKey);
    }
    
    bool BitcoinOutput::operator!=(const BitcoinOutput& other) const
    {
        return (value != other.value) || (scriptPubKey != other.scriptPubKey);
    }
    
    std::ostream& operator<<(std::ostream& os, const BitcoinOutput& bitcoinOutput)
    {
        os << static_cast<std::string>(bitcoinOutput);
        return os;
    }
}
