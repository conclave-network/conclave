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

#pragma once

#include "outpoint.h"
#include "../script.h"
#include "../util/json.h"
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    struct BitcoinInput final
    {
        // JSON Keys
        const static std::string JSONKEY_OUTPOINT;
        const static std::string JSONKEY_SCRIPTSIG;
        const static std::string JSONKEY_SEQUENCE;
        // Factories
        static BitcoinInput deserialize(const std::vector<BYTE>&, size_t&);
        static BitcoinInput deserialize(const std::vector<BYTE>&);
        // Constructors
        BitcoinInput(const Outpoint&, const Script&, const uint32_t);
        BitcoinInput(Outpoint&&, Script&&, const uint32_t);
        BitcoinInput(const pt::ptree&);
        BitcoinInput(const std::vector<BYTE>&);
        BitcoinInput(const BitcoinInput&);
        BitcoinInput(BitcoinInput&&);
        // Public Functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        BitcoinInput& operator=(const BitcoinInput&);
        BitcoinInput& operator=(BitcoinInput&&);
        bool operator==(const BitcoinInput&) const;
        bool operator!=(const BitcoinInput&) const;
        friend std::ostream& operator<<(std::ostream&, const BitcoinInput&);
        // Properties
        Outpoint outpoint;
        Script scriptSig;
        uint32_t sequence;
    };
}
