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


#define BOOST_TEST_MODULE Destination_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/destination.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace pt = boost::property_tree;
namespace conclave
{
    Address ADDRESS_1("5XwNrVLahqDguAwPWUsgDBgrpYkYDwJE");
    Address ADDRESS_2("bc1qqr32wt85gv8gvyruyxre7sq5n4r5nj63ghemtmfh909qkymwhs8sjlls5z");
    uint64_t VALUE_1 = 0xfffffffffffffffe;
    uint64_t VALUE_2 = 0x0000000000000001;
    std::string DESTINATION_STR =
        "{\n"
        "    \"address\": \"5XwNrVLahqDguAwPWUsgDBgrpYkYDwJE\",\n"
        "    \"value\": \"18446744073709551614\"\n"
        "}\n";
    
    pt::ptree makeDestinationTree()
    {
        std::stringstream ss(DESTINATION_STR);
        pt::ptree tree;
        pt::read_json(ss, tree);
        return tree;
    }
    
    BOOST_AUTO_TEST_CASE(DestinationConstructorsTest)
    {
        Destination destinationFromProps(ADDRESS_1, VALUE_1);
        Destination destinationFromPtree(makeDestinationTree());
        BOOST_TEST(destinationFromProps.address == ADDRESS_1);
        BOOST_TEST(destinationFromProps.value == VALUE_1);
        BOOST_TEST(destinationFromPtree.address == ADDRESS_1);
        BOOST_TEST(destinationFromPtree.value == VALUE_1);
    }
    
    BOOST_AUTO_TEST_CASE(DestinationCastToPtreeTest)
    {
        BOOST_TEST(makeDestinationTree() == (pt::ptree) Destination(ADDRESS_1, VALUE_1));
    }
    
    BOOST_AUTO_TEST_CASE(DestinationCastToStringTest)
    {
        BOOST_TEST(DESTINATION_STR == (std::string) Destination(ADDRESS_1, VALUE_1));
    }
    
    BOOST_AUTO_TEST_CASE(DestinationOperatorsTest)
    {
        Destination destination1(ADDRESS_1, VALUE_1);
        Destination destination2(ADDRESS_1, VALUE_2);
        Destination destination3(ADDRESS_2, VALUE_1);
        Destination destination4(ADDRESS_2, VALUE_2);
        Destination destination5(ADDRESS_1, VALUE_1);
        Destination destination6(ADDRESS_1, VALUE_2);
        Destination destination7(ADDRESS_2, VALUE_1);
        Destination destination8(ADDRESS_2, VALUE_2);
        BOOST_TEST((destination1 == destination5));
        BOOST_TEST((destination1 != destination6));
        BOOST_TEST((destination1 != destination7));
        BOOST_TEST((destination1 != destination8));
        BOOST_TEST((destination2 != destination5));
        BOOST_TEST((destination2 == destination6));
        BOOST_TEST((destination2 != destination7));
        BOOST_TEST((destination2 != destination8));
        BOOST_TEST((destination3 != destination5));
        BOOST_TEST((destination3 != destination6));
        BOOST_TEST((destination3 == destination7));
        BOOST_TEST((destination3 != destination8));
        BOOST_TEST((destination4 != destination5));
        BOOST_TEST((destination4 != destination6));
        BOOST_TEST((destination4 != destination7));
        BOOST_TEST((destination4 == destination8));
    }
};
