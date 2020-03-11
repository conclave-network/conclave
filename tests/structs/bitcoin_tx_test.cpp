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

#define BOOST_TEST_MODULE Bitcoin_Tx_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/bitcoin_tx.h"
#include <cstdint>

namespace conclave
{
    const static uint32_t VERSION_1 = 1;
    const static std::vector<BitcoinInput> INPUTS_1 = {
        BitcoinInput(
            Outpoint("62a35a1513f7bc227e684d6511b88c55e1e08745f434d5a3d47b024cb3c089ec", 1),
            Script(std::vector<ScriptElement>{
                HEX_TO_BYTE_VECTOR("3045022100cea337474273c5167eec87a35c753fb85ef47015d4160cd017176e5b468015d"
                                   "3022046d290407a33adacb79a08b7bdf94b616d0c33c962c8481c207686192ae2365c01"),
                PublicKey("024f9b181887126fe48ecc85511fdbe50115ea9b5998f6267dc8f10813b944d259")
            }),
            0xffffffff
        )
    };
    const static std::vector<BitcoinOutput> OUTPUTS_1 = {
        BitcoinOutput(
            4600000,
            Script(std::vector<ScriptElement>{
                ScriptOp::dup,
                ScriptOp::hash160,
                Hash160("866ccfdfa642d162d61159967a5f932471cc5380"),
                ScriptOp::equalverify,
                ScriptOp::checksig
            })
        ),
        BitcoinOutput(
            40328493,
            Script(std::vector<ScriptElement>{
                ScriptOp::dup,
                ScriptOp::hash160,
                Hash160("0b9ef14978f06acf9ec2e74d380fa0bc122a8f36"),
                ScriptOp::equalverify,
                ScriptOp::checksig
            })
        ),
    };
    const static uint32_t LOCKTIME_1 = 0;
    
    BOOST_AUTO_TEST_CASE(BitcoinTxConstructorsTest)
    {
        BitcoinTx bitcoinTx1(VERSION_1, INPUTS_1, OUTPUTS_1, LOCKTIME_1);
        BOOST_TEST(true); // Dummy test because no tests will fail
    }
}
