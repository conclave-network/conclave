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

#pragma once

#include "database/database_client.h"
#include "bitcoin_chain.h"
#include "../config/conclave_chain_config.h"
#include "../hash256.h"
#include "../address.h"
#include "../structs/conclave_standard_tx.h"
#include "../structs/entry_tx.h"

/***
 * Abstraction layer over the Conclave blockchain. All interaction with the Conclave chain
 * such as getting blocks, transactions, wallet balances, as well as submitting new transactions,
 * must be done through this class.
 */

namespace conclave
{
    namespace chain
    {
        using namespace database;
        
        class ConclaveChain
        {
            public:
            explicit ConclaveChain(const ConclaveChainConfig&, BitcoinChain& bitcoinChain);
            const uint64_t getAddressBalance(const Address&);
            void submitEntryTx(const EntryTx&);
            const Hash256 submitStandardTx(const ConclaveStandardTx&);
            private:
            BitcoinChain& bitcoinChain;
            DatabaseClient databaseClient;
        };
    }
}
