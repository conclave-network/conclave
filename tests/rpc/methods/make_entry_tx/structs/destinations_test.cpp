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

#define BOOST_TEST_MODULE Destinations_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../../../src/rpc/methods/make_entry_tx/structs/destinations.h"
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace rpc
    {
        namespace methods
        {
            namespace make_entry_tx
            {
                std::vector<Destination> BITCOIN_DESTINATIONS_1{
                    Destination("1gdiuyyUEF8ttC4JNxWJ2UV3hQYfUNE1q", 4000)
                };
                std::vector<Destination> BITCOIN_DESTINATIONS_2{
                    Destination("bc1qqr32wt85gv8gvyruyxre7sq5n4r5nj63ghemtmfh909qkymwhs8sjlls5z", 3000)
                };
                std::vector<Destination> CONCLAVE_DESTINATIONS_1{
                    Destination("5XwNrVLahqDguAwPWUsgDBgrpYkYDwJE", 6000)
                };
                std::vector<Destination> CONCLAVE_DESTINATIONS_2{
                    Destination("4YSGxYSKvbPpD8qACGSSD4umxLUw4wDg", 7000)
                };
                std::string DESTINATIONS_STR =
                    "{\n"
                    "    \"bitcoin\": [\n"
                    "        {\n"
                    "            \"address\": \"1gdiuyyUEF8ttC4JNxWJ2UV3hQYfUNE1q\",\n"
                    "            \"value\": \"4000\"\n"
                    "        }\n"
                    "    ],\n"
                    "    \"conclave\": [\n"
                    "        {\n"
                    "            \"address\": \"5XwNrVLahqDguAwPWUsgDBgrpYkYDwJE\",\n"
                    "            \"value\": \"6000\"\n"
                    "        }\n"
                    "    ]\n"
                    "}\n";
                
                pt::ptree makeDestinationsTree()
                {
                    std::stringstream ss(DESTINATIONS_STR);
                    pt::ptree tree;
                    pt::read_json(ss, tree);
                    return tree;
                }
                
                BOOST_AUTO_TEST_CASE(DestinationsConstructorsTest)
                {
                    Destinations destinationsFromProps(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_1);
                    Destinations destinationsFromPtree(makeDestinationsTree());
                    BOOST_TEST(destinationsFromProps.bitcoinDestinations == BITCOIN_DESTINATIONS_1);
                    BOOST_TEST(destinationsFromProps.conclaveDestinations == CONCLAVE_DESTINATIONS_1);
                    BOOST_TEST(destinationsFromPtree.bitcoinDestinations == BITCOIN_DESTINATIONS_1);
                    BOOST_TEST(destinationsFromPtree.conclaveDestinations == CONCLAVE_DESTINATIONS_1);
                }
                
                BOOST_AUTO_TEST_CASE(DestinationsCastToPtreeTest)
                {
                    BOOST_TEST(makeDestinationsTree() ==
                               (pt::ptree) Destinations(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_1));
                }
                
                BOOST_AUTO_TEST_CASE(DestinationsCastToStringTest)
                {
                    BOOST_TEST(DESTINATIONS_STR ==
                               (std::string) Destinations(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_1));
                }
                
                BOOST_AUTO_TEST_CASE(DestinationsOperatorsTest)
                {
                    Destinations destinations1(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_1);
                    Destinations destinations2(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_2);
                    Destinations destinations3(BITCOIN_DESTINATIONS_2, CONCLAVE_DESTINATIONS_1);
                    Destinations destinations4(BITCOIN_DESTINATIONS_2, CONCLAVE_DESTINATIONS_2);
                    Destinations destinations5(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_1);
                    Destinations destinations6(BITCOIN_DESTINATIONS_1, CONCLAVE_DESTINATIONS_2);
                    Destinations destinations7(BITCOIN_DESTINATIONS_2, CONCLAVE_DESTINATIONS_1);
                    Destinations destinations8(BITCOIN_DESTINATIONS_2, CONCLAVE_DESTINATIONS_2);
                    BOOST_TEST((destinations1 == destinations5));
                    BOOST_TEST((destinations1 != destinations6));
                    BOOST_TEST((destinations1 != destinations7));
                    BOOST_TEST((destinations1 != destinations8));
                    BOOST_TEST((destinations2 != destinations5));
                    BOOST_TEST((destinations2 == destinations6));
                    BOOST_TEST((destinations2 != destinations7));
                    BOOST_TEST((destinations2 != destinations8));
                    BOOST_TEST((destinations3 != destinations5));
                    BOOST_TEST((destinations3 != destinations6));
                    BOOST_TEST((destinations3 == destinations7));
                    BOOST_TEST((destinations3 != destinations8));
                    BOOST_TEST((destinations4 != destinations5));
                    BOOST_TEST((destinations4 != destinations6));
                    BOOST_TEST((destinations4 != destinations7));
                    BOOST_TEST((destinations4 == destinations8));
                }
            }
        }
    }
}
