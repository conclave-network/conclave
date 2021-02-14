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

#include "conclave_rich_output.h"
#include <utility>
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON Keys
    //
    
    const std::string ConclaveRichOutput::JSONKEY_OUTPOINT = "outpoint";
    const std::string ConclaveRichOutput::JSONKEY_CONCLAVE_OUTPUT = "conclaveOutput";
    
    //
    // Factories
    //
    
    ConclaveRichOutput ConclaveRichOutput::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        Outpoint outpoint = Outpoint::deserialize(data, pos);
        ConclaveOutput conclaveOutput = ConclaveOutput::deserialize(data, pos);
        return ConclaveRichOutput(std::move(outpoint), std::move(conclaveOutput));
    }
    
    ConclaveRichOutput ConclaveRichOutput::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    ConclaveRichOutput::ConclaveRichOutput(const Outpoint& outpoint, const ConclaveOutput& conclaveOutput)
        : outpoint(outpoint), conclaveOutput(conclaveOutput)
    {
    }
    
    ConclaveRichOutput::ConclaveRichOutput(Outpoint&& outpoint, ConclaveOutput&& conclaveOutput)
        : outpoint(std::move(outpoint)), conclaveOutput(std::move(conclaveOutput))
    {
    }
    
    ConclaveRichOutput::ConclaveRichOutput(const pt::ptree& tree)
        : ConclaveRichOutput(getObjectFromJson<Outpoint>(tree, JSONKEY_OUTPOINT),
                             getObjectFromJson<ConclaveOutput>(tree, JSONKEY_CONCLAVE_OUTPUT))
    {
    }
    
    ConclaveRichOutput::ConclaveRichOutput(const std::vector<BYTE>& data)
        : ConclaveRichOutput(deserialize(data))
    {
    }
    
    ConclaveRichOutput::ConclaveRichOutput(const ConclaveRichOutput& other)
        : ConclaveRichOutput(other.outpoint, other.conclaveOutput)
    {
    }
    
    ConclaveRichOutput::ConclaveRichOutput(ConclaveRichOutput&& other) noexcept
        : ConclaveRichOutput(std::move(other.outpoint), std::move(other.conclaveOutput))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 ConclaveRichOutput::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> ConclaveRichOutput::serialize() const
    {
        const std::vector<BYTE> outpointSerialized = outpoint.serialize();
        const std::vector<BYTE> conclaveOutputSerialized = conclaveOutput.serialize();
        std::vector<BYTE> serialized(outpointSerialized.size() + conclaveOutputSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, outpointSerialized, pos);
        writeToByteVector(serialized, conclaveOutputSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    ConclaveRichOutput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_OUTPOINT, static_cast<pt::ptree>(outpoint));
        tree.add_child(JSONKEY_CONCLAVE_OUTPUT, static_cast<pt::ptree>(conclaveOutput));
        return tree;
    }
    
    ConclaveRichOutput::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    ConclaveRichOutput::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    ConclaveRichOutput& ConclaveRichOutput::operator=(const ConclaveRichOutput& other)
    {
        outpoint = other.outpoint;
        conclaveOutput = other.conclaveOutput;
        return *this;
    }
    
    ConclaveRichOutput& ConclaveRichOutput::operator=(ConclaveRichOutput&& other)
    {
        outpoint = std::move(other.outpoint);
        conclaveOutput = std::move(other.conclaveOutput);
        return *this;
    }
    
    bool ConclaveRichOutput::operator==(const ConclaveRichOutput& other) const
    {
        return (outpoint == other.outpoint) && (conclaveOutput == other.conclaveOutput);
    }
    
    bool ConclaveRichOutput::operator!=(const ConclaveRichOutput& other) const
    {
        return (outpoint != other.outpoint) || (conclaveOutput != other.conclaveOutput);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveRichOutput& conclaveRichOutput)
    {
        os << static_cast<std::string>(conclaveRichOutput);
        return os;
    }
}
