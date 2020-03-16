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

#include "entry_tx.h"

namespace conclave
{
    const std::string EntryTx::JSONKEY_FUND_TX = "FundTx";
    const std::string EntryTx::JSONKEY_CLAIM_TX = "ClaimTx";
    
    EntryTx::EntryTx(const BitcoinTx& fundTx, const ClaimTx& claimTx)
        : fundTx(fundTx), claimTx(claimTx)
    {
    }
    
    EntryTx::EntryTx(const pt::ptree& tree)
        : EntryTx(getObjectFromJson<BitcoinTx>(tree, JSONKEY_FUND_TX),
                  getObjectFromJson<ClaimTx>(tree, JSONKEY_CLAIM_TX))
    {
    }
    
    EntryTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_FUND_TX, static_cast<pt::ptree>(fundTx));
        tree.add_child(JSONKEY_CLAIM_TX, static_cast<pt::ptree>(claimTx));
        return tree;
    }
    
    EntryTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool EntryTx::operator==(const EntryTx& other) const
    {
        return (fundTx == other.fundTx) && (claimTx == other.claimTx);
    }
    
    bool EntryTx::operator!=(const EntryTx& other) const
    {
        return (fundTx != other.fundTx) || (claimTx != other.claimTx);
    }
    
    std::ostream& operator<<(std::ostream& os, const EntryTx& entryTx)
    {
        os << static_cast<std::string>(entryTx);
        return os;
    }
}
