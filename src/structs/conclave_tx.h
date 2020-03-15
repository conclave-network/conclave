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
#include "conclave_input.h"
#include "conclave_output.h"
#include "bitcoin_output.h"
#include "../hash256.h"
#include <cstdint>
#include <string>
#include <vector>

namespace pt = boost::property_tree;
namespace conclave
{
    struct ConclaveTx final
    {
        // JSON keys
        const static std::string JSONKEY_VERSION;
        const static std::string JSONKEY_INPUTS;
        const static std::string JSONKEY_CONCLAVE_OUTPUTS;
        const static std::string JSONKEY_BITCOIN_OUTPUTS;
        const static std::string JSONKEY_LOCKTIME;
        // Constructors
        ConclaveTx(const uint32_t,
                   const std::vector<ConclaveInput>&,
                   const std::vector<ConclaveOutput>&,
                   const std::vector<BitcoinOutput>&,
                   const uint32_t);
        ConclaveTx(const pt::ptree&);
        // Public functions
        const std::vector<BYTE> serialize() const;
        const Hash256 getHash256() const;
        // Operators
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        bool operator==(const ConclaveTx&) const;
        bool operator!=(const ConclaveTx&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveTx&);
        // Properties
        const uint32_t version;
        const std::vector<ConclaveInput> inputs;
        const std::vector<ConclaveOutput> conclaveOutputs;
        const std::vector<BitcoinOutput> bitcoinOutputs;
        const uint32_t lockTime;
    };
}
