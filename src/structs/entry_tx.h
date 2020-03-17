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

#include "bitcoin_tx.h"
#include "conclave_claim_tx.h"

namespace conclave
{
    struct EntryTx final
    {
        // JSON keys
        const static std::string JSONKEY_FUND_TX;
        const static std::string JSONKEY_CLAIM_TX;
        // Constructors
        EntryTx(const BitcoinTx&, const ConclaveClaimTx&);
        EntryTx(const pt::ptree&);
        // Operators
        explicit operator pt::ptree() const;
        explicit operator std::string() const;
        bool operator==(const EntryTx&) const;
        bool operator!=(const EntryTx&) const;
        friend std::ostream& operator<<(std::ostream&, const EntryTx&);
        // Properties
        const BitcoinTx fundTx;
        const ConclaveClaimTx claimTx;
    };
}
