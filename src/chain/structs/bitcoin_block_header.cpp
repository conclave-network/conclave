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

#include "bitcoin_block_header.h"
#include "../../util/json.h"
#include "../../util/serialization.h"
#include <cstdint>

namespace conclave
{
    namespace chain
    {
        //
        // JSON keys
        //
        
        const std::string BitcoinBlockHeader::JSONKEY_VERSION = "version";
        const std::string BitcoinBlockHeader::JSONKEY_HASH_PREV_BLOCK = "hashPrevBlock";
        const std::string BitcoinBlockHeader::JSONKEY_HASH_MERKLE_ROOT = "hashMerkleRoot";
        const std::string BitcoinBlockHeader::JSONKEY_TIME = "time";
        const std::string BitcoinBlockHeader::JSONKEY_BITS = "bits";
        const std::string BitcoinBlockHeader::JSONKEY_NONCE = "nonce";
        
        //
        // Factories
        //
        
        BitcoinBlockHeader BitcoinBlockHeader::deserialize(const std::vector<BYTE>& data, size_t& pos)
        {
            uint32_t version = deserializeIntegral<uint32_t>(data, pos);
            Hash256 hashPrevBlock = Hash256::deserialize(data, pos);
            Hash256 hashMerkleRoot = Hash256::deserialize(data, pos);
            uint32_t time = deserializeIntegral<uint32_t>(data, pos);
            uint32_t bits = deserializeIntegral<uint32_t>(data, pos);
            uint32_t nonce = deserializeIntegral<uint32_t>(data, pos);
            return BitcoinBlockHeader(version, hashPrevBlock, hashMerkleRoot, time, bits, nonce);
        }
        
        BitcoinBlockHeader BitcoinBlockHeader::deserialize(const std::vector<BYTE>& data)
        {
            size_t pos = 0;
            return deserialize(data, pos);
        }
        
        //
        // Constructors
        //
        
        BitcoinBlockHeader::BitcoinBlockHeader(const uint32_t version, const Hash256& hashPrevBlock,
                                               const Hash256& hashMerkleRoot, const uint32_t time,
                                               const uint32_t bits, const uint32_t nonce)
            : version(version), hashPrevBlock(hashPrevBlock), hashMerkleRoot(hashMerkleRoot),
              time(time), bits(bits), nonce(nonce)
        {
        }
        
        BitcoinBlockHeader::BitcoinBlockHeader(const pt::ptree& tree)
            : BitcoinBlockHeader(
            getPrimitiveFromJson<uint32_t>(tree, JSONKEY_VERSION),
            getPrimitiveFromJson<std::string>(tree, JSONKEY_HASH_PREV_BLOCK),
            getPrimitiveFromJson<std::string>(tree, JSONKEY_HASH_MERKLE_ROOT),
            getPrimitiveFromJson<uint32_t>(tree, JSONKEY_TIME),
            getPrimitiveFromJson<uint32_t>(tree, JSONKEY_BITS),
            getPrimitiveFromJson<uint32_t>(tree, JSONKEY_NONCE))
        {
        }
        
        BitcoinBlockHeader::BitcoinBlockHeader(const std::vector<BYTE>& data)
            : BitcoinBlockHeader(deserialize(data))
        {
        }
        
        BitcoinBlockHeader::BitcoinBlockHeader(const BitcoinBlockHeader& other)
            : BitcoinBlockHeader(other.version, other.hashPrevBlock, other.hashMerkleRoot,
                                 other.time, other.bits, other.nonce)
        {
        }
        
        BitcoinBlockHeader::BitcoinBlockHeader(BitcoinBlockHeader&& other)
            : BitcoinBlockHeader(other.version, std::move(other.hashPrevBlock), std::move(other.hashMerkleRoot),
                                 other.time, other.bits, other.nonce)
        {
        }
        
        //
        // Public Functions
        //
        
        const Hash256 BitcoinBlockHeader::getHash256() const
        {
            return Hash256::digest(serialize());
        }
        
        const std::vector<BYTE> BitcoinBlockHeader::serialize() const
        {
            const std::vector<BYTE> versionSerialized = serializeIntegral(version);
            const std::vector<BYTE> hashPrevBlockSerialized = hashPrevBlock.serialize();
            const std::vector<BYTE> hashMerkleRootSerialized = hashMerkleRoot.serialize();
            const std::vector<BYTE> timeSerialized = serializeIntegral(time);
            const std::vector<BYTE> bitsSerialized = serializeIntegral(bits);
            const std::vector<BYTE> nonceSerialized = serializeIntegral(nonce);
            std::vector<BYTE> serialized(
                versionSerialized.size() + hashPrevBlockSerialized.size() +
                hashMerkleRootSerialized.size() + timeSerialized.size() +
                bitsSerialized.size() + nonceSerialized.size()
            );
            size_t pos = writeToByteVector(serialized, versionSerialized);
            pos += writeToByteVector(serialized, hashPrevBlockSerialized, pos);
            pos += writeToByteVector(serialized, hashMerkleRootSerialized, pos);
            pos += writeToByteVector(serialized, timeSerialized, pos);
            pos += writeToByteVector(serialized, bitsSerialized, pos);
            writeToByteVector(serialized, nonceSerialized, pos);
            return serialized;
        }
        
        //
        // Conversions
        //
        
        BitcoinBlockHeader::operator pt::ptree() const
        {
            pt::ptree tree;
            tree.add<uint32_t>(JSONKEY_VERSION, version);
            tree.add<std::string>(JSONKEY_HASH_PREV_BLOCK, hashPrevBlock);
            tree.add<std::string>(JSONKEY_HASH_MERKLE_ROOT, hashMerkleRoot);
            tree.add<uint32_t>(JSONKEY_TIME, time);
            tree.add<uint32_t>(JSONKEY_BITS, bits);
            tree.add<uint32_t>(JSONKEY_NONCE, nonce);
            return tree;
        }
        
        BitcoinBlockHeader::operator std::string() const
        {
            return jsonToString(static_cast<pt::ptree>(*this));
        }
        
        BitcoinBlockHeader::operator std::vector<BYTE>() const
        {
            return serialize();
        }
        
        //
        // Operator Overloads
        //
        
        BitcoinBlockHeader& BitcoinBlockHeader::operator=(const BitcoinBlockHeader& other)
        {
            version = other.version;
            hashPrevBlock = other.hashPrevBlock;
            hashMerkleRoot = other.hashMerkleRoot;
            time = other.time;
            bits = other.bits;
            nonce = other.nonce;
            return *this;
        }
        
        BitcoinBlockHeader& BitcoinBlockHeader::operator=(BitcoinBlockHeader&& other)
        {
            version = other.version;
            hashPrevBlock = std::move(other.hashPrevBlock);
            hashMerkleRoot = std::move(other.hashMerkleRoot);
            time = other.time;
            bits = other.bits;
            nonce = other.nonce;
            return *this;
        }
        
        bool BitcoinBlockHeader::operator==(const BitcoinBlockHeader& other) const
        {
            return (version == other.version) && (hashPrevBlock == other.hashPrevBlock) &&
                   (hashMerkleRoot == other.hashMerkleRoot) && (time == other.time) &&
                   (bits == other.bits) && (nonce == other.nonce);
        }
        
        bool BitcoinBlockHeader::operator!=(const BitcoinBlockHeader& other) const
        {
            return (version != other.version) || (hashPrevBlock != other.hashPrevBlock) ||
                   (hashMerkleRoot != other.hashMerkleRoot) || (time != other.time) ||
                   (bits != other.bits) || (nonce != other.nonce);
        }
        
        std::ostream& operator<<(std::ostream& os, const BitcoinBlockHeader& bitcoinBlockHeader)
        {
            os << static_cast<std::string>(bitcoinBlockHeader);
            return os;
        }
    }
}
