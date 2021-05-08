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

#pragma once

#include "chainwatch/chainwatch_manager.h"
#include "p2p/p2p_manager.h"
#include "rpc/rpc_manager.h"
#include "config/config.h"
#include "private_key.h"
#include "public_key.h"
#include "worker.h"
#include "chain/bitcoin_chain.h"
#include "chain/conclave_chain.h"

using namespace conclave::chainwatch;
using namespace conclave::p2p;
using namespace conclave::rpc;
using namespace conclave::chain;
namespace conclave
{
    class ConclaveNode final : public Worker
    {
        public:
        explicit ConclaveNode(const Config&);
        [[nodiscard]] const std::string getDisplayName() const;
        [[nodiscard]] const PublicKey getPublicKey() const;
        BitcoinChain& getBitcoinChain();
        ConclaveChain& getConclaveChain();
        private:
        void prepare() override;
        void cleanup() override;
        std::string displayName;
        PrivateKey privateKey;
        BitcoinChain bitcoinChain;
        ConclaveChain conclaveChain;
        ChainwatchManager chainwatchManager;
        P2pManager p2pManager;
        RpcManager rpcManager;
    };
}
