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

#include "sources.h"

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace make_entry_tx
            {
                const std::string Sources::JSONKEY_OUTPOINTS = "outpoints";
                
                Sources::Sources(const std::vector<Outpoint>& outpoints)
                    : outpoints(outpoints)
                {
                }
                
                Sources::Sources(const pt::ptree& tree)
                    : Sources(getVectorOfObjectsFromJson<Outpoint>(tree, JSONKEY_OUTPOINTS))
                {
                }
                
                Sources::operator pt::ptree() const
                {
                    pt::ptree tree;
                    tree.put_child(JSONKEY_OUTPOINTS, vectorOfObjectsToArray(outpoints));
                    return tree;
                }
                
                Sources::operator std::string() const
                {
                    return ptreeToString(static_cast<pt::ptree>(*this));
                }
                
                bool Sources::operator==(const Sources& other) const
                {
                    return outpoints == other.outpoints;
                }
                
                bool Sources::operator!=(const Sources& other) const
                {
                    return outpoints != other.outpoints;
                }
                
                std::ostream& operator<<(std::ostream& os, const Sources& sources)
                {
                    os << static_cast<std::string>(sources);
                    return os;
                }
            }
        }
    }
}
