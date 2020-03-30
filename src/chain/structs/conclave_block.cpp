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

#include "conclave_block.h"
#include "../../util/json.h"
#include "../../util/serialization.h"
#include <cstdint>

namespace conclave
{
    namespace chain
    {
        //
        // JSON Keys
        //
        
        const std::string ConclaveBlock::JSONKEY_POT = "pot";
        const std::string ConclaveBlock::JSONKEY_HEIGHT = "height";
        const std::string ConclaveBlock::JSONKEY_EPOCH = "epoch";
        const std::string ConclaveBlock::JSONKEY_HASH_PREV_BLOCK = "hashPrevBlock";
        const std::string ConclaveBlock::JSONKEY_LOWEST_PARENT_BITCOIN_BLOCK_HASH = "lowestParentBitcoinBlockHash";
        const std::string ConclaveBlock::JSONKEY_TX_TYPE_ID = "txTypeId";
        const std::string ConclaveBlock::JSONKEY_TX_VERSION = "txVersion";
        const std::string ConclaveBlock::JSONKEY_TX_HASH = "txHash";
        
        //
        // Factories
        //
        
        ConclaveBlock ConclaveBlock::deserialize(const std::vector<BYTE>& data, size_t& pos)
        {
            uint64_t pot = deserializeIntegral<uint64_t>(data, pos);
            uint64_t height = deserializeIntegral<uint64_t>(data, pos);
            uint32_t epoch = deserializeIntegral<uint32_t>(data, pos);
            Hash256 hashPrevBlock = Hash256::deserialize(data, pos);
            Hash256 lowestParentBitcoinBlockHash = Hash256::deserialize(data, pos);
            uint16_t txTypeId = deserializeIntegral<uint16_t>(data, pos);
            uint16_t txVersion = deserializeIntegral<uint16_t>(data, pos);
            Hash256 txHash = Hash256::deserialize(data, pos);
            return ConclaveBlock(pot, height, epoch, hashPrevBlock,
                                 lowestParentBitcoinBlockHash, txTypeId, txVersion, txHash);
        }
        
        ConclaveBlock ConclaveBlock::deserialize(const std::vector<BYTE>& data)
        {
            size_t pos = 0;
            return deserialize(data, pos);
        }
        
        //
        // Constructors
        //
        
        ConclaveBlock::ConclaveBlock(const uint64_t pot, const uint64_t height, const uint32_t epoch,
                                     const Hash256& hashPrevBlock, const Hash256& lowestParentBitcoinBlockHash,
                                     const uint16_t txTypeId, const uint16_t txVersion, const Hash256& txHash)
            : pot(pot), height(height), epoch(epoch), hashPrevBlock(hashPrevBlock),
              lowestParentBitcoinBlockHash(lowestParentBitcoinBlockHash),
              txTypeId(txTypeId), txVersion(txVersion), txHash(txHash)
        {
        }
        
        ConclaveBlock::ConclaveBlock(const pt::ptree& tree)
            : ConclaveBlock(
            getPrimitiveFromJson<uint64_t>(tree, JSONKEY_POT),
            getPrimitiveFromJson<uint64_t>(tree, JSONKEY_HEIGHT),
            getPrimitiveFromJson<uint32_t>(tree, JSONKEY_EPOCH),
            getPrimitiveFromJson<std::string>(tree, JSONKEY_HASH_PREV_BLOCK),
            getPrimitiveFromJson<std::string>(tree, JSONKEY_LOWEST_PARENT_BITCOIN_BLOCK_HASH),
            getPrimitiveFromJson<uint16_t>(tree, JSONKEY_TX_TYPE_ID),
            getPrimitiveFromJson<uint16_t>(tree, JSONKEY_TX_VERSION),
            getPrimitiveFromJson<std::string>(tree, JSONKEY_TX_HASH))
        {
        }
        
        ConclaveBlock::ConclaveBlock(const std::vector<BYTE>& data)
            : ConclaveBlock(deserialize(data))
        {
        }
        
        ConclaveBlock::ConclaveBlock(const ConclaveBlock& other)
            : ConclaveBlock(other.pot, other.height, other.epoch, other.hashPrevBlock,
                            other.lowestParentBitcoinBlockHash, other.txTypeId, other.txVersion, other.txHash)
        {
        }
        
        ConclaveBlock::ConclaveBlock(ConclaveBlock&& other)
            : ConclaveBlock(other.pot, other.height, other.epoch, std::move(other.hashPrevBlock),
                            std::move(other.lowestParentBitcoinBlockHash), other.txTypeId,
                            other.txVersion, std::move(other.txHash))
        {
        }
        
        //
        // Public Functions
        //
        
        const Hash256 ConclaveBlock::getHash256() const
        {
            return Hash256::digest(serialize());
        }
        
        const std::vector<BYTE> ConclaveBlock::serialize() const
        {
            const std::vector<BYTE> potSerialized = serializeIntegral(pot);
            const std::vector<BYTE> heightSerialized = serializeIntegral(height);
            const std::vector<BYTE> epochSerialized = serializeIntegral(epoch);
            const std::vector<BYTE> hashPrevBlockSerialized = hashPrevBlock.serialize();
            const std::vector<BYTE> lowestParentBitcoinBlockHashSerialized = lowestParentBitcoinBlockHash.serialize();
            const std::vector<BYTE> txTypeIdSerialized = serializeIntegral(txTypeId);
            const std::vector<BYTE> txVersionSerialized = serializeIntegral(txVersion);
            const std::vector<BYTE> txHashSerialized = txHash.serialize();
            std::vector<BYTE> serialized(
                potSerialized.size() + heightSerialized.size() + epochSerialized.size() +
                hashPrevBlockSerialized.size() + lowestParentBitcoinBlockHashSerialized.size() +
                txTypeIdSerialized.size() + txVersionSerialized.size() + txHashSerialized.size()
            );
            size_t pos = 0;
            writeToByteVector(serialized, potSerialized, pos);
            writeToByteVector(serialized, heightSerialized, pos);
            writeToByteVector(serialized, epochSerialized, pos);
            writeToByteVector(serialized, hashPrevBlockSerialized, pos);
            writeToByteVector(serialized, lowestParentBitcoinBlockHashSerialized, pos);
            writeToByteVector(serialized, txTypeIdSerialized, pos);
            writeToByteVector(serialized, txVersionSerialized, pos);
            writeToByteVector(serialized, txHashSerialized, pos);
            return serialized;
        }
        
        //
        // Conversions
        //
        
        ConclaveBlock::operator pt::ptree() const
        {
            pt::ptree tree;
            tree.add<uint64_t>(JSONKEY_POT, pot);
            tree.add<uint64_t>(JSONKEY_HEIGHT, height);
            tree.add<uint32_t>(JSONKEY_EPOCH, epoch);
            tree.add<std::string>(JSONKEY_HASH_PREV_BLOCK, hashPrevBlock);
            tree.add<std::string>(JSONKEY_LOWEST_PARENT_BITCOIN_BLOCK_HASH, lowestParentBitcoinBlockHash);
            tree.add<uint16_t>(JSONKEY_TX_TYPE_ID, txTypeId);
            tree.add<uint16_t>(JSONKEY_TX_VERSION, txVersion);
            tree.add<std::string>(JSONKEY_TX_HASH, txHash);
            return tree;
        }
        
        ConclaveBlock::operator std::string() const
        {
            return jsonToString(static_cast<pt::ptree>(*this));
        }
        
        ConclaveBlock::operator std::vector<BYTE>() const
        {
            return serialize();
        }
        
        //
        // Operator Overloads
        //
        
        ConclaveBlock& ConclaveBlock::operator=(const ConclaveBlock& other)
        {
            pot = other.pot;
            height = other.height;
            epoch = other.epoch;
            hashPrevBlock = other.hashPrevBlock;
            lowestParentBitcoinBlockHash = other.lowestParentBitcoinBlockHash;
            txTypeId = other.txTypeId;
            txVersion = other.txVersion;
            txHash = other.txHash;
            return *this;
        }
        
        ConclaveBlock& ConclaveBlock::operator=(ConclaveBlock&& other)
        {
            pot = other.pot;
            height = other.height;
            epoch = other.epoch;
            hashPrevBlock = std::move(other.hashPrevBlock);
            lowestParentBitcoinBlockHash = std::move(other.lowestParentBitcoinBlockHash);
            txTypeId = other.txTypeId;
            txVersion = other.txVersion;
            txHash = std::move(other.txHash);
            return *this;
        }
        
        bool ConclaveBlock::operator==(const ConclaveBlock& other) const
        {
            return (pot == other.pot) && (height == other.height) && (epoch == other.epoch) &&
                   (hashPrevBlock == other.hashPrevBlock) &&
                   (lowestParentBitcoinBlockHash == other.lowestParentBitcoinBlockHash) &&
                   (txTypeId == other.txTypeId) && (txVersion == other.txVersion) && (txHash == other.txHash);
        }
        
        bool ConclaveBlock::operator!=(const ConclaveBlock& other) const
        {
            return (pot != other.pot) || (height != other.height) || (epoch != other.epoch) ||
                   (hashPrevBlock != other.hashPrevBlock) ||
                   (lowestParentBitcoinBlockHash != other.lowestParentBitcoinBlockHash) ||
                   (txTypeId != other.txTypeId) || (txVersion != other.txVersion) || (txHash != other.txHash);
        }
        
        std::ostream& operator<<(std::ostream& os, const ConclaveBlock& conclaveBlock)
        {
            os << static_cast<std::string>(conclaveBlock);
            return os;
        }
    }
}
