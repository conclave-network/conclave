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

#include "conclave_claim_tx.h"
#include "../util/json.h"
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON keys
    //
    
    const std::string ConclaveClaimTx::JSONKEY_OUTPUTS = "outputs";
    const std::string ConclaveClaimTx::JSONKEY_TRUSTEES = "trustees";
    const std::string ConclaveClaimTx::JSONKEY_MIN_SIGS = "minSigs";
    const std::string ConclaveClaimTx::JSONKEY_FUNDING_OUTPOINT = "fundingOutpoint";
    
    //
    // Factories
    //
    
    ConclaveClaimTx ConclaveClaimTx::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const std::vector<ConclaveOutput> outputs = deserializeVectorOfObjects<ConclaveOutput>(data, pos);
        const std::vector<PublicKey> trustees = deserializeVectorOfObjects<PublicKey>(data, pos);
        const uint32_t minSigs = deserializeIntegral<uint32_t>(data, pos);
        const std::optional<Outpoint> fundingOutpoint = deserializeOptionalObject<Outpoint>(data, pos);
        if (fundingOutpoint.has_value()) {
            return ConclaveClaimTx(outputs, trustees, minSigs, *fundingOutpoint);
        } else {
            return ConclaveClaimTx(outputs, trustees, minSigs);
        }
    }
    
    ConclaveClaimTx ConclaveClaimTx::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    //
    // Constructors
    //
    
    ConclaveClaimTx::ConclaveClaimTx(const std::vector<ConclaveOutput>& outputs,
                                     const std::vector<PublicKey>& trustees,
                                     const uint32_t minSigs)
        : outputs(outputs), trustees(trustees), minSigs(minSigs), fundingOutpoint(std::nullopt)
    {
    }
    
    ConclaveClaimTx::ConclaveClaimTx(const std::vector<ConclaveOutput>& outputs,
                                     const std::vector<PublicKey>& trustees,
                                     const uint32_t minSigs,
                                     const Outpoint& fundingOutpoint)
        : outputs(outputs), trustees(trustees), minSigs(minSigs), fundingOutpoint(fundingOutpoint)
    {
    }
    
    ConclaveClaimTx::ConclaveClaimTx(const pt::ptree& tree)
        : outputs(tryGetVectorOfObjects<ConclaveOutput>(tree, JSONKEY_OUTPUTS)),
          trustees(tryGetVectorOfPrimitives<PublicKey>(tree, JSONKEY_TRUSTEES)),
          minSigs(getPrimitiveFromJson<uint32_t>(tree, JSONKEY_MIN_SIGS)),
          fundingOutpoint(getOptionalObjectFromJson<Outpoint>(tree, JSONKEY_FUNDING_OUTPOINT))
    {
    }
    
    ConclaveClaimTx::ConclaveClaimTx(const std::vector<BYTE>& data)
        : ConclaveClaimTx(deserialize(data))
    {
    }
    
    ConclaveClaimTx::ConclaveClaimTx(const ConclaveClaimTx& other)
        : outputs(other.outputs),
          trustees(other.trustees),
          minSigs(other.minSigs),
          fundingOutpoint(other.fundingOutpoint)
    {
    }
    
    ConclaveClaimTx::ConclaveClaimTx(ConclaveClaimTx&& other)
        : outputs(std::move(other.outputs)),
          trustees(std::move(other.trustees)),
          minSigs(other.minSigs),
          fundingOutpoint(std::move(other.fundingOutpoint))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 ConclaveClaimTx::getHash256() const
    {
        return Hash256::digest(serialize());
    }
    
    const std::vector<BYTE> ConclaveClaimTx::serialize() const
    {
        const std::vector<BYTE> outputsSerialized = serializeVectorOfObjects(outputs);
        const std::vector<BYTE> trusteesSerialized = serializeVectorOfObjects(trustees);
        const std::vector<BYTE> minSigsSerialized = serializeIntegral(minSigs);
        const std::vector<BYTE> fundingOutpointSerialized = serializeOptionalObject(fundingOutpoint);
        std::vector<BYTE> serialized(outputsSerialized.size() + trusteesSerialized.size() +
                                     minSigsSerialized.size() + fundingOutpointSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, outputsSerialized, pos);
        writeToByteVector(serialized, trusteesSerialized, pos);
        writeToByteVector(serialized, minSigsSerialized, pos);
        writeToByteVector(serialized, fundingOutpointSerialized, pos);
        return serialized;
    }
    
    //
    // Conversions
    //
    
    ConclaveClaimTx::operator pt::ptree() const
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
    
    ConclaveClaimTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    ConclaveClaimTx::operator std::vector<BYTE>() const
    {
        return serialize();
    }
    
    //
    // Operator Overloads
    //
    
    ConclaveClaimTx& ConclaveClaimTx::operator=(const ConclaveClaimTx& other)
    {
        outputs = other.outputs;
        trustees = other.trustees;
        minSigs = other.minSigs;
        fundingOutpoint = other.fundingOutpoint;
        return *this;
    }
    
    ConclaveClaimTx& ConclaveClaimTx::operator=(ConclaveClaimTx&& other)
    {
        outputs = std::move(other.outputs);
        trustees = std::move(other.trustees);
        minSigs = other.minSigs;
        fundingOutpoint = std::move(other.fundingOutpoint);
        return *this;
    }
    
    bool ConclaveClaimTx::operator==(const ConclaveClaimTx& other) const
    {
        return (outputs == other.outputs) && (trustees == other.trustees)
               && (minSigs == other.minSigs) && (fundingOutpoint == other.fundingOutpoint);
    }
    
    bool ConclaveClaimTx::operator!=(const ConclaveClaimTx& other) const
    {
        return (outputs != other.outputs) || (trustees != other.trustees) ||
               (minSigs != other.minSigs) || (fundingOutpoint != other.fundingOutpoint);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveClaimTx& conclaveClaimTx)
    {
        os << static_cast<std::string>(conclaveClaimTx);
        return os;
    }
}
