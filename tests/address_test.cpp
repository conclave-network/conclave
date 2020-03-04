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

#define BOOST_TEST_MODULE Address_Test

#include <boost/test/included/unit_test.hpp>
#include "../src/address.h"
#include "../src/conclave.h"

namespace conclave
{
    const std::string EXPECTED_MAINNET_STANDARD_PUBKEY_ADDRESS = "1gdiuyyUEF8ttC4JNxWJ2UV3hQYfUNE1q";
    const std::string EXPECTED_MAINNET_STANDARD_SCRIPT_ADDRESS = "32NeeTUR28ZWz3tVRUd6ieqRCDhG9XZo97";
    const std::string EXPECTED_MAINNET_WITNESS_PUBKEY_ADDRESS = "bc1qqalwppgamc425u28xt4mlcxqqy65jzr0ae5lyn";
    const std::string EXPECTED_MAINNET_WITNESS_SCRIPT_ADDRESS = "bc1qqr32wt85gv8gvyruyxre7sq5n4r5nj63ghemtmfh909qkymwhs8sjlls5z";
    const std::string EXPECTED_MAINNET_CONCLAVE_PUBKEY_ADDRESS = "5XwNrVLahqDguAwPWUsgDBgrpYkYDwJE";
    const std::string EXPECTED_MAINNET_CONCLAVE_SCRIPT_ADDRESS = "4YSGxYSKvbPpD8qACGSSD4umxLUw4wDg";
    const std::string EXPECTED_TESTNET_STANDARD_PUBKEY_ADDRESS = "mgCb1y4xHFgPfzfg1wvt7wgouh1FZrDDSx";
    const std::string EXPECTED_TESTNET_STANDARD_SCRIPT_ADDRESS = "2MsvriCQSdb4sBqX36cEyLbpgQZuS1h3TJ7";
    const std::string EXPECTED_TESTNET_WITNESS_PUBKEY_ADDRESS = "tb1qqalwppgamc425u28xt4mlcxqqy65jzr0hl0vlq";
    const std::string EXPECTED_TESTNET_WITNESS_SCRIPT_ADDRESS = "tb1qqr32wt85gv8gvyruyxre7sq5n4r5nj63ghemtmfh909qkymwhs8s9hflwd";
    const std::string EXPECTED_TESTNET_CONCLAVE_PUBKEY_ADDRESS = "2tbmoJGZxXgh4Qen1F3hYCwyBKNFow66";
    const std::string EXPECTED_TESTNET_CONCLAVE_SCRIPT_ADDRESS = "u6fuMNKBHrpNNYYh2cTY6AtK76eew1Y";
    const std::array<BYTE, SMALL_HASH_SIZE_BYTES> shortHashData{
        0x07, 0x7e, 0xe0, 0x85, 0x1d, 0xde, 0x2a, 0xaa, 0x71, 0x47,
        0x32, 0xeb, 0xbf, 0xe0, 0xc0, 0x01, 0x35, 0x49, 0x08, 0x6f
    };
    const std::array<BYTE, LARGE_HASH_SIZE_BYTES> longHashData{
        0x00, 0xe2, 0xa7, 0x2c, 0xf4, 0x43, 0x0e, 0x86,
        0x10, 0x7c, 0x21, 0x87, 0x9f, 0x40, 0x14, 0x9d,
        0x47, 0x49, 0xcb, 0x51, 0x45, 0xf3, 0xb5, 0xed,
        0x37, 0x2b, 0xca, 0x0b, 0x13, 0x6e, 0xbc, 0x0f
    };
    const std::vector<BYTE> hashData20Vector(shortHashData.begin(), shortHashData.end());
    const std::vector<BYTE> hashData32Vector(longHashData.begin(), longHashData.end());
    
    BOOST_AUTO_TEST_CASE(AddressByteArrayConstructorsTest)
    {
        // Test byte array constructors
        Address mainnetStandardPubkeyAddress(shortHashData, Address::NetworkType::MAINNET,
                                             Address::StandardAddressType::P2PKH);
        Address mainnetStandardScriptAddress(shortHashData, Address::NetworkType::MAINNET,
                                             Address::StandardAddressType::P2SH);
        Address mainnetWitnessPubkeyAddress(shortHashData, Address::NetworkType::MAINNET,
                                            Address::Bech32AddressType::WITNESS_PUBKEY);
        Address mainnetWitnessScriptAddress(longHashData, Address::NetworkType::MAINNET,
                                            Address::Bech32AddressType::WITNESS_SCRIPT);
        Address mainnetConclavePubkeyAddress(shortHashData, Address::NetworkType::MAINNET,
                                             Address::ConclaveAddressType::P2PKH_ANALOG);
        Address mainnetConclaveScriptAddress(shortHashData, Address::NetworkType::MAINNET,
                                             Address::ConclaveAddressType::P2SH_ANALOG);
        Address testnetStandardPubkeyAddress(shortHashData, Address::NetworkType::TESTNET,
                                             Address::StandardAddressType::P2PKH);
        Address testnetStandardScriptAddress(shortHashData, Address::NetworkType::TESTNET,
                                             Address::StandardAddressType::P2SH);
        Address testnetWitnessPubkeyAddress(shortHashData, Address::NetworkType::TESTNET,
                                            Address::Bech32AddressType::WITNESS_PUBKEY);
        Address testnetWitnessScriptAddress(longHashData, Address::NetworkType::TESTNET,
                                            Address::Bech32AddressType::WITNESS_SCRIPT);
        Address testnetConclavePubkeyAddress(shortHashData, Address::NetworkType::TESTNET,
                                             Address::ConclaveAddressType::P2PKH_ANALOG);
        Address testnetConclaveScriptAddress(shortHashData, Address::NetworkType::TESTNET,
                                             Address::ConclaveAddressType::P2SH_ANALOG);
        BOOST_TEST((std::string) mainnetStandardPubkeyAddress == EXPECTED_MAINNET_STANDARD_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) mainnetStandardScriptAddress == EXPECTED_MAINNET_STANDARD_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) mainnetWitnessPubkeyAddress == EXPECTED_MAINNET_WITNESS_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) mainnetWitnessScriptAddress == EXPECTED_MAINNET_WITNESS_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) mainnetConclavePubkeyAddress == EXPECTED_MAINNET_CONCLAVE_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) mainnetConclaveScriptAddress == EXPECTED_MAINNET_CONCLAVE_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) testnetStandardPubkeyAddress == EXPECTED_TESTNET_STANDARD_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) testnetStandardScriptAddress == EXPECTED_TESTNET_STANDARD_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) testnetWitnessPubkeyAddress == EXPECTED_TESTNET_WITNESS_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) testnetWitnessScriptAddress == EXPECTED_TESTNET_WITNESS_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) testnetConclavePubkeyAddress == EXPECTED_TESTNET_CONCLAVE_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) testnetConclaveScriptAddress == EXPECTED_TESTNET_CONCLAVE_SCRIPT_ADDRESS);
    }
    
    BOOST_AUTO_TEST_CASE(AddressStringConstructorsTest)
    {
        // Test string constructor
        Address mainnetStandardPubkeyAddress(EXPECTED_MAINNET_STANDARD_PUBKEY_ADDRESS);
        Address mainnetStandardScriptAddress(EXPECTED_MAINNET_STANDARD_SCRIPT_ADDRESS);
        Address mainnetWitnessPubkeyAddress(EXPECTED_MAINNET_WITNESS_PUBKEY_ADDRESS);
        Address mainnetWitnessScriptAddress(EXPECTED_MAINNET_WITNESS_SCRIPT_ADDRESS);
        Address mainnetConclavePubkeyAddress(EXPECTED_MAINNET_CONCLAVE_PUBKEY_ADDRESS);
        Address mainnetConclaveScriptAddress(EXPECTED_MAINNET_CONCLAVE_SCRIPT_ADDRESS);
        Address testnetStandardPubkeyAddress(EXPECTED_TESTNET_STANDARD_PUBKEY_ADDRESS);
        Address testnetStandardScriptAddress(EXPECTED_TESTNET_STANDARD_SCRIPT_ADDRESS);
        Address testnetWitnessPubkeyAddress(EXPECTED_TESTNET_WITNESS_PUBKEY_ADDRESS);
        Address testnetWitnessScriptAddress(EXPECTED_TESTNET_WITNESS_SCRIPT_ADDRESS);
        Address testnetConclavePubkeyAddress(EXPECTED_TESTNET_CONCLAVE_PUBKEY_ADDRESS);
        Address testnetConclaveScriptAddress(EXPECTED_TESTNET_CONCLAVE_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) mainnetStandardPubkeyAddress == EXPECTED_MAINNET_STANDARD_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) mainnetStandardScriptAddress == EXPECTED_MAINNET_STANDARD_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) mainnetWitnessPubkeyAddress == EXPECTED_MAINNET_WITNESS_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) mainnetWitnessScriptAddress == EXPECTED_MAINNET_WITNESS_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) mainnetConclavePubkeyAddress == EXPECTED_MAINNET_CONCLAVE_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) mainnetConclaveScriptAddress == EXPECTED_MAINNET_CONCLAVE_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) testnetStandardPubkeyAddress == EXPECTED_TESTNET_STANDARD_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) testnetStandardScriptAddress == EXPECTED_TESTNET_STANDARD_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) testnetWitnessPubkeyAddress == EXPECTED_TESTNET_WITNESS_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) testnetWitnessScriptAddress == EXPECTED_TESTNET_WITNESS_SCRIPT_ADDRESS);
        BOOST_TEST((std::string) testnetConclavePubkeyAddress == EXPECTED_TESTNET_CONCLAVE_PUBKEY_ADDRESS);
        BOOST_TEST((std::string) testnetConclaveScriptAddress == EXPECTED_TESTNET_CONCLAVE_SCRIPT_ADDRESS);
    }
    
    BOOST_AUTO_TEST_CASE(AddressGettersTest)
    {
        // Test getters
        Address mainnetStandardPubkeyAddress(shortHashData, Address::NetworkType::MAINNET,
                                             Address::StandardAddressType::P2PKH);
        Address mainnetWitnessScriptAddress(longHashData, Address::NetworkType::MAINNET,
                                            Address::Bech32AddressType::WITNESS_SCRIPT);
        BOOST_TEST(mainnetStandardPubkeyAddress.getHash() == shortHashData);
        BOOST_TEST(mainnetWitnessScriptAddress.getP2wshHash() == longHashData);
        BOOST_CHECK_THROW(mainnetStandardPubkeyAddress.getP2wshHash(), std::logic_error);
        BOOST_CHECK_THROW(mainnetWitnessScriptAddress.getHash(), std::logic_error);
        BOOST_TEST(mainnetStandardPubkeyAddress.getHashData() == hashData20Vector);
        BOOST_TEST(mainnetWitnessScriptAddress.getHashData() == hashData32Vector);
    }
}