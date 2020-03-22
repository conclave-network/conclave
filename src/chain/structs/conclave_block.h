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

#include "../../structs/conclave_claim_tx.h"
#include "../../structs/conclave_standard_tx.h"
#include "../../hash256.h"
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <string>
#include <optional>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        struct ConclaveBlock final
        {
            // JSON keys
            const static std::string JSONKEY_HEIGHT;
            const static std::string JSONKEY_PREV_BLOCK_HASH;
            const static std::string JSONKEY_LOWEST_PARENT_BITCOIN_BLOCK_HASH;
            const static std::string JSONKEY_CONCLAVE_TX;
            // Constructors
            // Properties
            uint64_t height;
            uint64_t time;
            uint32_t txTypeId;
            uint32_t txVersion;
            Hash256 hashPrevBlock;
            Hash256 lowestParentBitcoinBlockHash;
            std::optional<ConclaveClaimTx> concalveClaimTx;
            std::optional<ConclaveStandardTx> conclaveStandardTx;
        };
    }
}
