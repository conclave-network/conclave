/*
 * CONCLAVE - Scaling Bitcoin Simply.
 * Copyright (C) 2020-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>
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
#include "../hash256.h"
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
        // Factories
        static Outpoint deserialize(const std::vector<BYTE>&, size_t&);
        static Outpoint deserialize(const std::vector<BYTE>&);
        // Constructors
        Outpoint(const Hash256&, const uint32_t);
        Outpoint(const pt::ptree&);
        Outpoint(const std::vector<BYTE>&);
        Outpoint(const Outpoint&);
        Outpoint(Outpoint&&);
        // Public functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        Outpoint& operator=(const Outpoint&);
        Outpoint& operator=(Outpoint&&);
        bool operator==(const Outpoint&) const;
        bool operator!=(const Outpoint&) const;
        friend std::ostream& operator<<(std::ostream&, const Outpoint&);
        // Properties
        Hash256 txId;
        uint32_t index;
    };
}
