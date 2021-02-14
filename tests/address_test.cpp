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

#define BOOST_TEST_MODULE Address_Test

#include <boost/test/included/unit_test.hpp>
#include "../src/address.h"
#include "../src/conclave.h"

namespace conclave
{
    const static std::string MAINNET_CLASSIC_PUBKEY_ADDRESS = "1gdiuyyUEF8ttC4JNxWJ2UV3hQYfUNE1q";
    const static std::string MAINNET_CLASSIC_SCRIPT_ADDRESS = "32NeeTUR28ZWz3tVRUd6ieqRCDhG9XZo97";
    const static std::string MAINNET_SEGWIT_PUBKEY_ADDRESS = "bc1qqalwppgamc425u28xt4mlcxqqy65jzr0ae5lyn";
    const static std::string MAINNET_SEGWIT_SCRIPT_ADDRESS = "bc1qqr32wt85gv8gvyruyxre7sq5n4r5nj63ghemtmfh909qkymwhs8sjlls5z";
    const static std::string MAINNET_CONCLAVE_PUBKEY_ADDRESS = "5XwNrVLahqDguAwPWUsgDBgrpYkYDwJE";
    const static std::string MAINNET_CONCLAVE_SCRIPT_ADDRESS = "4YSGxYSKvbPpD8qACGSSD4umxLUw4wDg";
    const static std::string TESTNET_CLASSIC_PUBKEY_ADDRESS = "mgCb1y4xHFgPfzfg1wvt7wgouh1FZrDDSx";
    const static std::string TESTNET_CLASSIC_SCRIPT_ADDRESS = "2MsvriCQSdb4sBqX36cEyLbpgQZuS1h3TJ7";
    const static std::string TESTNET_SEGWIT_PUBKEY_ADDRESS = "tb1qqalwppgamc425u28xt4mlcxqqy65jzr0hl0vlq";
    const static std::string TESTNET_SEGWIT_SCRIPT_ADDRESS = "tb1qqr32wt85gv8gvyruyxre7sq5n4r5nj63ghemtmfh909qkymwhs8s9hflwd";
    const static std::string TESTNET_CONCLAVE_PUBKEY_ADDRESS = "2tbmoJGZxXgh4Qen1F3hYCwyBKNFow66";
    const static std::string TESTNET_CONCLAVE_SCRIPT_ADDRESS = "u6fuMNKBHrpNNYYh2cTY6AtK76eew1Y";
    const static std::array<BYTE, SMALL_HASH_SIZE_BYTES> SHORT_HASH_BA{
        0x07, 0x7e, 0xe0, 0x85, 0x1d, 0xde, 0x2a, 0xaa, 0x71, 0x47,
        0x32, 0xeb, 0xbf, 0xe0, 0xc0, 0x01, 0x35, 0x49, 0x08, 0x6f
    };
    const static std::array<BYTE, LARGE_HASH_SIZE_BYTES> LONG_HASH_BA{
        0x00, 0xe2, 0xa7, 0x2c, 0xf4, 0x43, 0x0e, 0x86,
        0x10, 0x7c, 0x21, 0x87, 0x9f, 0x40, 0x14, 0x9d,
        0x47, 0x49, 0xcb, 0x51, 0x45, 0xf3, 0xb5, 0xed,
        0x37, 0x2b, 0xca, 0x0b, 0x13, 0x6e, 0xbc, 0x0f
    };
    const static std::vector<BYTE> SHORT_HASH_BV(SHORT_HASH_BA.begin(), SHORT_HASH_BA.end());
    const static std::vector<BYTE> LONG_HASH_BV(LONG_HASH_BA.begin(), LONG_HASH_BA.end());
    const static PublicKey PUBLIC_KEY("026022eda26f4fbe5a33f533d6204731bb0e21a94191869395dc205b53e799e2d9");
    BOOST_AUTO_TEST_SUITE(AddressTestSuite)
        
        BOOST_AUTO_TEST_CASE(AddressByteArrayConstructorsTest)
        {
            // Test byte array constructors
            Address mainnetClassicPubkeyAddress(SHORT_HASH_BA, Address::AddressFormat::CLASSIC,
                                                Address::NetworkType::MAINNET, Address::PayeeType::PUBKEY);
            Address mainnetClassicScriptAddress(SHORT_HASH_BA, Address::AddressFormat::CLASSIC,
                                                Address::NetworkType::MAINNET, Address::PayeeType::SCRIPT);
            Address mainnetSegwitPubkeyAddress(SHORT_HASH_BA, Address::AddressFormat::SEGWIT,
                                               Address::NetworkType::MAINNET, Address::PayeeType::PUBKEY);
            Address mainnetSegwitScriptAddress(LONG_HASH_BA, Address::AddressFormat::SEGWIT,
                                               Address::NetworkType::MAINNET, Address::PayeeType::SCRIPT);
            Address mainnetConclavePubkeyAddress(SHORT_HASH_BA, Address::AddressFormat::CONCLAVE,
                                                 Address::NetworkType::MAINNET, Address::PayeeType::PUBKEY);
            Address mainnetConclaveScriptAddress(SHORT_HASH_BA, Address::AddressFormat::CONCLAVE,
                                                 Address::NetworkType::MAINNET, Address::PayeeType::SCRIPT);
            Address testnetClassicPubkeyAddress(SHORT_HASH_BA, Address::AddressFormat::CLASSIC,
                                                Address::NetworkType::TESTNET, Address::PayeeType::PUBKEY);
            Address testnetClassicScriptAddress(SHORT_HASH_BA, Address::AddressFormat::CLASSIC,
                                                Address::NetworkType::TESTNET, Address::PayeeType::SCRIPT);
            Address testnetSegwitPubkeyAddress(SHORT_HASH_BA, Address::AddressFormat::SEGWIT,
                                               Address::NetworkType::TESTNET, Address::PayeeType::PUBKEY);
            Address testnetSegwitScriptAddress(LONG_HASH_BA, Address::AddressFormat::SEGWIT,
                                               Address::NetworkType::TESTNET, Address::PayeeType::SCRIPT);
            Address testnetConclavePubkeyAddress(SHORT_HASH_BA, Address::AddressFormat::CONCLAVE,
                                                 Address::NetworkType::TESTNET, Address::PayeeType::PUBKEY);
            Address testnetConclaveScriptAddress(SHORT_HASH_BA, Address::AddressFormat::CONCLAVE,
                                                 Address::NetworkType::TESTNET, Address::PayeeType::SCRIPT);
            BOOST_TEST((mainnetClassicPubkeyAddress == MAINNET_CLASSIC_PUBKEY_ADDRESS));
            BOOST_TEST((mainnetClassicScriptAddress == MAINNET_CLASSIC_SCRIPT_ADDRESS));
            BOOST_TEST((mainnetSegwitPubkeyAddress == MAINNET_SEGWIT_PUBKEY_ADDRESS));
            BOOST_TEST((mainnetSegwitScriptAddress == MAINNET_SEGWIT_SCRIPT_ADDRESS));
            BOOST_TEST((mainnetConclavePubkeyAddress == MAINNET_CONCLAVE_PUBKEY_ADDRESS));
            BOOST_TEST((mainnetConclaveScriptAddress == MAINNET_CONCLAVE_SCRIPT_ADDRESS));
            BOOST_TEST((testnetClassicPubkeyAddress == TESTNET_CLASSIC_PUBKEY_ADDRESS));
            BOOST_TEST((testnetClassicScriptAddress == TESTNET_CLASSIC_SCRIPT_ADDRESS));
            BOOST_TEST((testnetSegwitPubkeyAddress == TESTNET_SEGWIT_PUBKEY_ADDRESS));
            BOOST_TEST((testnetSegwitScriptAddress == TESTNET_SEGWIT_SCRIPT_ADDRESS));
            BOOST_TEST((testnetConclavePubkeyAddress == TESTNET_CONCLAVE_PUBKEY_ADDRESS));
            BOOST_TEST((testnetConclaveScriptAddress == TESTNET_CONCLAVE_SCRIPT_ADDRESS));
        }
        
        BOOST_AUTO_TEST_CASE(AddressStringConstructorsTest)
        {
            // Test string constructor
            Address mainnetClassicPubkeyAddress(MAINNET_CLASSIC_PUBKEY_ADDRESS);
            Address mainnetClassicScriptAddress(MAINNET_CLASSIC_SCRIPT_ADDRESS);
            Address mainnetSegwitPubkeyAddress(MAINNET_SEGWIT_PUBKEY_ADDRESS);
            Address mainnetSegwitScriptAddress(MAINNET_SEGWIT_SCRIPT_ADDRESS);
            Address mainnetConclavePubkeyAddress(MAINNET_CONCLAVE_PUBKEY_ADDRESS);
            Address mainnetConclaveScriptAddress(MAINNET_CONCLAVE_SCRIPT_ADDRESS);
            Address testnetClassicPubkeyAddress(TESTNET_CLASSIC_PUBKEY_ADDRESS);
            Address testnetClassicScriptAddress(TESTNET_CLASSIC_SCRIPT_ADDRESS);
            Address testnetSegwitPubkeyAddress(TESTNET_SEGWIT_PUBKEY_ADDRESS);
            Address testnetSegwitScriptAddress(TESTNET_SEGWIT_SCRIPT_ADDRESS);
            Address testnetConclavePubkeyAddress(TESTNET_CONCLAVE_PUBKEY_ADDRESS);
            Address testnetConclaveScriptAddress(TESTNET_CONCLAVE_SCRIPT_ADDRESS);
            BOOST_TEST(mainnetClassicPubkeyAddress == MAINNET_CLASSIC_PUBKEY_ADDRESS);
            BOOST_TEST(mainnetClassicScriptAddress == MAINNET_CLASSIC_SCRIPT_ADDRESS);
            BOOST_TEST(mainnetSegwitPubkeyAddress == MAINNET_SEGWIT_PUBKEY_ADDRESS);
            BOOST_TEST(mainnetSegwitScriptAddress == MAINNET_SEGWIT_SCRIPT_ADDRESS);
            BOOST_TEST(mainnetConclavePubkeyAddress == MAINNET_CONCLAVE_PUBKEY_ADDRESS);
            BOOST_TEST(mainnetConclaveScriptAddress == MAINNET_CONCLAVE_SCRIPT_ADDRESS);
            BOOST_TEST(testnetClassicPubkeyAddress == TESTNET_CLASSIC_PUBKEY_ADDRESS);
            BOOST_TEST(testnetClassicScriptAddress == TESTNET_CLASSIC_SCRIPT_ADDRESS);
            BOOST_TEST(testnetSegwitPubkeyAddress == TESTNET_SEGWIT_PUBKEY_ADDRESS);
            BOOST_TEST(testnetSegwitScriptAddress == TESTNET_SEGWIT_SCRIPT_ADDRESS);
            BOOST_TEST(testnetConclavePubkeyAddress == TESTNET_CONCLAVE_PUBKEY_ADDRESS);
            BOOST_TEST(testnetConclaveScriptAddress == TESTNET_CONCLAVE_SCRIPT_ADDRESS);
        }
        
        BOOST_AUTO_TEST_CASE(AddressPublicKeyConstructorsTest)
        {
            // Test pubkey constructor
            Address mainnetClassicPubkeyAddress(PUBLIC_KEY, Address::AddressFormat::CLASSIC,
                                                Address::NetworkType::MAINNET);
            Address mainnetConclavePubkeyAddress(PUBLIC_KEY, Address::AddressFormat::CONCLAVE,
                                                 Address::NetworkType::MAINNET);
            Address testnetClassicPubkeyAddress(PUBLIC_KEY, Address::AddressFormat::CLASSIC,
                                                Address::NetworkType::TESTNET);
            Address testnetConclavePubkeyAddress(PUBLIC_KEY, Address::AddressFormat::CONCLAVE,
                                                 Address::NetworkType::TESTNET);
            BOOST_TEST((mainnetClassicPubkeyAddress == "1FVsnLgYbZxtPA9QRnUxRV8GAboNxvjGVv"));
            BOOST_TEST((mainnetConclavePubkeyAddress == "5VyB5gP3iYRQeQDfFKLYRz9BkX2gzbHm"));
            BOOST_TEST((testnetClassicPubkeyAddress == "mv1q5PmXQbQ9AGd29MTLFQLb2bQ5vGFJuH"));
            BOOST_TEST((testnetConclavePubkeyAddress == "2rda2VK2yEtQodw3k5WZm1QJ7HeQab5d"));
        }
    
    BOOST_AUTO_TEST_SUITE_END()
}