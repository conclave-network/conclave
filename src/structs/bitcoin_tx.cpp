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

#include "bitcoin_tx.h"
#include "../util/json.h"

namespace conclave
{
    const std::string BitcoinTx::JSONKEY_INPUTS = "inputs";
    const std::string BitcoinTx::JSONKEY_OUTPUTS = "outputs";
    const std::string BitcoinTx::JSONKEY_VERSION = "version";
    const std::string BitcoinTx::JSONKEY_LOCKTIME = "lockTime";
    
    BitcoinTx::BitcoinTx(const std::vector<BitcoinInput>& inputs,
                         const std::vector<BitcoinOutput>& outputs,
                         const uint32_t version,
                         const uint32_t lockTime)
        : inputs(inputs), outputs(outputs), version(version), lockTime(lockTime)
    {
    }
    
    BitcoinTx::BitcoinTx(const pt::ptree& tree)
        : BitcoinTx(tryGetVectorOfObjects<BitcoinInput>(tree, JSONKEY_INPUTS),
                    tryGetVectorOfObjects<BitcoinOutput>(tree, JSONKEY_OUTPUTS),
                    getPrimitiveFromJson<uint32_t>(tree, JSONKEY_VERSION),
                    getPrimitiveFromJson<uint32_t>(tree, JSONKEY_LOCKTIME))
    {
    }
    
    BitcoinTx::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_INPUTS, vectorOfObjectsToArray(inputs));
        tree.add_child(JSONKEY_OUTPUTS, vectorOfObjectsToArray(outputs));
        tree.add<uint32_t>(JSONKEY_VERSION, version);
        tree.add<uint32_t>(JSONKEY_LOCKTIME, version);
        return tree;
    }
    
    BitcoinTx::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool BitcoinTx::operator==(const BitcoinTx& other) const
    {
        return (inputs == other.inputs) && (outputs == other.outputs) &&
               (version == other.version) && (lockTime == other.lockTime);
    }
    
    bool BitcoinTx::operator!=(const BitcoinTx& other) const
    {
        return (inputs != other.inputs) || (outputs != other.outputs) ||
               (version != other.version) || (lockTime != other.lockTime);
    }
    
    std::ostream& operator<<(std::ostream& os, const BitcoinTx& bitcoinTx)
    {
        os << static_cast<std::string>(bitcoinTx);
        return os;
    }
}
