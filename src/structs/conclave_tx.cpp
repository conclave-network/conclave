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
#include "../util/serialization.h"

namespace conclave
{
    //
    // JSON keys
    //
    
    const std::string ConclaveTx::JSONKEY_VERSION = "version";
    const std::string ConclaveTx::JSONKEY_LOCK_TIME = "lockTime";
    const std::string ConclaveTx::JSONKEY_MIN_SIGS = "minSigs";
    const std::string ConclaveTx::JSONKEY_FUND_POINT = "fundPoint";
    const std::string ConclaveTx::JSONKEY_TRUSTEES = "trustees";
    const std::string ConclaveTx::JSONKEY_CONCLAVE_INPUTS = "conclaveInputs";
    const std::string ConclaveTx::JSONKEY_BITCOIN_OUTPUTS = "bitcoinOutputs";
    const std::string ConclaveTx::JSONKEY_CONCLAVE_OUTPUTS = "conclaveOutputs";
    
    //
    // Factories
    //
    
    ConclaveTx ConclaveTx::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        const uint32_t version = deserializeIntegral<uint32_t>(data, pos);
        const uint32_t lockTime = deserializeIntegral<uint32_t>(data, pos);
        const uint32_t minSigs = deserializeIntegral<uint32_t>(data, pos);
        const std::optional<Outpoint> fundPoint = deserializeOptionalObject<Outpoint>(data, pos);
        const std::vector<PublicKey> trustees = deserializeVectorOfObjects<PublicKey>(data, pos);
        const std::vector<ConclaveInput> conclaveInputs = deserializeVectorOfObjects<ConclaveInput>(data, pos);
        const std::vector<BitcoinOutput> bitcoinOutputs = deserializeVectorOfObjects<BitcoinOutput>(data, pos);
        const std::vector<ConclaveOutput> conclaveOutputs = deserializeVectorOfObjects<ConclaveOutput>(data, pos);
        return ConclaveTx(version, lockTime, minSigs, fundPoint, trustees, conclaveInputs, bitcoinOutputs,
                          conclaveOutputs);
    }
    
    ConclaveTx ConclaveTx::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
    }
    
    
    //
    // Constructors
    //
    
    ConclaveTx::ConclaveTx(const uint32_t minSigs, const std::vector<PublicKey>& trustees,
                           const std::vector<ConclaveOutput>& conclaveOutputs)
        : version(0), lockTime(0), minSigs(minSigs), fundPoint(std::nullopt), trustees(trustees),
          conclaveInputs({}), bitcoinOutputs({}), conclaveOutputs(conclaveOutputs)
    {
    }
    
    ConclaveTx::ConclaveTx(const uint32_t minSigs, const std::vector<PublicKey>& trustees,
                           const std::vector<BitcoinOutput>& bitcoinOutputs,
                           const std::vector<ConclaveOutput>& conclaveOutputs)
        : version(0), lockTime(0), minSigs(minSigs), fundPoint(std::nullopt), trustees(trustees),
          conclaveInputs({}), bitcoinOutputs(bitcoinOutputs), conclaveOutputs(conclaveOutputs)
    {
    }
    
    ConclaveTx::ConclaveTx(const uint32_t minSigs, const Outpoint& fundPoint,
                           const std::vector<PublicKey>& trustees, const std::vector<BitcoinOutput>& bitcoinOutputs,
                           const std::vector<ConclaveOutput>& conclaveOutputs)
        : version(0), lockTime(0), minSigs(minSigs), fundPoint(fundPoint), trustees(trustees),
          conclaveInputs({}), bitcoinOutputs(bitcoinOutputs), conclaveOutputs(conclaveOutputs)
    {
    }
    
    ConclaveTx::ConclaveTx(const uint32_t version, const uint32_t lockTime,
                           const std::vector<ConclaveInput>& conclaveInputs,
                           const std::vector<BitcoinOutput>& bitcoinOutputs,
                           const std::vector<ConclaveOutput>& conclaveOutputs)
        : version(version), lockTime(lockTime), minSigs(0), fundPoint(std::nullopt), trustees({}),
          conclaveInputs(conclaveInputs), bitcoinOutputs(bitcoinOutputs), conclaveOutputs(conclaveOutputs)
    {
    }
    
    ConclaveTx::ConclaveTx(const uint32_t version, const uint32_t lockTime, const uint32_t minSigs,
                           const std::optional<Outpoint>& fundPoint, const std::vector<PublicKey>& trustees,
                           const std::vector<ConclaveInput>& conclaveInputs,
                           const std::vector<BitcoinOutput>& bitcoinOutputs,
                           const std::vector<ConclaveOutput>& conclaveOutputs)
        : version(version), lockTime(lockTime), minSigs(minSigs), fundPoint(fundPoint), trustees(trustees),
          conclaveInputs(conclaveInputs), bitcoinOutputs(bitcoinOutputs), conclaveOutputs(conclaveOutputs)
    {
    }
    
    ConclaveTx::ConclaveTx(const pt::ptree& tree)
        : ConclaveTx(getPrimitiveFromJson<uint32_t>(tree, JSONKEY_VERSION),
                     getPrimitiveFromJson<uint32_t>(tree, JSONKEY_LOCK_TIME),
                     getPrimitiveFromJson<uint32_t>(tree, JSONKEY_MIN_SIGS),
                     getOptionalObjectFromJson<Outpoint>(tree, JSONKEY_FUND_POINT),
                     getVectorOfPrimitivesFromJson<PublicKey>(tree, JSONKEY_TRUSTEES),
                     getVectorOfObjectsFromJson<ConclaveInput>(tree, JSONKEY_CONCLAVE_INPUTS),
                     getVectorOfObjectsFromJson<BitcoinOutput>(tree, JSONKEY_BITCOIN_OUTPUTS),
                     getVectorOfObjectsFromJson<ConclaveOutput>(tree, JSONKEY_CONCLAVE_OUTPUTS))
    {
    }
    
    ConclaveTx::ConclaveTx(const std::vector<BYTE>& data)
        : ConclaveTx(deserialize(data))
    {
    }
    
    ConclaveTx::ConclaveTx(const ConclaveTx& other)
        : ConclaveTx(other.version, other.lockTime, other.minSigs, other.fundPoint, other.trustees,
                     other.conclaveInputs, other.bitcoinOutputs, other.conclaveOutputs)
    {
    }
    
    ConclaveTx::ConclaveTx(ConclaveTx&& other)
        : ConclaveTx(other.version, other.lockTime, other.minSigs, std::move(other.fundPoint),
                     std::move(other.trustees), std::move(other.conclaveInputs),
                     std::move(other.bitcoinOutputs), std::move(other.conclaveOutputs))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash256 ConclaveTx::getHash256(const bool preFund) const
    {
        return Hash256::digest(serialize(preFund));
    }
    
    const std::vector<BYTE> ConclaveTx::serialize(const bool preFund) const
    {
        const std::optional<Outpoint> fundPoint = preFund ? std::nullopt : this->fundPoint;
        const std::vector<BYTE> versionSerialized = serializeIntegral<uint32_t>(version);
        const std::vector<BYTE> lockTimeSerialized = serializeIntegral<uint32_t>(lockTime);
        const std::vector<BYTE> minSigsSerialized = serializeIntegral<uint32_t>(minSigs);
        const std::vector<BYTE> fundPointSerialized = serializeOptionalObject<Outpoint>(fundPoint);
        const std::vector<BYTE> trusteesSerialized = serializeVectorOfObjects<PublicKey>(trustees);
        const std::vector<BYTE> conclaveInputsSerialized = serializeVectorOfObjects<ConclaveInput>(conclaveInputs);
        const std::vector<BYTE> bitcoinOutputsSerialized = serializeVectorOfObjects<BitcoinOutput>(bitcoinOutputs);
        const std::vector<BYTE> conclaveOutputsSerialized = serializeVectorOfObjects<ConclaveOutput>(conclaveOutputs);
        std::vector<BYTE> serialized(versionSerialized.size() + lockTimeSerialized.size() +
                                     minSigsSerialized.size() + fundPointSerialized.size() +
                                     trusteesSerialized.size() + conclaveInputsSerialized.size() +
                                     bitcoinOutputsSerialized.size() + conclaveOutputsSerialized.size());
        size_t pos = 0;
        writeToByteVector(serialized, versionSerialized, pos);
        writeToByteVector(serialized, lockTimeSerialized, pos);
        writeToByteVector(serialized, minSigsSerialized, pos);
        writeToByteVector(serialized, fundPointSerialized, pos);
        writeToByteVector(serialized, trusteesSerialized, pos);
        writeToByteVector(serialized, conclaveInputsSerialized, pos);
        writeToByteVector(serialized, bitcoinOutputsSerialized, pos);
        writeToByteVector(serialized, conclaveOutputsSerialized, pos);
        return serialized;
    }
    
    const bool ConclaveTx::isClaimTx() const
    {
        return (trustees.size() > 0) && (minSigs <= trustees.size());
    }
    
    const Script ConclaveTx::getClaimScript() const
    {
        const uint32_t nTrustees = trustees.size();
        std::vector<ScriptElement> scriptElements{
            getHash256(true), ScriptOp::drop, nTrustees
        };
        scriptElements.reserve(3 + nTrustees + 2);
        for (const PublicKey& trustee: trustees) {
            scriptElements.emplace_back(trustee);
        }
        scriptElements.emplace_back(minSigs);
        scriptElements.emplace_back(ScriptOp::checkmultisig);
        return Script(scriptElements);
    }
    
    const uint64_t ConclaveTx::getBitcoinOutputValue() const
    {
        uint64_t value = 0;
        for (const BitcoinOutput& bitcoinOutput: bitcoinOutputs) {
            value += bitcoinOutput.value;
        }
        return value;
    }
    
    const uint64_t ConclaveTx::getConclaveOutputValue() const
    {
        uint64_t value = 0;
        for (const ConclaveOutput& conclaveOutput: conclaveOutputs) {
            value += conclaveOutput.value;
        }
        return value;
    }
    
    const uint64_t ConclaveTx::getTotalOutputValue() const
    {
        return getBitcoinOutputValue() + getConclaveOutputValue();
    }
    
    //
    // Conversions
    //
    
    ConclaveTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<uint32_t>(JSONKEY_VERSION, version);
        tree.add<uint32_t>(JSONKEY_LOCK_TIME, lockTime);
        tree.add<uint32_t>(JSONKEY_MIN_SIGS, minSigs);
        if (fundPoint.has_value()) {
            tree.add_child(JSONKEY_FUND_POINT, static_cast<pt::ptree>(*fundPoint));
        }
        if (trustees.size() > 0) {
            tree.add_child(JSONKEY_TRUSTEES, vectorOfObjectsToArray(trustees));
        }
        if (conclaveInputs.size() > 0) {
            tree.add_child(JSONKEY_CONCLAVE_INPUTS, vectorOfObjectsToArray(conclaveInputs));
        }
        if (bitcoinOutputs.size() > 0) {
            tree.add_child(JSONKEY_BITCOIN_OUTPUTS, vectorOfObjectsToArray(bitcoinOutputs));
        }
        if (conclaveOutputs.size() > 0) {
            tree.add_child(JSONKEY_CONCLAVE_OUTPUTS, vectorOfObjectsToArray(conclaveOutputs));
        }
        return tree;
    }
    
    ConclaveTx::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    ConclaveTx::operator std::vector<BYTE>() const
    {
        return serialize(false);
    }
    
    //
    // Operator Overloads
    //
    
    ConclaveTx& ConclaveTx::operator=(const ConclaveTx& other)
    {
        version = other.version;
        lockTime = other.lockTime;
        minSigs = other.minSigs;
        fundPoint = other.fundPoint;
        trustees = other.trustees;
        conclaveInputs = other.conclaveInputs;
        bitcoinOutputs = other.bitcoinOutputs;
        conclaveOutputs = other.conclaveOutputs;
        return *this;
    }
    
    ConclaveTx& ConclaveTx::operator=(ConclaveTx&& other)
    {
        version = other.version;
        lockTime = other.lockTime;
        minSigs = other.minSigs;
        fundPoint = std::move(other.fundPoint);
        trustees = std::move(other.trustees);
        conclaveInputs = std::move(other.conclaveInputs);
        bitcoinOutputs = std::move(other.bitcoinOutputs);
        conclaveOutputs = std::move(other.conclaveOutputs);
        return *this;
    }
    
    bool ConclaveTx::operator==(const ConclaveTx& other) const
    {
        return (version == other.version) && (lockTime == other.lockTime) && (minSigs == other.minSigs) &&
               (fundPoint == other.fundPoint) && (trustees == other.trustees) &&
               (conclaveInputs == other.conclaveInputs) && (bitcoinOutputs == other.bitcoinOutputs) &&
               (conclaveOutputs == other.conclaveOutputs);
    }
    
    bool ConclaveTx::operator!=(const ConclaveTx& other) const
    {
        return (version != other.version) || (lockTime != other.lockTime) || (minSigs != other.minSigs) ||
               (fundPoint != other.fundPoint) || (trustees != other.trustees) ||
               (conclaveInputs != other.conclaveInputs) || (bitcoinOutputs != other.bitcoinOutputs) ||
               (conclaveOutputs != other.conclaveOutputs);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveTx& conclaveTx)
    {
        os << static_cast<std::string>(conclaveTx);
        return os;
    }
}
