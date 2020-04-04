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
#include "util/serialization.h"
#include "script.h"

namespace conclave
{
    using namespace bc::system;
    using namespace bc::system::machine;
    
    //
    // Helpers For Constructors
    //
    
    inline const static
    std::vector<machine::operation> stringVectorToMachineOpVector(const std::vector<std::string>& in)
    {
        std::vector<machine::operation> ops(in.size());
        for (size_t i = 0; i < in.size(); i++) {
            ops[i].from_string(in[i]);
        }
        return ops;
    }
    
    inline const static
    std::vector<machine::operation> scriptElementVectorToMachineOpVector(const std::vector<ScriptElement>& in)
    {
        std::vector<machine::operation> ops(0);
        ops.reserve(in.size());
        for (const ScriptElement& scriptElement: in) {
            ops.emplace_back(scriptElement);
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
    // Helpers For Casts
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
    
    //
    // Factories
    //
    
    Script Script::deserialize(const std::vector<BYTE>& data, size_t& pos)
    {
        size_t len = deserializeVarInt(data, pos);
        Script ret = Script(std::vector<BYTE>(data.begin() + pos, data.begin() + pos + len));
        pos += len;
        return ret;
    }
    
    Script Script::deserialize(const std::vector<BYTE>& data)
    {
        size_t pos = 0;
        return deserialize(data, pos);
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
        if (address.isClassic() || address.isConclave()) {
            if (address.payeeIsPubKey()) {
                return p2pkhScript(address);
            } else if (address.payeeIsScript()) {
                return p2shScript(address);
            }
        } else if (address.isSegwit()) {
            if (address.payeeIsPubKey()) {
                return p2wpkhScript(address);
            } else if (address.payeeIsScript()) {
                return p2wshScript(address);
            }
        }
        throw std::logic_error("Could not create script which pays to address: " + (std::string) address);
    }
    
    Script Script::p2pkhScript(const Address& address)
    {
        return Script(std::vector<machine::operation>{
            opcode::dup,
            opcode::hash160,
            address.getHashData(),
            opcode::equalverify,
            opcode::checksig
        });
    }
    
    Script Script::p2shScript(const Address& address)
    {
        return Script(std::vector<machine::operation>{
            opcode::hash160,
            address.getHashData(),
            opcode::equal
        });
    }
    
    Script Script::p2wpkhScript(const Address& address)
    {
        return Script(std::vector<machine::operation>{
            opcode::push_size_0,
            address.getHashData()
        });
    }
    
    Script Script::p2wshScript(const Address& address)
    {
        return Script(std::vector<machine::operation>{
            opcode::push_size_0,
            address.getHashData()
        });
    }
    
    Script Script::p2shScript(const Script& script)
    {
        return Script(std::vector<machine::operation>{
            opcode::hash160,
            static_cast<std::vector<BYTE>>(script.getHash160()),
            opcode::equal
        });
    }
    
    Script Script::p2wshScript(const Script& script)
    {
        return Script(std::vector<machine::operation>{
            opcode::push_size_0,
            static_cast<std::vector<BYTE>>(script.getHash256())
        });
    }
    
    //
    // Constructors
    //
    
    Script::Script()
        : script()
    {
    }
    
    Script::Script(const std::vector<BYTE>& data)
        : script(data, false)
    {
    }
    
    Script::Script(const std::vector<ScriptElement>& seVec)
        : Script(scriptElementVectorToMachineOpVector(seVec))
    {
    }
    
    Script::Script(const std::vector<std::string>& strVec)
        : Script(stringVectorToMachineOpVector(strVec))
    {
    }
    
    Script::Script(const std::string& str)
        : Script(scriptStrToScriptStrVector(str))
    {
    }
    
    Script::Script(const char* cStr)
        : Script(std::string(cStr))
    {
    }
    
    Script::Script(const pt::ptree& tree)
        : Script(arrayToVectorOfPrimitives<std::string>(tree))
    {
    }
    
    Script::Script(const std::vector<machine::operation>& operations)
        : script(chain::script(operations))
    {
    }
    
    Script::Script(const Script& other)
        : script(other.script)
    {
    }
    
    Script::Script(Script&& other)
        : script(std::move(other.script))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash160 Script::getHash160() const
    {
        return Hash160::digest(script.to_data(false));
    }
    
    const Hash256 Script::getHash256() const
    {
        return Hash256::digest(script.to_data(false));
    }
    
    const Hash256 Script::getSingleSHA256() const
    {
        return sha256_hash(script.to_data(false));
    }
    
    const std::vector<BYTE> Script::serialize() const
    {
        return script.to_data(true);
    }
    
    const std::string Script::toHexString() const
    {
        return byteVectorToHexString(script.to_data(false));
    }
    
    //
    // Conversions
    //
    
    Script::operator pt::ptree() const
    {
        return vectorOfPrimitivesToArray<std::string>(static_cast<std::vector<std::string>>(*this));
    }
    
    Script::operator std::string() const
    {
        return script.to_string(0);
    }
    
    Script::operator std::vector<BYTE>() const
    {
        return script.to_data(false);
    }
    
    Script::operator std::vector<std::string>() const
    {
        return machineOpVectorToStringVector(script.operations());
    }
    
    //
    // Operator Overloads
    //
    
    Script& Script::operator=(const Script& other)
    {
        script = other.script;
        return *this;
    }
    
    Script& Script::operator=(Script&& other)
    {
        script = std::move(other.script);
        return *this;
    }
    
    bool Script::operator==(const Script& other) const
    {
        return (script == other.script);
    }
    
    bool Script::operator!=(const Script& other) const
    {
        return (script != other.script);
    }
    
    std::ostream& operator<<(std::ostream& os, const Script& script)
    {
        os << static_cast<std::string>(script);
        return os;
    }
};
