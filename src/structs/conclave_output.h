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
#include "outpoint.h"
#include "../script.h"
#include "../util/json.h"
#include <optional>
#include <cstdint>
#include <string>

namespace conclave
{
    struct ConclaveOutput final
    {
        // JSON Keys
        const static std::string JSONKEY_SCRIPTPUBKEY;
        const static std::string JSONKEY_VALUE;
        const static std::string JSONKEY_PREDECESSOR;
        // Factories
        static ConclaveOutput deserialize(const std::vector<BYTE>&, size_t&);
        static ConclaveOutput deserialize(const std::vector<BYTE>&);
        // Constructors
        ConclaveOutput(const Script&, const uint64_t);
        ConclaveOutput(Script&&, const uint64_t);
        ConclaveOutput(const Script&, const uint64_t, const Outpoint&);
        ConclaveOutput(Script&&, const uint64_t, Outpoint&&);
        ConclaveOutput(const Script&, const uint64_t, const std::optional<Outpoint>&);
        ConclaveOutput(Script&&, const uint64_t, std::optional<Outpoint>&&);
        ConclaveOutput(const pt::ptree&);
        ConclaveOutput(const std::vector<BYTE>&);
        ConclaveOutput(const ConclaveOutput&);
        ConclaveOutput(ConclaveOutput&&);
        // Public Functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        ConclaveOutput& operator=(const ConclaveOutput&);
        ConclaveOutput& operator=(ConclaveOutput&&);
        bool operator==(const ConclaveOutput&) const;
        bool operator!=(const ConclaveOutput&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveOutput&);
        // Properties
        Script scriptPubKey;
        uint64_t value;
        std::optional<Outpoint> predecessor;
    };
}
