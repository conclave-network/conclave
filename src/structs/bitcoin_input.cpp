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

#include "bitcoin_input.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON Keys
    //
    
    const std::string BitcoinInput::JSONKEY_OUTPOINT = "outpoint";
    const std::string BitcoinInput::JSONKEY_SCRIPTSIG = "scriptSig";
    const std::string BitcoinInput::JSONKEY_SEQUENCE = "sequence";
    
    //
    // Factories
    //
    
    BitcoinInput BitcoinInput::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const Outpoint outpoint = Outpoint::deserialize(data, pos);
        const Script scriptSig = Script::deserialize(data, pos);
        const uint32_t sequence = deserializeIntegral<uint32_t>(data, pos);
        return BitcoinInput(std::move(outpoint), std::move(scriptSig), sequence);
    }
    
    BitcoinInput BitcoinInput::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    BitcoinInput::BitcoinInput(const Outpoint& outpoint, const Script& scriptSig, const uint32_t sequence)
        : outpoint(outpoint), scriptSig(scriptSig), sequence(sequence)
    {
    }
    
    BitcoinInput::BitcoinInput(Outpoint&& outpoint, Script&& scriptSig, const uint32_t sequence)
        : outpoint(std::move(outpoint)), scriptSig(std::move(scriptSig)), sequence(sequence)
    {
    }
    
    BitcoinInput::BitcoinInput(const pt::ptree& tree)
        : BitcoinInput(getObjectFromJson<Outpoint>(tree, JSONKEY_OUTPOINT),
                       getObjectFromJson<Script>(tree, JSONKEY_SCRIPTSIG),
                       getPrimitiveFromJson<uint32_t>(tree, JSONKEY_SEQUENCE))
    {
    }
    
    BitcoinInput::BitcoinInput(const std::vector<BYTE>& data)
        : BitcoinInput(deserialize(data))
    {
    }
    
    BitcoinInput::BitcoinInput(const BitcoinInput& other)
        : BitcoinInput(other.outpoint, other.scriptSig, other.sequence)
    {
    }
    
    BitcoinInput::BitcoinInput(BitcoinInput&& other)
        : BitcoinInput(std::move(other.outpoint), std::move(other.scriptSig), other.sequence)
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 BitcoinInput::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> BitcoinInput::serialize() const
    {
        const std::vector<BYTE> outpointSerialized = outpoint.serialize();
        const std::vector<BYTE> scriptSigSerialized = scriptSig.serialize();
        const std::vector<BYTE> sequenceSerialized = serializeIntegral(sequence);
        std::vector<BYTE> serialized(
            outpointSerialized.size() + scriptSigSerialized.size() + sequenceSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, outpointSerialized, pos);
        writeToByteVector(serialized, scriptSigSerialized, pos);
        writeToByteVector(serialized, sequenceSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    BitcoinInput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_OUTPOINT, static_cast<pt::ptree>(outpoint));
        tree.add_child(JSONKEY_SCRIPTSIG, static_cast<pt::ptree>(scriptSig));
        tree.add<uint32_t>(JSONKEY_SEQUENCE, sequence);
        return tree;
    }
    
    BitcoinInput::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    BitcoinInput::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    BitcoinInput& BitcoinInput::operator=(const BitcoinInput& other)
    {
        outpoint = other.outpoint;
        scriptSig = other.scriptSig;
        sequence = other.sequence;
        return *this;
    }
    
    BitcoinInput& BitcoinInput::operator=(BitcoinInput&& other)
    {
        outpoint = std::move(other.outpoint);
        scriptSig = std::move(other.scriptSig);
        sequence = other.sequence;
        return *this;
    }
    
    bool BitcoinInput::operator==(const BitcoinInput& other) const
    {
        return (outpoint == other.outpoint) && (scriptSig == other.scriptSig) && (sequence == other.sequence);
    }
    
    bool BitcoinInput::operator!=(const BitcoinInput& other) const
    {
        return (outpoint != other.outpoint) || (scriptSig != other.scriptSig) || (sequence != other.sequence);
    }
    
    std::ostream& operator<<(std::ostream& os, const BitcoinInput& bitcoinInput)
    {
        os << static_cast<std::string>(bitcoinInput);
        return os;
    }
}
