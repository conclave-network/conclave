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

#define BOOST_TEST_MODULE Electrumx_Client_Test

#include <boost/test/included/unit_test.hpp>
#include "../../../src/bitcoin_chain/electrumx/electrumx_client.h"
#include "../../../src/address.h"
#include "../../../src/script.h"
#include "../../../src/util/json.h"
#include "../../../src/util/hex.h"
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;
namespace conclave
{
    namespace chain
    {
        namespace bitcoin
        {
            namespace electrumx
            {
                BOOST_AUTO_TEST_CASE(ElectrumxClientTest)
                {
                    ElectrumxClient electrumxClient("shogoth.no-ip.info", 50001);
                    pt::ptree tree = electrumxClient.blockchainScripthashGetBalance
                                                        ("8b01df4e368ea28f8dc0423bcf7a4923e3a12d307c875e47a0cfbf90b5c39161");
                    uint64_t balance = getPrimitiveFromJson<uint64_t>(tree, "confirmed");
                    std::cout << balance << std::endl;
                    //
                    std::string tx =
                        "02000000013dbfb73f7d31dd2e23b4c16b6132236ab8e49caf0d27141e367a0197866b8935000000006a4730440"
                        "2204e86f8cf1c2fb9f3d44053a523ae521b009825753237f5739b68880e42f9d59802201db91f005d3722b95c66"
                        "33c15cad6041feaa7ed86e91b11c7e41da8182dc3fd80121032f054dea6f57d98d4dc9acd15423dff9af4566696"
                        "1e84a4c8eed5eaa4f9c371afdffffff01f0550000000000001976a9147fb27545e1680662b355054b44eb27ab2a"
                        "96cb0f88ac497a0900";
                    tree = electrumxClient.blockchainTransactionBroadcast(tx);
                    std::string txid = tree.data();
                    std::cout << txid << std::endl;
                }
            };
        };
    };
};
