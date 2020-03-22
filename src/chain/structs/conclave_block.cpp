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
        
        //
        // Constructors
        //
        
        ConclaveBlock::ConclaveBlock(const uint64_t pot, const uint64_t height, const uint32_t epoch,
                                     const Hash256& hashPrevBlock, const Hash256& lowestParentBitcoinBlockHash,
                                     const uint16_t txTypeId, const uint16_t txVersion, const Hash256& txHash)
            : pot(pot), height(height), epoch(epoch), hashPrevBlock(hashPrevBlock),
              lowestParentBitcoinBlockHash(lowestParentBitcoinBlockHash),
              txTypeId(txTypeId), txVersion(txVersion), txHash(Hash)
        {
        }
        
        //
        // Public Functions
        //
        
        
        
    }
}
