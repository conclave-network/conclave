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

#define BOOST_TEST_MODULE Conclave_Entry_Tx_Test

#include <boost/test/included/unit_test.hpp>
#include "../../src/structs/conclave_entry_tx.h"

namespace conclave
{
    const static std::vector<ConclaveOutput> OUTPUTS_1{
        ConclaveOutput(Script(), 42)
    };
    const static std::vector<ConclaveOutput> OUTPUTS_2{
        ConclaveOutput(std::string("DROP"), 43)
    };
    const static Hash256 BITCOIN_TXID_1("1f4bb68db33c8a53996a4c6452892beb1638d799592889e3bc52b7a2cfe52cd4");
    const static Hash256 BITCOIN_TXID_2("92605ef4c724eba99523c0d73f53838ead11668c122037277c6704444e216a56");
    
    BOOST_AUTO_TEST_CASE(ConclaveEntryTxConstructorsTest)
    {
        ConclaveEntryTx conclaveEntryTxFromOutputs1(OUTPUTS_1);
        ConclaveEntryTx conclaveEntryTxFromOutputs2(OUTPUTS_2);
        ConclaveEntryTx conclaveEntryTxFromOutputs1AndTxId1(OUTPUTS_1, BITCOIN_TXID_1);
        ConclaveEntryTx conclaveEntryTxFromOutputs1AndTxId2(OUTPUTS_1, BITCOIN_TXID_2);
        ConclaveEntryTx conclaveEntryTxFromOutputs2AndTxId1(OUTPUTS_2, BITCOIN_TXID_1);
        ConclaveEntryTx conclaveEntryTxFromOutputs2AndTxId2(OUTPUTS_2, BITCOIN_TXID_2);
    }
}
