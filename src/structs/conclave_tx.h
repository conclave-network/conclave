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
#include "conclave_input.h"
#include "bitcoin_output.h"
#include "conclave_output.h"
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
    struct ConclaveTx final
    {
        // JSON Keys
        const static std::string JSONKEY_VERSION;
        const static std::string JSONKEY_LOCK_TIME;
        const static std::string JSONKEY_MIN_SIGS;
        const static std::string JSONKEY_FUND_POINT;
        const static std::string JSONKEY_TRUSTEES;
        const static std::string JSONKEY_CONCLAVE_INPUTS;
        const static std::string JSONKEY_BITCOIN_OUTPUTS;
        const static std::string JSONKEY_CONCLAVE_OUTPUTS;
        // Factories
        static ConclaveTx deserialize(const std::vector<BYTE>&, size_t&);
        static ConclaveTx deserialize(const std::vector<BYTE>&);
        // Constructors
        ConclaveTx(const uint32_t, const std::vector<PublicKey>&, const std::vector<ConclaveOutput>&);
        ConclaveTx(const uint32_t, const std::vector<PublicKey>&, const std::vector<BitcoinOutput>&,
                   const std::vector<ConclaveOutput>&);
        ConclaveTx(const uint32_t, const Outpoint&, const std::vector<PublicKey>&,
                   const std::vector<BitcoinOutput>&, const std::vector<ConclaveOutput>&);
        ConclaveTx(const uint32_t, const uint32_t, const std::vector<ConclaveInput>&, const std::vector<BitcoinOutput>&,
                   const std::vector<ConclaveOutput>&);
        ConclaveTx(const uint32_t, const uint32_t, const uint32_t, const std::optional<Outpoint>&,
                   const std::vector<PublicKey>&, const std::vector<ConclaveInput>&, const std::vector<BitcoinOutput>&,
                   const std::vector<ConclaveOutput>&);
        ConclaveTx(const pt::ptree&);
        ConclaveTx(const std::vector<BYTE>&);
        ConclaveTx(const ConclaveTx&);
        ConclaveTx(ConclaveTx&&);
        // Public Functions
        const Hash256 getHash256(const bool = false) const;
        const std::vector<BYTE> serialize(const bool = false) const;
        const bool isClaimTx() const;
        const Script getClaimScript() const;
        const uint64_t getClaimedValue() const;
        // Conversions
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        operator std::vector<BYTE>() const;
        // Operator Overloads
        ConclaveTx& operator=(const ConclaveTx&);
        ConclaveTx& operator=(ConclaveTx&&);
        bool operator==(const ConclaveTx&) const;
        bool operator!=(const ConclaveTx&) const;
        friend std::ostream& operator<<(std::ostream&, const ConclaveTx&);
        // Properties
        uint32_t version;
        uint32_t lockTime;
        uint32_t minSigs;
        std::optional<Outpoint> fundPoint;
        std::vector<PublicKey> trustees;
        std::vector<ConclaveInput> conclaveInputs;
        std::vector<BitcoinOutput> bitcoinOutputs;
        std::vector<ConclaveOutput> conclaveOutputs;
    };
}