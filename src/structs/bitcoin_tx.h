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
#include "bitcoin_input.h"
#include "bitcoin_output.h"
#include <cstdint>
#include <vector>

namespace conclave
{
    struct BitcoinTx final
    {
        // JSON Keys
        const static std::string JSONKEY_VERSION;
        const static std::string JSONKEY_INPUTS;
        const static std::string JSONKEY_OUTPUTS;
        const static std::string JSONKEY_LOCKTIME;
        // Factories
        static BitcoinTx deserialize(const std::vector<BYTE>&, size_t&);
        static BitcoinTx deserialize(const std::vector<BYTE>&);
        // Constructors
        BitcoinTx(const uint32_t, const std::vector<BitcoinInput>&, const std::vector<BitcoinOutput>&, const uint32_t);
        BitcoinTx(const uint32_t, std::vector<BitcoinInput>&&, std::vector<BitcoinOutput>&&, const uint32_t);
        BitcoinTx(const pt::ptree&);
        BitcoinTx(const std::vector<BYTE>&);
        BitcoinTx(const BitcoinTx&);
        BitcoinTx(BitcoinTx&&);
        // Public Functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        BitcoinTx& operator=(const BitcoinTx&);
        BitcoinTx& operator=(BitcoinTx&&);
        bool operator==(const BitcoinTx&) const;
        bool operator!=(const BitcoinTx&) const;
        friend std::ostream& operator<<(std::ostream&, const BitcoinTx&);
        // Properties
        uint32_t version;
        std::vector<BitcoinInput> inputs;
        std::vector<BitcoinOutput> outputs;
        uint32_t lockTime;
    };
}
