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

#include "destinations.h"

namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace make_entry_tx
            {
                const std::string Destinations::JSONKEY_BITCOIN = "bitcoin";
                const std::string Destinations::JSONKEY_CONCLAVE = "conclave";
                
                Destinations::Destinations(const std::vector<Destination>& bitcoinDestinations,
                                           const std::vector<Destination>& conclaveDestinations)
                    : bitcoinDestinations(bitcoinDestinations),
                      conclaveDestinations(conclaveDestinations)
                {
                }
                
                Destinations::Destinations(const pt::ptree& tree)
                    : Destinations(tryGetVectorOfObjects<Destination>(tree, JSONKEY_BITCOIN),
                                   tryGetVectorOfObjects<Destination>(tree, JSONKEY_CONCLAVE))
                {
                }
                
                Destinations::operator pt::ptree() const
                {
                    pt::ptree tree;
                    tree.put_child(JSONKEY_BITCOIN, vectorOfObjectsToArray(bitcoinDestinations));
                    tree.put_child(JSONKEY_CONCLAVE, vectorOfObjectsToArray(conclaveDestinations));
                    return tree;
                }
                
                Destinations::operator std::string() const
                {
                    return jsonToString(static_cast<pt::ptree>(*this));
                }
                
                bool Destinations::operator==(const Destinations& other) const
                {
                    return (bitcoinDestinations == other.bitcoinDestinations) &&
                           (conclaveDestinations == other.conclaveDestinations);
                }
                
                bool Destinations::operator!=(const Destinations& other) const
                {
                    return (bitcoinDestinations != other.bitcoinDestinations) ||
                           (conclaveDestinations != other.conclaveDestinations);
                }
                
                std::ostream& operator<<(std::ostream& os, const Destinations& destinations)
                {
                    os << static_cast<std::string>(destinations);
                    return os;
                }
            }
        }
    }
}
