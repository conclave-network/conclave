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

#include "script_element.h"
#include "conclave.h"
#include "address.h"
#include "hash160.h"
#include "hash256.h"
#include <boost/property_tree/ptree.hpp>
#include <bitcoin/system.hpp>
#include <vector>
#include <string>

namespace bc_chain = bc::system::chain;
namespace pt = boost::property_tree;
namespace conclave
{
    /***
     * Wrapper around libbitcoin's chain::script class
     */
    class Script final
    {
        public:
        // Factories
        static Script p2hScript(const Address&);
        static Script p2pkhScript(const Address&);
        static Script p2shScript(const Address&);
        static Script p2wpkhScript(const Address&);
        static Script p2wshScript(const Address&);
        static Script p2shScript(const Script&);
        static Script p2wshScript(const Script&);
        // Constructors
        Script();
        Script(const std::vector<BYTE>&);
        Script(const std::vector<ScriptElement>&);
        Script(const std::vector<std::string>&);
        Script(const std::string&);
        Script(const char*);
        Script(const pt::ptree&);
        Script(const Script&);
        Script(Script&&);
        // Public Functions
        const std::string toHexString() const;
        const Hash160 getHash160() const;
        const Hash256 getHash256() const;
        const Hash256 getSingleSHA256() const;
        const std::vector<BYTE> serialize() const;
        // Conversions
        explicit operator std::vector<std::string>() const;
        explicit operator std::vector<BYTE>() const;
        explicit operator std::string() const;
        explicit operator pt::ptree() const;
        // Operator Overloads
        Script& operator=(const Script&);
        Script& operator=(Script&&);
        bool operator==(const Script& other) const;
        bool operator!=(const Script& other) const;
        friend std::ostream& operator<<(std::ostream&, const Script&);
        private:
        // Private Constructors
        Script(const std::vector<machine::operation>&); // Only used by factories
        // Properties
        bc_chain::script script;
    };
};
