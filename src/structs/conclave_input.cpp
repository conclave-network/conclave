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

#include "conclave_input.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON Keys
    //
    
    const std::string ConclaveInput::JSONKEY_OUTPOINT = "outpoint";
    const std::string ConclaveInput::JSONKEY_SCRIPTSIG = "scriptSig";
    const std::string ConclaveInput::JSONKEY_SEQUENCE = "sequence";
    const std::string ConclaveInput::JSONKEY_PREDECESSOR = "predecessor";
    
    //
    // Factories
    //
    
    ConclaveInput ConclaveInput::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const Outpoint outpoint = Outpoint::deserialize(data, pos);
        const Script scriptSig = Script::deserialize(data, pos);
        const uint32_t sequence = deserializeIntegral<uint32_t>(data, pos);
        const std::optional<Inpoint> predecessor = deserializeOptionalObject<Inpoint>(data, pos);
        return ConclaveInput(std::move(outpoint), std::move(scriptSig), sequence, std::move(predecessor));
    }
    
    ConclaveInput ConclaveInput::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    ConclaveInput::ConclaveInput(const Outpoint& outpoint, const Script& scriptSig, const uint32_t sequence)
        : outpoint(outpoint), scriptSig(scriptSig), sequence(sequence), predecessor(std::nullopt)
    {
    }
    
    ConclaveInput::ConclaveInput(Outpoint&& outpoint, Script&& scriptSig, const uint32_t sequence)
        : outpoint(std::move(outpoint)), scriptSig(std::move(scriptSig)), sequence(sequence), predecessor(std::nullopt)
    {
    }
    
    ConclaveInput::ConclaveInput(const Outpoint& outpoint, const Script& scriptSig, const uint32_t sequence,
                                 const Inpoint& predecessor)
        : outpoint(outpoint), scriptSig(scriptSig), sequence(sequence), predecessor(predecessor)
    {
    }
    
    ConclaveInput::ConclaveInput(Outpoint&& outpoint, Script&& scriptSig, const uint32_t sequence,
                                 Inpoint&& predecessor)
        : outpoint(std::move(outpoint)), scriptSig(std::move(scriptSig)), sequence(sequence),
          predecessor(std::move(predecessor))
    {
    }
    
    ConclaveInput::ConclaveInput(const Outpoint& outpoint, const Script& scriptSig, const uint32_t sequence,
                                 const std::optional<Inpoint>& predecessor)
        : outpoint(outpoint), scriptSig(scriptSig), sequence(sequence), predecessor(predecessor)
    {
    }
    
    ConclaveInput::ConclaveInput(Outpoint&& outpoint, Script&& scriptSig, const uint32_t sequence,
                                 std::optional<Inpoint>&& predecessor)
        : outpoint(std::move(outpoint)), scriptSig(std::move(scriptSig)),
          sequence(sequence), predecessor(std::move(predecessor))
    {
    }
    
    ConclaveInput::ConclaveInput(const pt::ptree& tree)
        : outpoint(getObjectFromJson<Outpoint>(tree, JSONKEY_OUTPOINT)),
          scriptSig(getObjectFromJson<Script>(tree, JSONKEY_SCRIPTSIG)),
          sequence(getPrimitiveFromJson<uint32_t>(tree, JSONKEY_SEQUENCE)),
          predecessor(getOptionalObjectFromJson<Inpoint>(tree, JSONKEY_PREDECESSOR))
    {
    }
    
    ConclaveInput::ConclaveInput(const std::vector<BYTE>& data)
        : ConclaveInput(deserialize(data))
    {
    }
    
    ConclaveInput::ConclaveInput(const ConclaveInput& other)
        : ConclaveInput(other.outpoint, other.scriptSig, other.sequence, other.predecessor)
    {
    }
    
    ConclaveInput::ConclaveInput(ConclaveInput&& other)
        : ConclaveInput(std::move(other.outpoint), std::move(other.scriptSig), other.sequence,
                        std::move(other.predecessor))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 ConclaveInput::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> ConclaveInput::serialize() const
    {
        const std::vector<BYTE> outpointSerialized = outpoint.serialize();
        const std::vector<BYTE> scriptSigSerialized = scriptSig.serialize();
        const std::vector<BYTE> sequenceSerialized = serializeIntegral(sequence);
        const std::vector<BYTE> predecessorSerialized = serializeOptionalObject(predecessor);
        std::vector<BYTE> serialized(
            outpointSerialized.size() + scriptSigSerialized.size() + sequenceSerialized.size() +
            predecessorSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, outpointSerialized, pos);
        writeToByteVector(serialized, scriptSigSerialized, pos);
        writeToByteVector(serialized, sequenceSerialized, pos);
        writeToByteVector(serialized, predecessorSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    ConclaveInput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_OUTPOINT, static_cast<pt::ptree>(outpoint));
        tree.add_child(JSONKEY_SCRIPTSIG, static_cast<pt::ptree>(scriptSig));
        tree.add<uint32_t>(JSONKEY_SEQUENCE, sequence);
        if (predecessor.has_value()) {
            tree.add_child(JSONKEY_PREDECESSOR, static_cast<pt::ptree>(*predecessor));
        }
        return tree;
    }
    
    ConclaveInput::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    ConclaveInput::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    ConclaveInput& ConclaveInput::operator=(const ConclaveInput& other)
    {
        outpoint = other.outpoint;
        scriptSig = other.scriptSig;
        sequence = other.sequence;
        predecessor = other.predecessor;
        return *this;
    }
    
    ConclaveInput& ConclaveInput::operator=(ConclaveInput&& other)
    {
        outpoint = std::move(other.outpoint);
        scriptSig = std::move(other.scriptSig);
        sequence = other.sequence;
        predecessor = std::move(other.predecessor);
        return *this;
    }
    
    bool ConclaveInput::operator==(const ConclaveInput& other) const
    {
        return (outpoint == other.outpoint) && (scriptSig == other.scriptSig) &&
               (sequence == other.sequence) && (predecessor == other.predecessor);
    }
    
    bool ConclaveInput::operator!=(const ConclaveInput& other) const
    {
        return (outpoint != other.outpoint) || (scriptSig != other.scriptSig) ||
               (sequence != other.sequence) || (predecessor != other.predecessor);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveInput& ConclaveInput)
    {
        os << static_cast<std::string>(ConclaveInput);
        return os;
    }
}
