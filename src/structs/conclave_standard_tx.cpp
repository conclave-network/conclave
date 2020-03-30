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

#include "conclave_standard_tx.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON keys
    //
    
    const std::string ConclaveStandardTx::JSONKEY_VERSION = "version";
    const std::string ConclaveStandardTx::JSONKEY_INPUTS = "inputs";
    const std::string ConclaveStandardTx::JSONKEY_CONCLAVE_OUTPUTS = "conclaveOutputs";
    const std::string ConclaveStandardTx::JSONKEY_BITCOIN_OUTPUTS = "bitcoinOutputs";
    const std::string ConclaveStandardTx::JSONKEY_LOCKTIME = "lockTime";
    
    //
    // Factories
    //
    
    ConclaveStandardTx ConclaveStandardTx::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        uint32_t version = deserializeIntegral<uint32_t>(data, pos);
    }
    
    ConclaveStandardTx ConclaveStandardTx::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    ConclaveStandardTx::ConclaveStandardTx(const uint32_t version,
                                           const std::vector<ConclaveInput>& inputs,
                                           const std::vector<ConclaveOutput>& conclaveOutputs,
                                           const std::vector<BitcoinOutput>& bitcoinOutputs,
                                           const uint32_t lockTime)
        : version(version), inputs(inputs), conclaveOutputs(conclaveOutputs),
          bitcoinOutputs(bitcoinOutputs), lockTime(lockTime)
    {
    }
    
    ConclaveStandardTx::ConclaveStandardTx(const pt::ptree& tree)
        : ConclaveStandardTx(getPrimitiveFromJson<uint32_t>(tree, JSONKEY_VERSION),
                             tryGetVectorOfObjects<ConclaveInput>(tree, JSONKEY_INPUTS),
                             tryGetVectorOfObjects<ConclaveOutput>(tree, JSONKEY_CONCLAVE_OUTPUTS),
                             tryGetVectorOfObjects<BitcoinOutput>(tree, JSONKEY_BITCOIN_OUTPUTS),
                             getPrimitiveFromJson<uint32_t>(tree, JSONKEY_LOCKTIME))
    {
    }
    
    ConclaveStandardTx::ConclaveStandardTx(const std::vector<BYTE>& data)
        : ConclaveStandardTx(deserialize(data))
    {
    }
    
    ConclaveStandardTx::ConclaveStandardTx(const ConclaveStandardTx& other)
        : ConclaveStandardTx(other.version, other.inputs, other.conclaveOutputs, other.bitcoinOutputs, other.lockTime)
    {
    }
    
    ConclaveStandardTx::ConclaveStandardTx(ConclaveStandardTx&& other)
        : ConclaveStandardTx(other.version, std::move(other.inputs), std::move(other.conclaveOutputs),
                             std::move(other.bitcoinOutputs), other.lockTime)
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 ConclaveStandardTx::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> ConclaveStandardTx::serialize() const
    {
        const std::vector<BYTE> versionSerialized = serializeIntegral(version);
        const std::vector<BYTE> inputsSerialized = serializeVectorOfObjects(inputs);
        const std::vector<BYTE> conclaveOutputsSerialized = serializeVectorOfObjects(conclaveOutputs);
        const std::vector<BYTE> bitcoinOutputsSerialized = serializeVectorOfObjects(bitcoinOutputs);
        const std::vector<BYTE> lockTimeSerialized = serializeIntegral(lockTime);
        std::vector<BYTE> serialized(versionSerialized.size() + inputsSerialized.size() +
                                     conclaveOutputsSerialized.size() + bitcoinOutputsSerialized.size() +
                                     lockTimeSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, versionSerialized, pos);
        writeToByteVector(serialized, inputsSerialized, pos);
        writeToByteVector(serialized, conclaveOutputsSerialized, pos);
        writeToByteVector(serialized, bitcoinOutputsSerialized, pos);
        writeToByteVector(serialized, lockTimeSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    ConclaveStandardTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<uint32_t>(JSONKEY_VERSION, version);
        tree.add_child(JSONKEY_INPUTS, vectorOfObjectsToArray(inputs));
        tree.add_child(JSONKEY_CONCLAVE_OUTPUTS, vectorOfObjectsToArray(conclaveOutputs));
        tree.add_child(JSONKEY_BITCOIN_OUTPUTS, vectorOfObjectsToArray(bitcoinOutputs));
        tree.add<uint32_t>(JSONKEY_LOCKTIME, lockTime);
        return tree;
    }
    
    ConclaveStandardTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    ConclaveStandardTx::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    ConclaveStandardTx& ConclaveStandardTx::operator=(const ConclaveStandardTx& other)
    {
        version = other.version;
        inputs = other.inputs;
        conclaveOutputs = other.conclaveOutputs;
        bitcoinOutputs = other.bitcoinOutputs;
        lockTime = other.lockTime;
        return *this;
    }
    
    ConclaveStandardTx& ConclaveStandardTx::operator=(ConclaveStandardTx&& other)
    {
        version = other.version;
        inputs = std::move(other.inputs);
        conclaveOutputs = std::move(other.conclaveOutputs);
        bitcoinOutputs = std::move(other.bitcoinOutputs);
        lockTime = other.lockTime;
        return *this;
    }
    
    bool ConclaveStandardTx::operator==(const ConclaveStandardTx& other) const
    {
        return (version == other.version) && (inputs == other.inputs) && (conclaveOutputs == other.conclaveOutputs) &&
               (bitcoinOutputs == other.bitcoinOutputs) && (lockTime == other.lockTime);
    }
    
    bool ConclaveStandardTx::operator!=(const ConclaveStandardTx& other) const
    {
        return (version != other.version) && (inputs != other.inputs) && (conclaveOutputs != other.conclaveOutputs) &&
               (bitcoinOutputs != other.bitcoinOutputs) && (lockTime != other.lockTime);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveStandardTx& conclaveStandardTx)
    {
        os << static_cast<std::string>(conclaveStandardTx);
        return os;
    }
}
