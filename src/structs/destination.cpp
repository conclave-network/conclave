/*
 * CONCLAVE - Scaling Bitcoin Simply.
 * Copyright (C) 2019-2021 Conclave development team
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

#include "destination.h"

namespace pt = boost::property_tree;
namespace conclave
{
    const std::string Destination::JSONKEY_ADDRESS = "address";
    const std::string Destination::JSONKEY_VALUE = "value";
    
    Destination::Destination(const Address& address, const uint64_t value)
        : address(address), value(value)
    {
    }
    
    Destination::Destination(const pt::ptree& tree)
        : Destination(getPrimitiveFromJson<std::string>(tree, JSONKEY_ADDRESS),
                      getPrimitiveFromJson<uint64_t>(tree, JSONKEY_VALUE))
    {
    }
    
    Destination::operator pt::ptree() const
    {
        pt::ptree tree;
        tree.add<std::string>(JSONKEY_ADDRESS, static_cast<std::string>(address));
        tree.add<uint64_t>(JSONKEY_VALUE, value);
        return tree;
    }
    
    Destination::operator std::string() const
    {
        return ptreeToString(static_cast<pt::ptree>(*this));
    }
    
    bool Destination::operator==(const Destination& other) const
    {
        return (address == other.address) && (value == other.value);
    }
    
    bool Destination::operator!=(const Destination& other) const
    {
        return (address != other.address) || (value != other.value);
    }
    
    std::ostream& operator<<(std::ostream& os, const Destination& destination)
    {
        os << static_cast<std::string>(destination);
        return os;
    }
}
