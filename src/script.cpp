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

#include <boost/algorithm/string/split.hpp>
#include "util/hex.h"
#include "util/json.h"
#include "script.h"

namespace conclave
{
    using namespace bc::system;
    using namespace bc::system::machine;
    
    //
    // Helpers for constructors
    //
    
    inline const static
    std::vector<machine::operation> stringVectorToMachineOpVector(const std::vector<std::string>& sv)
    {
        std::vector<machine::operation> ops(sv.size());
        for (size_t i = 0; i < sv.size(); i++) {
            ops[i].from_string(sv[i]);
        }
        return ops;
    }
    
    inline const static
    std::vector<std::string> scriptStrToScriptStrVector(const std::string& str)
    {
        std::vector<std::string> vec;
        boost::split(vec, str, boost::is_any_of(" \r\n\t"), boost::token_compress_on);
        return vec;
    }
    
    //
    // Helpers for casts
    //
    
    inline const static
    std::vector<std::string> machineOpVectorToStringVector(const std::vector<machine::operation>& ops)
    {
        std::vector<std::string> sv(ops.size());
        for (size_t i = 0; i < ops.size(); i++) {
            sv[i] = ops[i].to_string(0);
        }
        return sv;
    }
    
    /***
     * Convenience factory which checks what kind of address was passed and
     * constructs a script with the appropriate structure: P2PKH, P2WSH,... etc.
     * If the type of address is known, consider using one of the specific
     * factories instead.
     * @param address - The address to which the script will pay
     * @return - The script which encumbers monies paid to the address
     */
    Script Script::p2hScript(const Address& address)
    {
        if (address.isStandard() || address.isConclave()) {
            if (address.isToPublicKeyHash()) {
                return p2pkhScript(address);
            } else if (address.isToScriptHash()) {
                return p2shScript(address);
            }
        } else if (address.isBech32()) {
            if (address.isToPublicKeyHash()) {
                return p2wpkhScript(address);
            } else if (address.isToScriptHash()) {
                return p2wshScript(address);
            }
        }
        throw std::logic_error("Could not create script which pays to address: " + (std::string) address);
    }
    
    Script Script::p2pkhScript(const Address& address)
    {
        return Script({
                          opcode::dup,
                          opcode::hash160,
                          address.getHashData(),
                          opcode::equalverify,
                          opcode::checksig
                      });
    }
    
    Script Script::p2shScript(const Address& address)
    {
        return Script({
                          opcode::hash160,
                          address.getHashData(),
                          opcode::equal
                      });
    }
    
    Script Script::p2wpkhScript(const Address& address)
    {
        return Script({
                          opcode::push_size_0,
                          address.getHashData()
                      });
    }
    
    Script Script::p2wshScript(const Address& address)
    {
        return Script({
                          opcode::push_size_0,
                          address.getHashData()
                      });
    }
    
    Script Script::p2shScript(const Script& script)
    {
        return Script({
                          opcode::hash160,
                          BYTE_ARRAY_TO_VECTOR(script.getHash160()),
                          opcode::equal
                      });
    }
    
    Script Script::p2wshScript(const Script& script)
    {
        return Script({
                          opcode::push_size_0,
                          BYTE_ARRAY_TO_VECTOR(script.getHash256())
                      });
    }
    
    Script::Script()
        : script()
    {
    }
    
    Script::Script(const Script& other)
        : script(other.script)
    {
    }
    
    Script::Script(const Script&& other)
        : script(std::move(other.script))
    {
    }
    
    Script::Script(const std::vector<BYTE>& data)
        : script(data, false)
    {
    }
    
    Script::Script(const std::vector<machine::operation>& operations)
        : script(chain::script(operations))
    {
    }
    
    Script::Script(const std::vector<std::string>& opVector)
        : script(chain::script(stringVectorToMachineOpVector(opVector)))
    {
    }
    
    Script::Script(const std::string& scriptStr)
        : Script(scriptStrToScriptStrVector(scriptStr))
    {
    }
    
    Script::Script(const char* scriptCStr)
        : Script(std::string(scriptCStr))
    {
    }
    
    Script::Script(const pt::ptree& tree)
        : Script(arrayToVectorOfPrimitives<std::string>(tree))
    {
    }
    
    const std::string Script::toHexString() const
    {
        return byteVectorToHexString(script.to_data(false));
    }
    
    const Hash160 Script::getHash160() const
    {
        return bitcoin_short_hash(script.to_data(false));
    }
    
    const Hash256 Script::getHash256() const
    {
        return static_cast<Hash256>(bitcoin_hash(script.to_data(false))).reversed();
    }
    
    const Hash256 Script::getSingleSHA256() const
    {
        return sha256_hash(script.to_data(false));
    }
    
    const std::vector<BYTE> Script::serialize() const
    {
        return script.to_data(true);
    }
    
    bool Script::operator==(const Script& other) const
    {
        return (script == other.script);
    }
    
    bool Script::operator!=(const Script& other) const
    {
        return (script != other.script);
    }
    
    Script::operator std::vector<BYTE>() const
    {
        return script.to_data(false);
    }
    
    Script::operator std::vector<std::string>() const
    {
        return machineOpVectorToStringVector(script.operations());
    }
    
    Script::operator std::string() const
    {
        return script.to_string(0);
    }
    
    Script::operator pt::ptree() const
    {
        return vectorOfPrimitivesToArray<std::string>(static_cast<std::vector<std::string>>(*this));
    }
    
    std::ostream& operator<<(std::ostream& os, const Script& script)
    {
        os << static_cast<std::string>(script);
        return os;
    }
};
