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

#include "bitcoin_tx.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    const std::string BitcoinTx::JSONKEY_VERSION = "version";
    const std::string BitcoinTx::JSONKEY_INPUTS = "inputs";
    const std::string BitcoinTx::JSONKEY_OUTPUTS = "outputs";
    const std::string BitcoinTx::JSONKEY_LOCKTIME = "lockTime";
    
    BitcoinTx::BitcoinTx(const uint32_t version,
                         const std::vector<BitcoinInput>& inputs,
                         const std::vector<BitcoinOutput>& outputs,
                         const uint32_t lockTime)
        : version(version), inputs(inputs), outputs(outputs), lockTime(lockTime)
    {
    }
    
    BitcoinTx::BitcoinTx(const pt::ptree& tree)
        : BitcoinTx(
        getPrimitiveFromJson<uint32_t>(tree, JSONKEY_VERSION),
        tryGetVectorOfObjects<BitcoinInput>(tree, JSONKEY_INPUTS),
        tryGetVectorOfObjects<BitcoinOutput>(tree, JSONKEY_OUTPUTS),
        getPrimitiveFromJson<uint32_t>(tree, JSONKEY_LOCKTIME))
    {
    }
    
    const std::vector<BYTE> BitcoinTx::serialize() const
    {
        const std::vector<BYTE> versionSerialized = serializeIntegral(version);
        const std::vector<BYTE> inputsSerialized = serializeVectorOfObjects(inputs);
        const std::vector<BYTE> outputsSerialized = serializeVectorOfObjects(outputs);
        const std::vector<BYTE> lockTimeSerialized = serializeIntegral(lockTime);
        std::vector<BYTE> serialized(
            versionSerialized.size() + inputsSerialized.size() +
            outputsSerialized.size() + lockTimeSerialized.size()
        );
        size_t pos = writeToByteVector(serialized, versionSerialized);
        pos += writeToByteVector(serialized, inputsSerialized, pos);
        pos += writeToByteVector(serialized, outputsSerialized, pos);
        writeToByteVector(serialized, lockTimeSerialized, pos);
        return serialized;
    }
    
    const Hash256 BitcoinTx::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    BitcoinTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<uint32_t>(JSONKEY_VERSION, version);
        tree.add_child(JSONKEY_INPUTS, vectorOfObjectsToArray(inputs));
        tree.add_child(JSONKEY_OUTPUTS, vectorOfObjectsToArray(outputs));
        tree.add<uint32_t>(JSONKEY_LOCKTIME, lockTime);
        return tree;
    }
    
    BitcoinTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    BitcoinTx::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    bool BitcoinTx::operator==(const BitcoinTx& other) const
    {
        return (version == other.version) && (inputs == other.inputs) &&
               (outputs == other.outputs) && (lockTime == other.lockTime);
    }
    
    bool BitcoinTx::operator!=(const BitcoinTx& other) const
    {
        return (version != other.version) || (inputs != other.inputs) ||
               (outputs != other.outputs) || (lockTime != other.lockTime);
    }
    
    std::ostream& operator<<(std::ostream& os, const BitcoinTx& bitcoinTx)
    {
        os << static_cast<std::string>(bitcoinTx);
        return os;
    }
}
