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

set(folder "tests")
set(CMAKE_CXX_FLAGS "-pthread -Wall -Wno-register -Wno-format-truncation")
set(Boost_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS ON)

# This is needed so CMake compiles .c files with c++ compiler
set_source_files_properties("../src/mongoose/mongoose.c" PROPERTIES LANGUAGE CXX)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions("-x c++")
endif ()

find_package(
        Boost
        1.72.0
        COMPONENTS REQUIRED
        unit_test_framework
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
        hex_test
        util/hex_test.cpp
)

add_executable(
        json_test
        util/json_test.cpp
)

add_executable(
        random_test
        util/random_test.cpp
)

add_executable(
        serialization_test
        util/serialization_test.cpp
)

add_executable(
        hash160_test
        ../src/hash160.cpp
        hash160_test.cpp
)

add_executable(
        hash256_test
        ../src/hash256.cpp
        hash256_test.cpp
)

add_executable(
        private_key_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/public_key.cpp
        ../src/private_key.cpp
        private_key_test.cpp
)

add_executable(
        public_key_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/public_key.cpp
        public_key_test.cpp
)

add_executable(
        address_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        address_test.cpp
)

add_executable(
        script_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/script.cpp
        script_test.cpp
)

add_executable(
        outpoint_test
        ../src/hash256.cpp
        ../src/structs/outpoint.cpp
        structs/outpoint_test.cpp
)

add_executable(
        inpoint_test
        ../src/hash256.cpp
        ../src/structs/outpoint.cpp
        structs/outpoint_test.cpp
)

add_executable(
        destination_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/structs/destination.cpp
        structs/destination_test.cpp
)

add_executable(
        bitcoin_input_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/bitcoin_input.cpp
        structs/bitcoin_input_test.cpp
)

add_executable(
        bitcoin_output_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/bitcoin_output.cpp
        structs/bitcoin_output_test.cpp
)

add_executable(
        conclave_input_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/inpoint.cpp
        ../src/structs/conclave_input.cpp
        structs/conclave_input_test.cpp
)

add_executable(
        conclave_output_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/conclave_output.cpp
        structs/conclave_output_test.cpp
)

add_executable(
        bitcoin_tx_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/public_key.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/bitcoin_input.cpp
        ../src/structs/bitcoin_output.cpp
        ../src/structs/bitcoin_tx.cpp
        structs/bitcoin_tx_test.cpp
)

add_executable(
        claim_tx_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/public_key.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/conclave_output.cpp
        ../src/structs/conclave_claim_tx.cpp
        structs/conclave_claim_tx_test.cpp
)

add_executable(
        entry_tx_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/public_key.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/bitcoin_input.cpp
        ../src/structs/bitcoin_output.cpp
        ../src/structs/conclave_output.cpp
        ../src/structs/conclave_claim_tx.cpp
        ../src/structs/bitcoin_tx.cpp
        structs/conclave_claim_tx_test.cpp
)

add_executable(
        conclave_standard_tx_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/public_key.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/inpoint.cpp
        ../src/structs/conclave_input.cpp
        ../src/structs/conclave_output.cpp
        ../src/structs/bitcoin_output.cpp
        ../src/structs/conclave_standard_tx.cpp
        structs/conclave_standard_tx_test.cpp
)

add_executable(
        bitcoin_chain_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/public_key.cpp
        ../src/script.cpp
        ../src/structs/outpoint.cpp
        ../src/structs/bitcoin_input.cpp
        ../src/structs/bitcoin_output.cpp
        ../src/structs/bitcoin_tx.cpp
        ../src/chain/bitcoin_chain.cpp
        ../src/chain/electrumx/electrumx_client.cpp
        ../src/config/bitcoin_chain_config.cpp
        ../src/config/electrumx_client_config.cpp
        chain/bitcoin_chain_test.cpp
)

add_executable(
        electrumx_client_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/script.cpp
        ../src/chain/electrumx/electrumx_client.cpp
        ../src/config/electrumx_client_config.cpp
        chain/electrumx/electrumx_client_test.cpp
)

add_executable(
        database_client_test
        ../src/hash256.cpp
        ../src/config/database_client_config.cpp
        ../src/chain/database/database_client.cpp
        chain/database/database_client_test.cpp
)

add_executable(
        sources_test
        ../src/hash256.cpp
        ../src/structs/outpoint.cpp
        ../src/rpc/methods/make_entry_tx/structs/sources.cpp
        rpc/methods/make_entry_tx/structs/sources_test.cpp
)

add_executable(
        destinations_test
        ../src/hash160.cpp
        ../src/hash256.cpp
        ../src/address.cpp
        ../src/structs/destination.cpp
        ../src/rpc/methods/make_entry_tx/structs/destinations.cpp
        rpc/methods/make_entry_tx/structs/destinations_test.cpp
)

target_link_libraries(
        hex_test
)

target_link_libraries(
        json_test
        LINK_PUBLIC ${Boost_LIBRARIES}
)

target_link_libraries(
        random_test
        LINK_PUBLIC ${Boost_LIBRARIES}
)

target_link_libraries(
        serialization_test
)

target_link_libraries(
        hash160_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        hash256_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        private_key_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        public_key_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        address_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        script_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        outpoint_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        inpoint_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        destination_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        bitcoin_input_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        conclave_input_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        bitcoin_output_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        conclave_output_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        bitcoin_tx_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        claim_tx_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)


target_link_libraries(
        entry_tx_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        conclave_standard_tx_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        bitcoin_chain_test
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
)

target_link_libraries(
        electrumx_client_test
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
)

target_link_libraries(
        database_client_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        PkgConfig::LIBBITCOIN_SYSTEM
        lmdb
        stdc++fs # Remove after GCC9
)

target_link_libraries(
        sources_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        PkgConfig::LIBBITCOIN_SYSTEM
)

target_link_libraries(
        destinations_test
        LINK_PUBLIC ${Boost_LIBRARIES}
        OpenSSL::Crypto
        OpenSSL::SSL
        ${CMAKE_DL_LIBS}
        PkgConfig::LIBBITCOIN_SYSTEM
)

add_test(
        NAME hex_test
        COMMAND $<TARGET_FILE:hex_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME json_test
        COMMAND $<TARGET_FILE:json_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME random_test
        COMMAND $<TARGET_FILE:random_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME serialization_test
        COMMAND $<TARGET_FILE:serialization_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME hash160_test
        COMMAND $<TARGET_FILE:hash160_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME hash256_test
        COMMAND $<TARGET_FILE:hash256_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME private_key_test
        COMMAND $<TARGET_FILE:private_key_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME public_key_test
        COMMAND $<TARGET_FILE:public_key_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME address_test
        COMMAND $<TARGET_FILE:address_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME script_test
        COMMAND $<TARGET_FILE:script_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME outpoint_test
        COMMAND $<TARGET_FILE:outpoint_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME inpoint_test
        COMMAND $<TARGET_FILE:inpoint_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME destination_test
        COMMAND $<TARGET_FILE:destination_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME bitcoin_input_test
        COMMAND $<TARGET_FILE:bitcoin_input_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME conclave_input_test
        COMMAND $<TARGET_FILE:bitcoin_input_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME bitcoin_output_test
        COMMAND $<TARGET_FILE:bitcoin_output_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME conclave_output_test
        COMMAND $<TARGET_FILE:conclave_output_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME bitcoin_tx_test
        COMMAND $<TARGET_FILE:bitcoin_tx_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME claim_tx_test
        COMMAND $<TARGET_FILE:claim_tx_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME entry_tx_test
        COMMAND $<TARGET_FILE:entry_tx_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME conclave_standard_tx_test
        COMMAND $<TARGET_FILE:conclave_standard_tx_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME bitcoin_chain_test
        COMMAND $<TARGET_FILE:bitcoin_chain_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME electrumx_client_test
        COMMAND $<TARGET_FILE:electrumx_client_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME database_client_test
        COMMAND $<TARGET_FILE:database_client_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME sources_test
        COMMAND $<TARGET_FILE:sources_test> --report_format=HRF --logger=HRF,all
)

add_test(
        NAME destinations_test
        COMMAND $<TARGET_FILE:destinations_test> --report_format=HRF --logger=HRF,all
)

enable_testing()
