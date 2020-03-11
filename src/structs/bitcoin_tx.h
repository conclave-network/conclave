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
#include "bitcoin_input.h"
#include "bitcoin_output.h"
#include <cstdint>
#include <vector>

namespace conclave
{
    struct BitcoinTx final
    {
        // JSON keys
        const static std::string JSONKEY_VERSION;
        const static std::string JSONKEY_INPUTS;
        const static std::string JSONKEY_OUTPUTS;
        const static std::string JSONKEY_LOCKTIME;
        // Constructors
        BitcoinTx(const uint32_t version,
                  const std::vector<BitcoinInput>& inputs,
                  const std::vector<BitcoinOutput>& outputs,
                  const uint32_t lockTime);
        BitcoinTx(const pt::ptree&);
        // Public functions
        const std::vector<BYTE> serialize() const;
        const Hash256 getHash256() const;
        // Operators
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        explicit operator std::vector<BYTE>() const;
        bool operator==(const BitcoinTx&) const;
        bool operator!=(const BitcoinTx&) const;
        friend std::ostream& operator<<(std::ostream&, const BitcoinTx&);
        // Properties
        const uint32_t version;
        const std::vector<BitcoinInput> inputs;
        const std::vector<BitcoinOutput> outputs;
        const uint32_t lockTime;
    };
}
