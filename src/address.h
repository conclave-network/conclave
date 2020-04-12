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

#include "public_key.h"
#include "hash160.h"
#include "hash256.h"
#include "conclave.h"
#include <bitcoin/system.hpp>
#include <vector>
#include <string>

/***
 * Conclave Addresses vs Bitcoin Addresses
 * ---------------------------------------
 * Bitcoin address formats such as P2PKH and P2SH consist of a 1-byte (8 bit) prefix, followed by a 20-byte (160-bit)
 * hash, followed by a 4-byte (32-bit) checksum, which is the first 4 bytes of the SHA256^2 of the 21 bytes of
 * (prefix | hash). This gives the address a total length of 25 bytes, which is then base58-encoded.
 *
 * Conclave addresses have a different format: we reduce the overall size from 25 bytes to 23 bytes, by allocating 20
 * bits for the checksum, 1 bit to denote mainnet/testnet, and the remaining 3 bits to denote 1 of 8 possible address
 * classes.
 *
 * The checksum for Conclave addresses does not use a hash function but instead a cheaper calculation using modular
 * arithmetic. As with standard chain addresses, the hash remains 160 bits and the address is base58-encoded.
 *
 * The bit-map of a Conclave address is as follows:
 * [b0 - mainnet/testnet (1 bit)] [b1-b3 Address class (3 bits)] [b4-b23 Checksum (20 bits)] [b24-b183 hash (160 bits)]
 *
 * +-----------+-----------+--------------+------------------------------------+---------------------------------------+
 * | Prefix    | Prefix    | Type         | Purpose                            | Redeemable By                         |
 * | (MainNet) | (Testnet) |              |                                    |                                       |
 * +-----------+-----------+--------------+------------------------------------+---------------------------------------+
 * | 0b1101    | 0b0101    | P2PKH_ANALOG | Sending funds to a public key hash | Anybody who possesses the underlying  |
 * |           |           |              |                                    | private key                           |
 * +-----------+-----------+--------------+------------------------------------+---------------------------------------+
 * | 0b1010    | 0b0010    | P2SH_ANALOG  | Sending funds to a script hash     | Anybody who possesses the underlying  |
 * |           |           |              |                                    | script and can solve the script.      |
 * +-----------+-----------+--------------+------------------------------------+---------------------------------------+
 */

using namespace bc::system;
namespace conclave
{
    /***
     * The `Address` class is used to represent all types of chain addresses. This class is deliberately marked final
     * and all subclassing is thus forbidden. The decision to implement all types of address in one monomorphic class
     * instead of going the subclassing route was made for several reasons:
     *
     * 1. Addresses are not very complicated things. Implementing them with full-blown polymorphism would be increasing
     *    complexity rather than decreasing it.
     * 2. Monomorphic classes are easier to work with and to reason about. Using them avoids the need for any runtime
     *    type checking with dynamic casts.
     * 3. We want to avoid all errors pertaining to covariant arrays. Covariant arrays can cause addresses to change in
     *    unexpected ways, which could potentially lead to financial loss.
     *
     * Addresses are also immutable, again to reduce potential for errors. To convert an address with the same
     * underlying hash to another format, e.g. From a testnet address to a mainnet address, several conversion methods
     * are provided.
     */
    class Address final
    {
        public:
        // Constants
        const static size_t CLASSIC_ADDRESS_DECODED_LENGTH;
        const static size_t CONCLAVE_ADDRESS_DECODED_LENGTH;
        const static std::string SEGWIT_MAINNET_PREFIX;
        const static std::string SEGWIT_TESTNET_PREFIX;
        const static size_t BECH32_CONTRACTED_BIT_SIZE;
        const static size_t BECH32_EXPANDED_BIT_SIZE;
        const static unsigned long CONCLAVE_CHECKSUM_MODULUS;
        // Enums
        enum class AddressFormat
        {
            CLASSIC,   // Most Bitcoin addresses, such as P2PKH and P2SH have this format
            SEGWIT,     // Brought in with BIP-173. See https://github.com/bitcoin/bips/blob/master/bip-0173.mediawiki
            CONCLAVE    // Shorter address which uses 23 bytes and is base58-encoded
        };
        enum class NetworkType
        {
            MAINNET,
            TESTNET
        };
        enum class PayeeType
        {
            PUBKEY,
            SCRIPT
        };
        enum ClassicAddressPrefix : BYTE
        {
            BITCOIN_MAINNET_P2PKH = 0x00,
            BITCOIN_MAINNET_P2SH = 0x05,
            BITCOIN_TESTNET_P2PKH = 0x6F,
            BITCOIN_TESTNET_P2SH = 0xC4
        };
        // Constructors
        Address(const Hash160&, const AddressFormat&, const NetworkType&, const PayeeType&);
        Address(const Hash256&, const AddressFormat&, const NetworkType&, const PayeeType&);
        Address(const PublicKey&, const AddressFormat&, const NetworkType&);
        Address(const std::string&);
        Address(const char*);
        Address(const Address&);
        Address(Address&&);
        // Public Functions
        const Hash160& getHash() const;
        const Hash256& getP2WSHash() const;
        const std::vector<BYTE> getHashData() const;
        const bool isClassic() const;
        const bool isSegwit() const;
        const bool isConclave() const;
        const bool isMainnet() const;
        const bool isTestnet() const;
        const bool payeeIsPubKey() const;
        const bool payeeIsScript() const;
        const bool isP2PKH() const;
        const bool isP2SH() const;
        const bool isP2WPKH() const;
        const bool isP2WSH() const;
        const bool isP2CPKH() const;
        const bool isP2CSH() const;
        // Conversions
        explicit operator std::string() const;
        // Operator Overloads
        Address& operator=(const Address&);
        Address& operator=(Address&&);
        bool operator==(const Address&) const;
        bool operator!=(const Address&) const;
        friend std::ostream& operator<<(std::ostream&, const Address&);
        private:
        // Private Functions
        void constructClassicAddress(const data_chunk&);
        void constructSegwitAddress(const base32&);
        void constructConclaveAddress(const data_chunk&);
        std::string makeClassicAddressString() const;
        std::string makeSegwitAddressString() const;
        std::string makeConclaveAddressString() const;
        // Properties
        Hash160 hash;
        Hash256 wsHash; // Only used if addressFormat is SEGWIT
        AddressFormat addressFormat;
        NetworkType networkType;
        PayeeType payeeType;
    };
};
