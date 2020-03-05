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

#include "conclave_entry_tx.h"
#include "../util/json.h"

namespace conclave
{
    const std::string ConclaveEntryTx::JSONKEY_OUTPUTS = "outputs";
    const std::string ConclaveEntryTx::JSONKEY_BITCOIN_TXID = "bitcoinTxid";
    
    ConclaveEntryTx::ConclaveEntryTx(const std::vector<ConclaveOutput>& outputs)
        : outputs(outputs), bitcoinTxid(std::nullopt)
    {
    }
    
    ConclaveEntryTx::ConclaveEntryTx(const std::vector<ConclaveOutput>& outputs, const Hash256& bitcoinTxid)
        : outputs(outputs), bitcoinTxid(bitcoinTxid)
    {
    }
    
    ConclaveEntryTx::ConclaveEntryTx(const pt::ptree& tree)
        : ConclaveEntryTx(tryGetVectorOfObjects<ConclaveOutput>(tree, JSONKEY_OUTPUTS),
                          *getOptionalPrimitiveFromJson<std::string>(tree, JSONKEY_BITCOIN_TXID))
    {
    }
    
    ConclaveEntryTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_OUTPUTS, vectorOfObjectsToArray(outputs));
        if (bitcoinTxid.has_value()) {
            tree.add<std::string>(JSONKEY_BITCOIN_TXID, *bitcoinTxid);
        }
        return tree;
    }
    
    ConclaveEntryTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool ConclaveEntryTx::operator==(const ConclaveEntryTx& other) const
    {
        return (outputs == other.outputs) && (bitcoinTxid == other.bitcoinTxid);
    }
    
    bool ConclaveEntryTx::operator!=(const ConclaveEntryTx& other) const
    {
        return (outputs != other.outputs) || (bitcoinTxid != other.bitcoinTxid);
    }
    
    std::ostream& operator<<(std::ostream& os, const ConclaveEntryTx& conclaveEntryTx)
    {
        os << static_cast<std::string>(conclaveEntryTx);
        return os;
    }
}
