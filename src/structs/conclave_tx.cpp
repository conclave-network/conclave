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

#include "conclave_tx.h"
#include "../util/json.h"

namespace conclave
{
    const std::string ConclaveTx::JSONKEY_VERSION = "version";
    const std::string ConclaveTx::JSONKEY_INPUTS = "inputs";
    const std::string ConclaveTx::JSONKEY_CONCLAVE_OUTPUTS = "conclaveOutputs";
    const std::string ConclaveTx::JSONKEY_BITCOIN_OUTPUTS = "bitcoinOutputs";
    const std::string ConclaveTx::JSONKEY_LOCKTIME = "lockTime";
    
    ConclaveTx::ConclaveTx(const uint32_t version,
                           const std::vector<ConclaveInput>& inputs,
                           const std::vector<ConclaveOutput>& conclaveOutputs,
                           const std::vector<BitcoinOutput>& bitcoinOutputs,
                           const uint32_t lockTime)
        : version(version), inputs(inputs), conclaveOutputs(conclaveOutputs),
          bitcoinOutputs(bitcoinOutputs), lockTime(lockTime)
    {
    }
    
    ConclaveTx::ConclaveTx(const pt::ptree& tree)
        : ConclaveTx(getPrimitiveFromJson<uint32_t>(tree, JSONKEY_VERSION),
                     tryGetVectorOfObjects<ConclaveInput>(tree, JSONKEY_INPUTS),
                     tryGetVectorOfObjects<ConclaveOutput>(tree, JSONKEY_CONCLAVE_OUTPUTS),
                     tryGetVectorOfObjects<BitcoinOutput>(tree, JSONKEY_BITCOIN_OUTPUTS),
                     getPrimitiveFromJson<uint32_t>(tree, JSONKEY_LOCKTIME))
    {
    }
    
    ConclaveTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<uint32_t>(JSONKEY_VERSION, version);
        tree.add_child(JSONKEY_INPUTS, vectorOfObjectsToArray(inputs));
        tree.add_child(JSONKEY_CONCLAVE_OUTPUTS, vectorOfObjectsToArray(conclaveOutputs));
        tree.add_child(JSONKEY_BITCOIN_OUTPUTS, vectorOfObjectsToArray(bitcoinOutputs));
        tree.add<uint32_t>(JSONKEY_LOCKTIME, lockTime);
        return tree;
    }
    
    const std::vector<BYTE> ConclaveTx::serialize() const
    {
        const std::vector<BYTE> versionSerialized = serializeU32(version);
        const std::vector<BYTE> inputsSerialized = serializeVectorOfObjects(inputs);
        const std::vector<BYTE> conclaveOutputsSerialized = serializeVectorOfObjects(conclaveOutputs);
        const std::vector<BYTE> bitcoinOutputsSerialized = serializeVectorOfObjects(bitcoinOutputs);
        const std::vector<BYTE> lockTimeSerialized = serializeU32(lockTime);
        std::vector<BYTE> serialized(versionSerialized.size() + inputsSerialized.size() +
                                     conclaveOutputsSerialized.size() + bitcoinOutputsSerialized.size() +
                                     lockTimeSerialized.size());
        size_t pos = writeToByteVector(serialized, versionSerialized);
        pos += writeToByteVector(serialized, inputsSerialized, pos);
        pos += writeToByteVector(serialized, conclaveOutputsSerialized, pos);
        pos += writeToByteVector(serialized, bitcoinOutputsSerialized, pos);
        writeToByteVector(serialized, lockTimeSerialized, pos);
        return serialized;
    }
    
    const Hash256 ConclaveTx::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    ConclaveTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool ConclaveTx::operator==(const ConclaveTx& other) const
    {
        return (version == other.version) && (inputs == other.inputs) && (conclaveOutputs == other.conclaveOutputs) &&
               (bitcoinOutputs == other.bitcoinOutputs) && (lockTime == other.lockTime);
    }
    
    bool ConclaveTx::operator!=(const ConclaveTx& other) const
    {
        return (version != other.version) && (inputs != other.inputs) && (conclaveOutputs != other.conclaveOutputs) &&
               (bitcoinOutputs != other.bitcoinOutputs) && (lockTime != other.lockTime);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveTx& conclaveTx)
    {
        os << static_cast<std::string>(conclaveTx);
        return os;
    }
}
