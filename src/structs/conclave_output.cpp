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

#include "conclave_output.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON Keys
    //
    
    const std::string ConclaveOutput::JSONKEY_SCRIPTPUBKEY = "scriptPubKey";
    const std::string ConclaveOutput::JSONKEY_VALUE = "value";
    const std::string ConclaveOutput::JSONKEY_PREDECESSOR = "predecessor";
    
    //
    // Factories
    //
    
    ConclaveOutput ConclaveOutput::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const uint64_t value = deserializeIntegral<uint64_t>(data, pos);
        const Script scriptPubKey = Script::deserialize(data, pos);
        const std::optional<Outpoint> predecessor = deserializeOptionalObject<Outpoint>(data, pos);
        if (predecessor.has_value()) {
            return ConclaveOutput(scriptPubKey, value, std::move(*predecessor));
        } else {
            return ConclaveOutput(scriptPubKey, value);
        }
    }
    
    ConclaveOutput ConclaveOutput::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    ConclaveOutput::ConclaveOutput(const Script& scriptPubKey, const uint64_t value)
        : scriptPubKey(scriptPubKey), value(value), predecessor(std::nullopt)
    {
    }
    
    ConclaveOutput::ConclaveOutput(Script&& scriptPubKey, const uint64_t value)
        : scriptPubKey(std::move(scriptPubKey)), value(value), predecessor(std::nullopt)
    {
    }
    
    ConclaveOutput::ConclaveOutput(const Script& scriptPubKey, const uint64_t value, const Outpoint& predecessor)
        : scriptPubKey(scriptPubKey), value(value), predecessor(predecessor)
    {
    }
    
    ConclaveOutput::ConclaveOutput(Script&& scriptPubKey, const uint64_t value, Outpoint&& predecessor)
        : scriptPubKey(std::move(scriptPubKey)), value(value), predecessor(std::move(predecessor))
    {
    }
    
    ConclaveOutput::ConclaveOutput(const Script& scriptPubKey, const uint64_t value,
                                   const std::optional<Outpoint>& predecessor)
        : scriptPubKey(scriptPubKey), value(value), predecessor(predecessor)
    {
    }
    
    ConclaveOutput::ConclaveOutput(Script&& scriptPubKey, const uint64_t value, std::optional<Outpoint>&& predecessor)
        : scriptPubKey(std::move(scriptPubKey)), value(value), predecessor(std::move(predecessor))
    {
    }
    
    ConclaveOutput::ConclaveOutput(const pt::ptree& tree)
        : ConclaveOutput(std::move(getObjectFromJson<Script>(tree, JSONKEY_SCRIPTPUBKEY)),
                         getPrimitiveFromJson<uint64_t>(tree, JSONKEY_VALUE),
                         std::move(getOptionalObjectFromJson<Outpoint>(tree, JSONKEY_PREDECESSOR)))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 ConclaveOutput::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> ConclaveOutput::serialize() const
    {
        const std::vector<BYTE> scriptPubKeySerialized = scriptPubKey.serialize();
        const std::vector<BYTE> valueSerialized = serializeIntegral(value);
        const std::vector<BYTE> predecessorSerialized = serializeOptionalObject(predecessor);
        std::vector<BYTE> serialized(
            scriptPubKeySerialized.size() + valueSerialized.size() + predecessorSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, scriptPubKeySerialized, pos);
        writeToByteVector(serialized, valueSerialized, pos);
        writeToByteVector(serialized, predecessorSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    ConclaveOutput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_SCRIPTPUBKEY, static_cast<pt::ptree>(scriptPubKey));
        tree.add<uint64_t>(JSONKEY_VALUE, value);
        if (predecessor.has_value()) {
            tree.add_child(JSONKEY_PREDECESSOR, static_cast<pt::ptree>(*predecessor));
        }
        return tree;
    }
    
    ConclaveOutput::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    ConclaveOutput::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    ConclaveOutput& ConclaveOutput::operator=(const ConclaveOutput& other)
    {
        scriptPubKey = other.scriptPubKey;
        value = other.value;
        predecessor = other.predecessor;
        return *this;
    }
    
    ConclaveOutput& ConclaveOutput::operator=(ConclaveOutput&& other)
    {
        scriptPubKey = std::move(other.scriptPubKey);
        value = other.value;
        predecessor = std::move(other.predecessor);
        return *this;
    }
    
    bool ConclaveOutput::operator==(const ConclaveOutput& other) const
    {
        return (scriptPubKey == other.scriptPubKey) && (value == other.value) && (predecessor == other.predecessor);
    }
    
    bool ConclaveOutput::operator!=(const ConclaveOutput& other) const
    {
        return (scriptPubKey != other.scriptPubKey) || (value != other.value) || (predecessor != other.predecessor);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveOutput& conclaveOutput)
    {
        os << static_cast<std::string>(conclaveOutput);
        return os;
    }
}
