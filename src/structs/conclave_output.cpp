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
    const std::string ConclaveOutput::JSONKEY_SCRIPTPUBKEY = "scriptPubKey";
    const std::string ConclaveOutput::JSONKEY_VALUE = "value";
    const std::string ConclaveOutput::JSONKEY_PREDECESSOR = "predecessor";
    
    ConclaveOutput::ConclaveOutput(const Script& scriptPubKey, const uint64_t value)
        : scriptPubKey(scriptPubKey), value(value), predecessor(std::nullopt)
    {
    }
    
    ConclaveOutput::ConclaveOutput(const Script& scriptPubKey, const uint64_t value, const Outpoint& predecessor)
        : scriptPubKey(scriptPubKey), value(value), predecessor(predecessor)
    {
    }
    
    ConclaveOutput::ConclaveOutput(const pt::ptree& tree)
        : ConclaveOutput(getObjectFromJson<Script>(tree, JSONKEY_SCRIPTPUBKEY),
                         getPrimitiveFromJson<uint64_t>(tree, JSONKEY_VALUE),
                         *getOptionalObjectFromJson<Outpoint>(tree, JSONKEY_PREDECESSOR))
    {
    }
    
    const std::vector<BYTE> ConclaveOutput::serialize() const
    {
        const std::vector<BYTE> scriptPubKeySerialized = scriptPubKey.serialize();
        const std::vector<BYTE> valueSerialized = serializeU64(value);
        const std::vector<BYTE> predecessorSerialized = serializeOptional(predecessor);
        const size_t scriptPubKeySerializedSize = scriptPubKeySerialized.size();
        const size_t valueSerializedSize = valueSerialized.size();
        const size_t predecessorSerializedSize = predecessorSerialized.size();
        std::vector<BYTE> conclaveOutputSerialized(
            scriptPubKeySerializedSize + valueSerializedSize + predecessorSerializedSize);
        writeToByteVector(conclaveOutputSerialized, scriptPubKeySerialized, 0);
        writeToByteVector(conclaveOutputSerialized, valueSerialized, scriptPubKeySerializedSize);
        writeToByteVector(conclaveOutputSerialized, predecessorSerialized,
                          scriptPubKeySerializedSize + valueSerializedSize);
        return conclaveOutputSerialized;
    }
    
    ConclaveOutput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_SCRIPTPUBKEY, (pt::ptree) scriptPubKey);
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
