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

#include "bitcoin_output.h"
#include "../util/json.h"

namespace conclave
{
    const std::string BitcoinOutput::JSONKEY_SCRIPTPUBKEY = "scriptPubKey";
    const std::string BitcoinOutput::JSONKEY_VALUE = "value";
    
    BitcoinOutput::BitcoinOutput(const Script& scriptPubKey, const uint64_t value)
        : scriptPubKey(scriptPubKey), value(value)
    {
    }
    
    BitcoinOutput::BitcoinOutput(const pt::ptree& tree)
        : BitcoinOutput(getObjectFromJson<Script>(tree, JSONKEY_SCRIPTPUBKEY),
                        getPrimitiveFromJson<uint64_t>(tree, JSONKEY_VALUE))
    {
    }
    
    BitcoinOutput::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add_child(JSONKEY_SCRIPTPUBKEY, (pt::ptree) scriptPubKey);
        tree.add<uint64_t>(JSONKEY_VALUE, value);
        return tree;
    }
    
    BitcoinOutput::operator std::string() const
    {
        return jsonToString(static_cast<pt::ptree>(*this));
    }
    
    bool BitcoinOutput::operator==(const BitcoinOutput& other) const
    {
        return (scriptPubKey == other.scriptPubKey) && (value == other.value);
    }
    
    bool BitcoinOutput::operator!=(const BitcoinOutput& other) const
    {
        return (scriptPubKey != other.scriptPubKey) || (value != other.value);
    }
    
    std::ostream& operator<<(std::ostream& os, const BitcoinOutput& bitcoinOutput)
    {
        os << static_cast<std::string>(bitcoinOutput);
        return os;
    }
}
