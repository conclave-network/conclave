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

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        struct BitcoinBlockHeader final
        {
            // JSON Keys
            const static std::string JSONKEY_VERSION;
            const static std::string JSONKEY_HASH_PREV_BLOCK;
            const static std::string JSONKEY_HASH_MERKLE_ROOT;
            const static std::string JSONKEY_TIME;
            const static std::string JSONKEY_BITS;
            const static std::string JSONKEY_NONCE;
            // Factories
            static BitcoinBlockHeader deserialize(const std::vector<BYTE>&, size_t&);
            static BitcoinBlockHeader deserialize(const std::vector<BYTE>&);
            // Constructors
            BitcoinBlockHeader(const uint32_t, const Hash256&, const Hash256&,
                               const uint32_t, const uint32_t, const uint32_t);
            BitcoinBlockHeader(const uint32_t, Hash256&&, Hash256&&,
                               const uint32_t, const uint32_t, const uint32_t);
            BitcoinBlockHeader(const pt::ptree&);
            BitcoinBlockHeader(const std::vector<BYTE>&);
            BitcoinBlockHeader(const BitcoinBlockHeader&);
            BitcoinBlockHeader(BitcoinBlockHeader&&);
            // Public Functions
            const Hash256 getHash256() const;
            const std::vector<BYTE> serialize() const;
            // Conversions
            explicit operator pt::ptree() const;
            explicit operator std::string() const;
            operator std::vector<BYTE>() const;
            // Operator Overloads
            BitcoinBlockHeader& operator=(const BitcoinBlockHeader&);
            BitcoinBlockHeader& operator=(BitcoinBlockHeader&&);
            bool operator==(const BitcoinBlockHeader&) const;
            bool operator!=(const BitcoinBlockHeader&) const;
            friend std::ostream& operator<<(std::ostream&, const BitcoinBlockHeader&);
            // Properties
            uint32_t version;
            Hash256 hashPrevBlock;
            Hash256 hashMerkleRoot;
            uint32_t time;
            uint32_t bits;
            uint32_t nonce;
        };
    }
}
