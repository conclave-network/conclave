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

#define BOOST_TEST_MODULE Conclave_Claim_Tx_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/conclave_claim_tx.h"

namespace conclave
{
    const static std::vector<ConclaveOutput> OUTPUTS_1{
        ConclaveOutput(Script(), 42)
    };
    const static std::vector<ConclaveOutput> OUTPUTS_2{
        ConclaveOutput(std::string("DROP"), 43)
    };
    const static std::vector<PublicKey> TRUSTEES_1{
        PublicKey("03519e185b4c0de842cb5cf9e49cd6df00569c4f34bf87ed295b65f9473d508e9d"),
        PublicKey("02a013b162136d674a37b95ce50d0ae02c5f3d3ab74e52d363e809cde86e92cbbb")
    };
    const static std::vector<PublicKey> TRUSTEES_2{
        PublicKey("02a013b162136d674a37b95ce50d0ae02c5f3d3ab74e52d363e809cde86e92cbbb"),
        PublicKey("031f74bdb5873d9ae3d46aa3473718eac8fe19d4b9d62d9421cf6b9b6beab1e6c4")
    };
    const static uint32_t MIN_SIGS_1 = 1;
    const static uint32_t MIN_SIGS_2 = 2;
    const static Outpoint FUNDING_OUTPOINT("1f4bb68db33c8a53996a4c6452892beb1638d799592889e3bc52b7a2cfe52cd4", 21);
    
    BOOST_AUTO_TEST_CASE(ConclaveClaimTxConstructorsTest)
    {
        ConclaveClaimTx conclaveClaimTxFromOutputs1(OUTPUTS_1, TRUSTEES_1, MIN_SIGS_1);
        ConclaveClaimTx conclaveClaimTxFromOutputs2(OUTPUTS_2, TRUSTEES_2, MIN_SIGS_2);
        ConclaveClaimTx conclaveClaimTxFromOutputsWithBitcoinTxid1(OUTPUTS_1, TRUSTEES_1, MIN_SIGS_1, FUNDING_OUTPOINT);
        ConclaveClaimTx conclaveClaimTxFromOutputsWithBitcoinTxid2(OUTPUTS_2, TRUSTEES_2, MIN_SIGS_2, FUNDING_OUTPOINT);
    }
}
