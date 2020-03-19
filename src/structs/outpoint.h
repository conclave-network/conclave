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

#include "../conclave.h"
#include "../util/json.h"
#include "../hash256.h"
#include "../util/serialization.h"
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace pt = boost::property_tree;
namespace conclave
{
    struct Outpoint final
    {
        // JSON keys
        const static std::string JSONKEY_TXID;
        const static std::string JSONKEY_INDEX;
        // Constructors
        Outpoint(const Hash256&, const uint32_t);
        Outpoint(const pt::ptree&);
        Outpoint(const std::vector<BYTE>&);
        // Public functions
        const std::vector<BYTE> serialize() const;
        // Operators
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        explicit operator std::vector<BYTE>() const;
        bool operator==(const Outpoint&) const;
        bool operator!=(const Outpoint&) const;
        friend std::ostream& operator<<(std::ostream&, const Outpoint&);
        // Properties
        const Hash256 txId;
        const uint32_t index;
    };
}
