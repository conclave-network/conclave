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

#define BOOST_TEST_MODULE Sources_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../../../src/rpc/methods/make_entry_tx/structs/sources.h"
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
                std::vector<Outpoint> OUTPOINTS_1{
                    Outpoint("d3607b8606ab4d42feb4ea957f47ed3709e664203f821453ce35f08bd3e93ea7", 2),
                    Outpoint("2f767a1715bb8b200421a7ea4f0dccfac85622ccb08a2c0708a81d546748425c", 0),
                    Outpoint("e5b9e4c5fc3f22f0da9cee31648d6c7d26f0d596f45c1117b89ec262fabb037e", 1)
                };
                std::vector<Outpoint> OUTPOINTS_2{
                    Outpoint("d3607b8606ab4d42feb4ea957f47ed3709e664203f821453ce35f08bd3e93ea6", 2),
                    Outpoint("2f767a1715bb8b200421a7ea4f0dccfac85622ccb08a2c0708a81d546748425c", 1),
                    Outpoint("f5b9e4c5fc3f22f0da9cee31648d6c7d26f0d596f45c1117b89ec262fabb037e", 4)
                };
                std::string SOURCES_STR =
                    "{\n"
                    "    \"outpoints\": [\n"
                    "        {\n"
                    "            \"txId\": \"d3607b8606ab4d42feb4ea957f47ed3709e664203f821453ce35f08bd3e93ea7\",\n"
                    "            \"index\": \"2\"\n"
                    "        },\n"
                    "        {\n"
                    "            \"txId\": \"2f767a1715bb8b200421a7ea4f0dccfac85622ccb08a2c0708a81d546748425c\",\n"
                    "            \"index\": \"0\"\n"
                    "        },\n"
                    "        {\n"
                    "            \"txId\": \"e5b9e4c5fc3f22f0da9cee31648d6c7d26f0d596f45c1117b89ec262fabb037e\",\n"
                    "            \"index\": \"1\"\n"
                    "        }\n"
                    "    ]\n"
                    "}\n";
                
                pt::ptree makeSourcesTree()
                {
                    std::stringstream ss(SOURCES_STR);
                    pt::ptree tree;
                    pt::read_json(ss, tree);
                    return tree;
                }
                
                BOOST_AUTO_TEST_CASE(SourcesConstructorsTest)
                {
                    Sources sourcesFromProps(OUTPOINTS_1);
                    Sources sourcesFromPtree(makeSourcesTree());
                    BOOST_TEST(sourcesFromProps.outpoints == OUTPOINTS_1);
                    BOOST_TEST(sourcesFromPtree.outpoints == OUTPOINTS_1);
                }
                
                BOOST_AUTO_TEST_CASE(SourcesCastToPtreeTest)
                {
                    BOOST_TEST(makeSourcesTree() == (pt::ptree) Sources(OUTPOINTS_1));
                }
                
                BOOST_AUTO_TEST_CASE(SourcesCastToStringTest)
                {
                    BOOST_TEST(SOURCES_STR == (std::string) Sources(OUTPOINTS_1));
                }
                
                BOOST_AUTO_TEST_CASE(SourcesOperatorsTest)
                {
                    Sources sources1(OUTPOINTS_1);
                    Sources sources2(OUTPOINTS_2);
                    Sources sources3(OUTPOINTS_1);
                    Sources sources4(OUTPOINTS_2);
                    BOOST_TEST((sources1 == sources3));
                    BOOST_TEST((sources1 != sources4));
                    BOOST_TEST((sources2 != sources3));
                    BOOST_TEST((sources2 == sources4));
                }
            };
        };
    };
};
