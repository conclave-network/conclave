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

#include "conclave_entry_tx.h"
#include "../util/json.h"

namespace conclave
{
    const std::string ConclaveEntryTx::JSONKEY_OUTPUTS = "outputs";
    const std::string ConclaveEntryTx::JSONKEY_TRUSTEES = "trustees";
    const std::string ConclaveEntryTx::JSONKEY_MIN_SIGS = "minSigs";
    const std::string ConclaveEntryTx::JSONKEY_FUNDING_OUTPOINT = "fundingOutpoint";
    
    ConclaveEntryTx::ConclaveEntryTx(const std::vector<ConclaveOutput>& outputs,
                                     const std::vector<PublicKey>& trustees,
                                     const uint32_t minSigs)
        : outputs(outputs), trustees(trustees), minSigs(minSigs), fundingOutpoint(std::nullopt)
    {
    }
    
    ConclaveEntryTx::ConclaveEntryTx(const std::vector<ConclaveOutput>& outputs,
                                     const std::vector<PublicKey>& trustees,
                                     const uint32_t minSigs,
                                     const Outpoint& fundingOutpoint)
        : outputs(outputs), trustees(trustees), minSigs(minSigs), fundingOutpoint(fundingOutpoint)
    {
    }
    
    ConclaveEntryTx::ConclaveEntryTx(const pt::ptree& tree)
        : ConclaveEntryTx(tryGetVectorOfObjects<ConclaveOutput>(tree, JSONKEY_OUTPUTS),
                          tryGetVectorOfPrimitives<PublicKey>(tree, JSONKEY_TRUSTEES),
                          getPrimitiveFromJson<uint32_t>(tree, JSONKEY_MIN_SIGS),
                          *getOptionalObjectFromJson<Outpoint>(tree, JSONKEY_FUNDING_OUTPOINT))
    {
    }
    
    ConclaveEntryTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_OUTPUTS, vectorOfObjectsToArray(outputs));
        tree.add_child(JSONKEY_TRUSTEES, vectorOfPrimitivesToArray(trustees));
        tree.add<uint32_t>(JSONKEY_MIN_SIGS, minSigs);
        if (fundingOutpoint.has_value()) {
            tree.add_child(JSONKEY_FUNDING_OUTPOINT, static_cast<pt::ptree>(*fundingOutpoint));
        }
        return tree;
    }
    
    const std::vector<BYTE> ConclaveEntryTx::serialize() const
    {
        const std::vector<BYTE> outputsSerialized = serializeVectorOfObjects(outputs);
        const std::vector<BYTE> trusteesSerialized = serializeVectorOfObjects(trustees);
        const std::vector<BYTE> minSigsSerialized = serializeU32(minSigs);
        const std::vector<BYTE> fundingOutpointSerialized = serializeOptionalObject(fundingOutpoint);
        std::vector<BYTE> serialized(outputsSerialized.size() + trusteesSerialized.size() +
                                     minSigsSerialized.size() + fundingOutpointSerialized.size());
        size_t pos = writeToByteVector(serialized, outputsSerialized);
        pos += writeToByteVector(serialized, trusteesSerialized, pos);
        pos += writeToByteVector(serialized, minSigsSerialized, pos);
        writeToByteVector(serialized, fundingOutpointSerialized, pos);
        return serialized;
    }
    
    const Hash256 ConclaveEntryTx::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    ConclaveEntryTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool ConclaveEntryTx::operator==(const ConclaveEntryTx& other) const
    {
        return (outputs == other.outputs) && (trustees == other.trustees)
               && (minSigs == other.minSigs) && (fundingOutpoint == other.fundingOutpoint);
    }
    
    bool ConclaveEntryTx::operator!=(const ConclaveEntryTx& other) const
    {
        return (outputs != other.outputs) || (trustees != other.trustees) ||
               (minSigs != other.minSigs) || (fundingOutpoint != other.fundingOutpoint);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveEntryTx& conclaveEntryTx)
    {
        os << static_cast<std::string>(conclaveEntryTx);
        return os;
    }
}
