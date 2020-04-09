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

#include <boost/property_tree/ptree.hpp>
#include "bitcoin_output.h"
#include "outpoint.h"
#include <vector>
#include <string>

namespace conclave
{
    struct BitcoinRichOutput final
    {
        // JSON Keys
        const static std::string JSONKEY_OUTPOINT;
        const static std::string JSONKEY_BITCOIN_OUTPUT;
        // Factories
        static BitcoinRichOutput deserialize(const std::vector<BYTE>&, size_t&);
        static BitcoinRichOutput deserialize(const std::vector<BYTE>&);
        // Constructors
        BitcoinRichOutput(const Outpoint&, const BitcoinOutput&);
        BitcoinRichOutput(Outpoint&&, BitcoinOutput&&);
        BitcoinRichOutput(const pt::ptree&);
        BitcoinRichOutput(const std::vector<BYTE>&);
        BitcoinRichOutput(const BitcoinRichOutput&);
        BitcoinRichOutput(BitcoinRichOutput&&) noexcept;
        // Public Functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        BitcoinRichOutput& operator=(const BitcoinRichOutput&);
        BitcoinRichOutput& operator=(BitcoinRichOutput&&);
        bool operator==(const BitcoinRichOutput&) const;
        bool operator!=(const BitcoinRichOutput&) const;
        friend std::ostream& operator<<(std::ostream&, const BitcoinRichOutput&);
        // Properties
        Outpoint outpoint;
        BitcoinOutput bitcoinOutput;
    };
}
