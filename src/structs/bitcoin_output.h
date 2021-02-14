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

#include <boost/property_tree/ptree.hpp>
#include "../script.h"
#include "../util/json.h"
#include <cstdint>
#include <string>

namespace conclave
{
    struct BitcoinOutput final
    {
        // JSON keys
        const static std::string JSONKEY_VALUE;
        const static std::string JSONKEY_SCRIPTPUBKEY;
        // Factories
        static BitcoinOutput deserialize(const std::vector<BYTE>&, size_t&);
        static BitcoinOutput deserialize(const std::vector<BYTE>&);
        // Constructors
        BitcoinOutput(const uint64_t, const Script&);
        BitcoinOutput(const uint64_t, Script&&);
        BitcoinOutput(const pt::ptree&);
        BitcoinOutput(const std::vector<BYTE>&);
        BitcoinOutput(const BitcoinOutput&);
        BitcoinOutput(BitcoinOutput&&);
        // Public functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        BitcoinOutput& operator=(const BitcoinOutput&);
        BitcoinOutput& operator=(BitcoinOutput&&);
        bool operator==(const BitcoinOutput&) const;
        bool operator!=(const BitcoinOutput&) const;
        friend std::ostream& operator<<(std::ostream&, const BitcoinOutput&);
        // Properties
        uint64_t value;
        Script scriptPubKey;
    };
}
