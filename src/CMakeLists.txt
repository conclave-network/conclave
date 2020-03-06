# CONCLAVE - Making Bitcoin Scale And Be Useful
# Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

set(folder "src")
set(CMAKE_CXX_FLAGS "-pthread -Wall -Wno-register -Wno-format-truncation")
set(Boost_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS ON)

# This is needed so CMake compiles .c files with c++ compiler
set_source_files_properties("mongoose/mongoose.c" PROPERTIES LANGUAGE CXX)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions("-x c++")
endif ()

find_package(
        Boost
        1.72.0
        COMPONENTS REQUIRED
        program_options
        random
)

find_package(
        OpenSSL
        REQUIRED
)

find_package(
        PkgConfig
        REQUIRED
)

pkg_check_modules(
        LIBBITCOIN_SYSTEM
        REQUIRED
        IMPORTED_TARGET
        libbitcoin-system
)

add_executable(
        conclaved
        conclaved.cpp
        conclave_node.cpp
        worker.cpp
        private_key.cpp
        public_key.cpp
        hash160.cpp
        hash256.cpp
        address.cpp
        script.cpp
        mongoose/mongoose.c
        structs/outpoint.cpp
        structs/destination.cpp
        structs/bitcoin_tx.cpp
        structs/bitcoin_input.cpp
        structs/bitcoin_output.cpp
        structs/conclave_output.cpp
        structs/conclave_entry_tx.cpp
        config/config.cpp
        config/rpc_config.cpp
        config/rpc_acceptor_config.cpp
        config/cloud_config.cpp
        config/chainwatch_config.cpp
        config/bitcoin_chain_config.cpp
        config/conclave_chain_config.cpp
        config/electrumx_client_config.cpp
        chainwatch/chainwatch_manager.cpp
        p2p/p2p_manager.cpp
        rpc/rpc_manager.cpp
        rpc/rpc_acceptor.cpp
        rpc/rpc_dispatcher.cpp
        rpc/rpc_processor.cpp
        rpc/methods/request.cpp
        rpc/methods/response.cpp
        rpc/methods/node_info/node_info_handler.cpp
        rpc/methods/send_tx/send_tx_handler.cpp
        rpc/methods/get_address_balance/get_address_balance_handler.cpp
        rpc/methods/make_entry_tx/make_entry_tx_handler.cpp
        rpc/methods/make_entry_tx/structs/sources.cpp
        rpc/methods/make_entry_tx/structs/destinations.cpp
        cloud/cloud.cpp
        conclave_chain/conclave_chain.cpp
        bitcoin_chain/bitcoin_chain.cpp
        bitcoin_chain/electrumx/electrumx_client.cpp
        rpc/methods/make_entry_tx/make_entry_tx_handler.cpp rpc/methods/submit_entry_tx/submit_entry_tx_handler.cpp rpc/methods/submit_entry_tx/submit_entry_tx_request.h rpc/methods/submit_entry_tx/submit_entry_tx_response.h)

target_link_libraries(
        conclaved
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
        PocoNet
        PocoUtil
        PocoJSON
        PocoXML
        PocoFoundation
        lmdb
        stdc++fs # Remove after GCC9
)

set_target_properties(
        conclaved
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
