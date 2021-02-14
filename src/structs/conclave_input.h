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
#include "outpoint.h"
#include "inpoint.h"
#include "../script.h"
#include "../util/json.h"
#include <optional>
#include <cstdint>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    struct ConclaveInput final
    {
        // JSON Keys
        const static std::string JSONKEY_OUTPOINT;
        const static std::string JSONKEY_SCRIPTSIG;
        const static std::string JSONKEY_SEQUENCE;
        const static std::string JSONKEY_PREDECESSOR;
        // Factories
        static ConclaveInput deserialize(const std::vector<BYTE>&, size_t&);
        static ConclaveInput deserialize(const std::vector<BYTE>&);
        // Constructors
        ConclaveInput(const Outpoint&, const Script&, const uint32_t);
        ConclaveInput(Outpoint&&, Script&&, const uint32_t);
        ConclaveInput(const Outpoint&, const Script&, const uint32_t, const Inpoint&);
        ConclaveInput(Outpoint&&, Script&&, const uint32_t, Inpoint&&);
        ConclaveInput(const Outpoint&, const Script&, const uint32_t, const std::optional<Inpoint>&);
        ConclaveInput(Outpoint&&, Script&&, const uint32_t, std::optional<Inpoint>&&);
        ConclaveInput(const pt::ptree&);
        ConclaveInput(const std::vector<BYTE>&);
        ConclaveInput(const ConclaveInput&);
        ConclaveInput(ConclaveInput&&);
        // Public Functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operators
        ConclaveInput& operator=(const ConclaveInput&);
        ConclaveInput& operator=(ConclaveInput&&);
        bool operator==(const ConclaveInput&) const;
        bool operator!=(const ConclaveInput&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveInput&);
        // Properties
        Outpoint outpoint;
        Script scriptSig;
        uint32_t sequence;
        std::optional<Inpoint> predecessor;
    };
}
