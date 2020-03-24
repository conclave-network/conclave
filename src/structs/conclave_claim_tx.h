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
#include "../hash256.h"
#include "../public_key.h"
#include "../util/serialization.h"
#include "../conclave.h"
#include <optional>
#include <cstdint>
#include <string>

namespace pt = boost::property_tree;
namespace conclave
{
    struct ConclaveClaimTx final
    {
        // JSON keys
        const static std::string JSONKEY_OUTPUTS;
        const static std::string JSONKEY_TRUSTEES;
        const static std::string JSONKEY_MIN_SIGS;
        const static std::string JSONKEY_FUNDING_OUTPOINT;
        // Factories
        static ConclaveClaimTx deserialize(const std::vector<BYTE>&, size_t&);
        static ConclaveClaimTx deserialize(const std::vector<BYTE>&);
        // Constructors
        ConclaveClaimTx(const std::vector<ConclaveOutput>&, const std::vector<PublicKey>&, const uint32_t);
        ConclaveClaimTx(const std::vector<ConclaveOutput>&, const std::vector<PublicKey>&,
                        const uint32_t, const Outpoint&);
        ConclaveClaimTx(const pt::ptree&);
        ConclaveClaimTx(const std::vector<BYTE>&);
        ConclaveClaimTx(const ConclaveClaimTx&);
        ConclaveClaimTx(ConclaveClaimTx&&);
        // Public functions
        const Hash256 getHash256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        // Operator Overloads
        ConclaveClaimTx& operator=(const ConclaveClaimTx&);
        ConclaveClaimTx& operator=(ConclaveClaimTx&&);
        bool operator==(const ConclaveClaimTx&) const;
        bool operator!=(const ConclaveClaimTx&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveClaimTx&);
        // Properties
        std::vector<ConclaveOutput> conclaveOutputs;
        std::vector<PublicKey> trustees;
        uint32_t minSigs;
        std::optional<Outpoint> fundingOutpoint;
    };
}
