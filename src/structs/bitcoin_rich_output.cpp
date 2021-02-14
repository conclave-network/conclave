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

#include "bitcoin_rich_output.h"
#include <utility>
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON Keys
    //
    
    const std::string BitcoinRichOutput::JSONKEY_OUTPOINT = "outpoint";
    const std::string BitcoinRichOutput::JSONKEY_BITCOIN_OUTPUT = "bitcoinOutput";
    
    //
    // Factories
    //
    
    BitcoinRichOutput BitcoinRichOutput::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        Outpoint outpoint = Outpoint::deserialize(data, pos);
        BitcoinOutput bitcoinOutput = BitcoinOutput::deserialize(data, pos);
        return BitcoinRichOutput(std::move(outpoint), std::move(bitcoinOutput));
    }
    
    BitcoinRichOutput BitcoinRichOutput::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    BitcoinRichOutput::BitcoinRichOutput(const Outpoint& outpoint, const BitcoinOutput& bitcoinOutput)
        : outpoint(outpoint), bitcoinOutput(bitcoinOutput)
    {
    }
    
    BitcoinRichOutput::BitcoinRichOutput(Outpoint&& outpoint, BitcoinOutput&& bitcoinOutput)
        : outpoint(std::move(outpoint)), bitcoinOutput(std::move(bitcoinOutput))
    {
    }
    
    BitcoinRichOutput::BitcoinRichOutput(const pt::ptree& tree)
        : BitcoinRichOutput(getObjectFromJson<Outpoint>(tree, JSONKEY_OUTPOINT),
                            getObjectFromJson<BitcoinOutput>(tree, JSONKEY_BITCOIN_OUTPUT))
    {
    }
    
    BitcoinRichOutput::BitcoinRichOutput(const std::vector<BYTE>& data)
        : BitcoinRichOutput(deserialize(data))
    {
    }
    
    BitcoinRichOutput::BitcoinRichOutput(const BitcoinRichOutput& other)
        : BitcoinRichOutput(other.outpoint, other.bitcoinOutput)
    {
    }
    
    BitcoinRichOutput::BitcoinRichOutput(BitcoinRichOutput&& other) noexcept
        : BitcoinRichOutput(std::move(other.outpoint), std::move(other.bitcoinOutput))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 BitcoinRichOutput::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> BitcoinRichOutput::serialize() const
    {
        const std::vector<BYTE> outpointSerialized = outpoint.serialize();
        const std::vector<BYTE> bitcoinOutputSerialized = bitcoinOutput.serialize();
        std::vector<BYTE> serialized(outpointSerialized.size() + bitcoinOutputSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, outpointSerialized, pos);
        writeToByteVector(serialized, bitcoinOutputSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    BitcoinRichOutput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_OUTPOINT, static_cast<pt::ptree>(outpoint));
        tree.add_child(JSONKEY_BITCOIN_OUTPUT, static_cast<pt::ptree>(bitcoinOutput));
        return tree;
    }
    
    BitcoinRichOutput::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    BitcoinRichOutput::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    BitcoinRichOutput& BitcoinRichOutput::operator=(const BitcoinRichOutput& other)
    {
        outpoint = other.outpoint;
        bitcoinOutput = other.bitcoinOutput;
        return *this;
    }
    
    BitcoinRichOutput& BitcoinRichOutput::operator=(BitcoinRichOutput&& other)
    {
        outpoint = std::move(other.outpoint);
        bitcoinOutput = std::move(other.bitcoinOutput);
        return *this;
    }
    
    bool BitcoinRichOutput::operator==(const BitcoinRichOutput& other) const
    {
        return (outpoint == other.outpoint) && (bitcoinOutput == other.bitcoinOutput);
    }
    
    bool BitcoinRichOutput::operator!=(const BitcoinRichOutput& other) const
    {
        return (outpoint != other.outpoint) || (bitcoinOutput != other.bitcoinOutput);
    }
    
    std::ostream& operator<<(std::ostream& os, const BitcoinRichOutput& bitcoinRichOutput)
    {
        os << static_cast<std::string>(bitcoinRichOutput);
        return os;
    }
}
