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

#pragma once

#include "../../hash256.h"
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <string>
#include <optional>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        struct ConclaveBlock final
        {
            // JSON keys
            const static std::string JSONKEY_POT;
            const static std::string JSONKEY_HEIGHT;
            const static std::string JSONKEY_EPOCH;
            const static std::string JSONKEY_HASH_PREV_BLOCK;
            const static std::string JSONKEY_LOWEST_PARENT_BITCOIN_BLOCK_HASH;
            const static std::string JSONKEY_TX_TYPE_ID;
            const static std::string JSONKEY_TX_VERSION;
            const static std::string JSONKEY_TX_HASH;
            // Factories
            static ConclaveBlock deserialize(const std::vector<BYTE>&, size_t&);
            static ConclaveBlock deserialize(const std::vector<BYTE>&);
            // Constructors
            ConclaveBlock(const uint64_t, const uint64_t, const uint32_t, const Hash256&,
                          const Hash256&, const uint16_t, const uint16_t, const Hash256&);
            ConclaveBlock(const pt::ptree&);
            ConclaveBlock(const std::vector<BYTE>&);
            ConclaveBlock(const ConclaveBlock&);
            ConclaveBlock(ConclaveBlock&&);
            // Public Functions
            const Hash256 getHash256() const;
            const std::vector<BYTE> serialize() const;
            // Conversions
            explicit operator pt::ptree() const;
            explicit operator std::string() const;
            operator std::vector<BYTE>() const;
            // Operator Overloads
            ConclaveBlock& operator=(const ConclaveBlock&);
            ConclaveBlock& operator=(ConclaveBlock&&);
            bool operator==(const ConclaveBlock&) const;
            bool operator!=(const ConclaveBlock&) const;
            friend std::ostream& operator<<(std::ostream&, const ConclaveBlock&);
            // Properties
            uint64_t pot;
            uint64_t height;
            uint32_t epoch;
            Hash256 hashPrevBlock;
            Hash256 lowestParentBitcoinBlockHash;
            uint16_t txTypeId;
            uint16_t txVersion;
            Hash256 txHash;
        };
    }
}
