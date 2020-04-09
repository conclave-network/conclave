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
#include "conclave_output.h"
#include "outpoint.h"
#include <vector>
#include <string>

namespace conclave
{
    struct ConclaveRichOutput final
    {
        // JSON Keys
        const static std::string JSONKEY_OUTPOINT;
        const static std::string JSONKEY_CONCLAVE_OUTPUT;
        // Factories
        static ConclaveRichOutput deserialize(const std::vector<BYTE>&, size_t&);
        static ConclaveRichOutput deserialize(const std::vector<BYTE>&);
        // Constructors
        ConclaveRichOutput(const Outpoint&, const ConclaveOutput&);
        ConclaveRichOutput(Outpoint&&, ConclaveOutput&&);
        ConclaveRichOutput(const pt::ptree&);
        ConclaveRichOutput(const std::vector<BYTE>&);
        ConclaveRichOutput(const ConclaveRichOutput&);
        ConclaveRichOutput(ConclaveRichOutput&&) noexcept;
        // Public Functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        ConclaveRichOutput& operator=(const ConclaveRichOutput&);
        ConclaveRichOutput& operator=(ConclaveRichOutput&&);
        bool operator==(const ConclaveRichOutput&) const;
        bool operator!=(const ConclaveRichOutput&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveRichOutput&);
        // Properties
        Outpoint outpoint;
        ConclaveOutput conclaveOutput;
    };
}
