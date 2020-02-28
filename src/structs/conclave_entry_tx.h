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
#include "../hash256.h"
#include "../conclave.h"
#include <optional>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    struct ConclaveEntryTx final
    {
        // JSON keys
        const static std::string JSONKEY_CONCLAVE_OUTPUTS;
        const static std::string JSONKEY_BITCOIN_TXID;
        // Constructors
        ConclaveEntryTx(const std::vector<BitcoinOutput>&);
        ConclaveEntryTx(const std::vector<BitcoinOutput>&, const Hash256&);
        ConclaveEntryTx(const pt::ptree&);
        // Operators
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        bool operator==(const ConclaveEntryTx&) const;
        bool operator!=(const ConclaveEntryTx&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveEntryTx&);
        // Properties
        const std::vector<BitcoinOutput> conclaveOutputs;
        const std::optional<Hash256> bitcoinTxid;
    };
}
