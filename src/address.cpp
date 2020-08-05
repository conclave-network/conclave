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

#include "address.h"
#include "script.h"
#include <bitcoin/system.hpp>
#include <bitcoin/system/math/checksum.hpp>
#include <vector>
#include <string>
#include <openssl/bn.h>

using namespace bc::system;
namespace conclave
{
    //
    // Helpers
    //
    
    static data_chunk convertBits(uint32_t from_bits, uint32_t to_bits, bool pad,
                                  const data_chunk& in, size_t in_offset)
    {
        const uint32_t maximum = (1 << to_bits) - 1;
        const uint32_t maximum_accumulator = (1 << (from_bits + to_bits - 1)) - 1;
        size_t offset = 0;
        uint32_t bit_count = 0;
        uint32_t accumulator = 0;
        data_chunk out;
        for (const auto value: in) {
            if (offset++ < in_offset)
                continue;
            accumulator = ((accumulator << from_bits) | value);
            accumulator &= maximum_accumulator;
            bit_count += from_bits;
            while (bit_count >= to_bits) {
                bit_count -= to_bits;
                out.push_back((accumulator >> bit_count) & maximum);
            }
        }
        if (pad && bit_count > 0) {
            out.push_back((accumulator << (to_bits - bit_count)) & maximum);
        } else if (!pad && (bit_count >= from_bits || ((accumulator << (to_bits - bit_count)) & maximum))) {
            return {};
        }
        return out;
    }
    
    static unsigned long computeConclaveChecksum(const data_chunk addressData)
    {
        BIGNUM* addressBignum = BN_new();
        BN_bin2bn(&addressData[3], SMALL_HASH_SIZE_BYTES, addressBignum);
        // Numerically add the 4 prefix bits at the end.
        BN_mul_word(addressBignum, 16);
        BN_add_word(addressBignum, addressData[0] >> 4);
        uint64_t checksum = BN_mod_word(addressBignum, Address::CONCLAVE_CHECKSUM_MODULUS);
        if (addressBignum) {
            BN_free(addressBignum);
        }
        return checksum;
    }
    
    static unsigned long extractConclaveChecksum(const data_chunk addressData)
    {
        return ((addressData[0] & 0x0f) << 16) | (addressData[1] << 8) | addressData[2];
    }
    
    static bool verifyConclaveChecksum(const data_chunk addressData)
    {
        return (extractConclaveChecksum(addressData) == computeConclaveChecksum(addressData));
    }
    
    static Address::ClassicAddressPrefix getClassicAddressPrefix(const Address::NetworkType& networkType,
                                                                 const Address::PayeeType& payeeType)
    {
        if (networkType == Address::NetworkType::MAINNET) {
            if (payeeType == Address::PayeeType::PUBKEY) {
                return Address::ClassicAddressPrefix::BITCOIN_MAINNET_P2PKH;
            } else if (payeeType == Address::PayeeType::SCRIPT) {
                return Address::ClassicAddressPrefix::BITCOIN_MAINNET_P2SH;
            }
        } else if (networkType == Address::NetworkType::TESTNET) {
            if (payeeType == Address::PayeeType::PUBKEY) {
                return Address::ClassicAddressPrefix::BITCOIN_TESTNET_P2PKH;
            } else if (payeeType == Address::PayeeType::SCRIPT) {
                return Address::ClassicAddressPrefix::BITCOIN_TESTNET_P2SH;
            }
        }
        throw std::logic_error("Address::getClassicAddressPrefix failed");
    }
    
    static std::string getSegwitAddressPrefix(const Address::NetworkType& networkType)
    {
        return networkType == Address::NetworkType::MAINNET ?
               Address::SEGWIT_MAINNET_PREFIX :
               Address::SEGWIT_TESTNET_PREFIX;
    }
    
    static BYTE getConclaveAddressPrefix(const Address::NetworkType& networkType,
                                         const Address::PayeeType& payeeType)
    {
        BYTE addressPrefix = 0b00000000;
        if (networkType == Address::NetworkType::MAINNET) {
            addressPrefix |= 0b10000000;
        } else if (networkType == Address::NetworkType::TESTNET) {
            addressPrefix |= 0b00000000;
        }
        if (payeeType == Address::PayeeType::PUBKEY) {
            addressPrefix |= 0b01010000;
        } else if (payeeType == Address::PayeeType::SCRIPT) {
            addressPrefix |= 0b00100000;
        }
        return addressPrefix;
    }
    
    //
    // Constants
    //
    
    const size_t Address::CLASSIC_ADDRESS_DECODED_LENGTH = 25;
    const size_t Address::CONCLAVE_ADDRESS_DECODED_LENGTH = 23;
    const std::string Address::SEGWIT_MAINNET_PREFIX = "bc";
    const std::string Address::SEGWIT_TESTNET_PREFIX = "tb";
    const size_t Address::BECH32_CONTRACTED_BIT_SIZE = 5;
    const size_t Address::BECH32_EXPANDED_BIT_SIZE = 8;
    /***
     * Conclave address checksums are computed with a simple modulus operation over the 164 bits of the prefix and
     * address. The 164 bits are modulo'ed by 1048573 which is the largest prime that can fit into 20 bits.
     */
    const unsigned long Address::CONCLAVE_CHECKSUM_MODULUS = (1 << 20) - 3;
    
    //
    // Constructors
    //
    
    Address::Address(const Hash160& hash, const AddressFormat& addressFormat,
                     const NetworkType& networkType, const PayeeType& payeeType)
        : hash(hash), addressFormat(addressFormat), networkType(networkType), payeeType(payeeType)
    {
    }
    
    Address::Address(const Hash256& hash, const AddressFormat& addressFormat,
                     const NetworkType& networkType, const PayeeType& payeeType)
        : wsHash(hash), addressFormat(addressFormat), networkType(networkType), payeeType(payeeType)
    {
    }
    
    Address::Address(const PublicKey& publicKey, const AddressFormat& addressFormat,
                     const NetworkType& networkType)
        : hash(publicKey.getHash160Compressed()), addressFormat(addressFormat), networkType(networkType),
          payeeType(PayeeType::PUBKEY)
    {
    }
    
    Address::Address(const std::string& addressString)
    {
        data_chunk addressData;
        base32 bech32;
        if (is_base58(addressString)) {
            decode_base58(addressData, addressString);
            if (addressData.size() == CLASSIC_ADDRESS_DECODED_LENGTH) {
                constructClassicAddress(addressData);
                return;
            } else if (addressData.size() == CONCLAVE_ADDRESS_DECODED_LENGTH) {
                constructConclaveAddress(addressData);
                return;
            }
        } else if (decode_base32(bech32, addressString)) {
            constructSegwitAddress(bech32);
            return;
        }
        throw std::runtime_error("Unable to parse address: " + addressString);
    }
    
    Address::Address(const char* addressString)
        : Address(std::string(addressString))
    {
    }
    
    Address::Address(const Address& other)
        : hash(other.hash), wsHash(other.wsHash), addressFormat(other.addressFormat),
          networkType(other.networkType), payeeType(other.payeeType)
    {
    }
    
    Address::Address(Address&& other)
        : hash(std::move(other.hash)), wsHash(std::move(other.wsHash)), addressFormat(std::move(other.addressFormat)),
          networkType(std::move(other.networkType)), payeeType(std::move(other.payeeType))
    {
    }
    
    //
    // Public Functions
    //
    
    const Hash160& Address::getHash() const
    {
        if (isP2WSH()) {
            throw std::logic_error("getHash() called on p2wsh address");
        } else {
            return hash;
        }
    }
    
    const Hash256& Address::getP2WSHash() const
    {
        if (!isP2WSH()) {
            throw std::logic_error("getP2WSHash() called on non p2wsh address");
        } else {
            return wsHash;
        }
    }
    
    const std::vector<BYTE> Address::getHashData() const
    {
        if (isP2WSH()) {
            return std::vector<BYTE>(wsHash.begin(), wsHash.end());
        } else {
            return std::vector<BYTE>(hash.begin(), hash.end());
        }
    }
    
    const bool Address::isClassic() const
    {
        return (addressFormat == AddressFormat::CLASSIC);
    }
    
    const bool Address::isSegwit() const
    {
        return (addressFormat == AddressFormat::SEGWIT);
    }
    
    const bool Address::isConclave() const
    {
        return (addressFormat == AddressFormat::CONCLAVE);
    }
    
    const bool Address::isMainnet() const
    {
        return (networkType == NetworkType::MAINNET);
    }
    
    const bool Address::isTestnet() const
    {
        return (networkType == NetworkType::TESTNET);
    }
    
    const bool Address::payeeIsPubKey() const
    {
        return (payeeType == PayeeType::PUBKEY);
    }
    
    const bool Address::payeeIsScript() const
    {
        return (payeeType == PayeeType::SCRIPT);
    }
    
    const bool Address::isP2PKH() const
    {
        return (addressFormat == AddressFormat::CLASSIC) && (payeeType == PayeeType::PUBKEY);
    }
    
    const bool Address::isP2SH() const
    {
        return (addressFormat == AddressFormat::CLASSIC) && (payeeType == PayeeType::SCRIPT);
    }
    
    const bool Address::isP2WPKH() const
    {
        return (addressFormat == AddressFormat::SEGWIT) && (payeeType == PayeeType::PUBKEY);
    }
    
    const bool Address::isP2WSH() const
    {
        return (addressFormat == AddressFormat::SEGWIT) && (payeeType == PayeeType::SCRIPT);
    }
    
    const bool Address::isP2CPKH() const
    {
        return (addressFormat == AddressFormat::CONCLAVE) && (payeeType == PayeeType::PUBKEY);
    }
    
    const bool Address::isP2CSH() const
    {
        return (addressFormat == AddressFormat::CONCLAVE) && (payeeType == PayeeType::SCRIPT);
    }
    
    const Hash256 Address::getStandardScriptHash() const
    {
        return Script::p2hScript(*this).getSingleSHA256();
    }
    
    //
    // Conversions
    //
    
    Address::operator std::string() const
    {
        switch (addressFormat) {
            case AddressFormat::CLASSIC:
                return makeClassicAddressString();
            case AddressFormat::SEGWIT:
                return makeSegwitAddressString();
            case AddressFormat::CONCLAVE:
                return makeConclaveAddressString();
        }
        throw std::logic_error("Unknown address format");
    }
    
    //
    // Operator Overloads
    //
    
    Address& Address::operator=(const Address& other)
    {
        hash = other.hash;
        wsHash = other.wsHash;
        networkType = other.networkType;
        addressFormat = other.addressFormat;
        payeeType = other.payeeType;
        return *this;
    }
    
    Address& Address::operator=(Address&& other)
    {
        hash = std::move(other.hash);
        wsHash = std::move(other.wsHash);
        networkType = std::move(other.networkType);
        addressFormat = std::move(other.addressFormat);
        payeeType = std::move(other.payeeType);
        return *this;
    }
    
    /***
     * Two addresses are considered equal if only their underlying hashes match. This is intended to prevent bugs
     * associated with payees being incorrectly double paid if they use (for example) both a standard address and
     * a bech32 address.
     *
     * The thinking here is treat addresses as logical entities similar to bank account numbers, with can have
     * different formats such as BIC and IBAN. If a BIC and an IBAN both refer to the same account, we'd like to know
     * that rather than not know it, since the fact that they look different is already obvious. We choose to say
     * that they are equal if they are *potentially* equal, rather than requiring that they be *certainly* equal.
     *
     * If a full equality check is needed the addresses can be cast to strings and compared.
     */
    bool Address::operator==(const Address& other) const
    {
        return (isP2WSH() && other.isP2WSH()) ? (wsHash == other.wsHash) : (hash == other.hash);
    }
    
    bool Address::operator!=(const Address& other) const
    {
        return !(*this == other);
    }
    
    std::ostream& operator<<(std::ostream& os, const Address& address)
    {
        os << static_cast<std::string>(address);
        return os;
    }
    
    //
    // Private Functions
    //
    
    void Address::constructClassicAddress(const data_chunk& addressData)
    {
        if (addressData.size() != CLASSIC_ADDRESS_DECODED_LENGTH) {
            throw std::runtime_error("Invalid address length");
        } else if (!verify_checksum(addressData)) {
            throw std::runtime_error("Invalid address checksum");
        } else {
            addressFormat = AddressFormat::CLASSIC;
            switch (addressData[0]) {
                case ClassicAddressPrefix::BITCOIN_MAINNET_P2PKH:
                    networkType = NetworkType::MAINNET;
                    payeeType = PayeeType::PUBKEY;
                    break;
                case ClassicAddressPrefix::BITCOIN_MAINNET_P2SH:
                    networkType = NetworkType::MAINNET;
                    payeeType = PayeeType::SCRIPT;
                    break;
                case ClassicAddressPrefix::BITCOIN_TESTNET_P2PKH:
                    networkType = NetworkType::TESTNET;
                    payeeType = PayeeType::PUBKEY;
                    break;
                case ClassicAddressPrefix::BITCOIN_TESTNET_P2SH:
                    networkType = NetworkType::TESTNET;
                    payeeType = PayeeType::SCRIPT;
                    break;
                default:
                    throw std::runtime_error("Unknown address prefix: " + std::to_string(addressData[0]));
            }
            std::memcpy(&hash[0], &addressData[1], SMALL_HASH_SIZE_BYTES);
        }
    }
    
    void Address::constructSegwitAddress(const base32& bech32)
    {
        addressFormat = AddressFormat::SEGWIT;
        if (bech32.prefix == SEGWIT_MAINNET_PREFIX) {
            networkType = NetworkType::MAINNET;
        } else if (bech32.prefix == SEGWIT_TESTNET_PREFIX) {
            networkType = NetworkType::TESTNET;
        } else {
            throw std::runtime_error("Invalid bech32 prefix: " + bech32.prefix);
        }
        // Ensure witness V0
        if (bech32.payload[0] != 0) {
            throw std::runtime_error("Unsupported witness version: " +
                                     std::to_string((int) bech32.payload[0]));
        }
        data_chunk expanded = convertBits(BECH32_CONTRACTED_BIT_SIZE, BECH32_EXPANDED_BIT_SIZE, false,
                                          bech32.payload, 1);
        if (expanded.size() == SMALL_HASH_SIZE_BYTES) {
            payeeType = PayeeType::PUBKEY;
            std::memcpy(&hash[0], &expanded[0], SMALL_HASH_SIZE_BYTES);
        } else if (expanded.size() == LARGE_HASH_SIZE_BYTES) {
            payeeType = PayeeType::SCRIPT;
            std::memcpy(&wsHash[0], &expanded[0], LARGE_HASH_SIZE_BYTES);
        } else {
            throw std::runtime_error("Invalid bech32 payload length:" + std::to_string(bech32.payload.size()));
        }
    }
    
    void Address::constructConclaveAddress(const data_chunk& addressData)
    {
        if (addressData.size() != CONCLAVE_ADDRESS_DECODED_LENGTH) {
            throw std::runtime_error("Invalid address length");
        } else if (!verifyConclaveChecksum(addressData)) {
            throw std::runtime_error("Invalid address checksum");
        } else {
            addressFormat = AddressFormat::CONCLAVE;
            networkType = (addressData[0] & 0b10000000) ? NetworkType::MAINNET : NetworkType::TESTNET;
            switch ((addressData[0] & 0b01110000) >> 4) {
                case 0b0101:
                    payeeType = PayeeType::PUBKEY;
                    break;
                case 0b0010:
                    payeeType = PayeeType::SCRIPT;
                    break;
                default:
                    throw std::runtime_error("Unsupported address type");
            }
            std::memcpy(&hash[0], &addressData[3], SMALL_HASH_SIZE_BYTES);
        }
    }
    
    std::string Address::makeClassicAddressString() const
    {
        one_byte addr_prefix = {
            {getClassicAddressPrefix(networkType, payeeType)}
        };
        data_chunk prefix_pubkey_checksum(to_chunk(addr_prefix));
        extend_data(prefix_pubkey_checksum, hash);
        append_checksum(prefix_pubkey_checksum);
        return encode_base58(prefix_pubkey_checksum);
    }
    
    std::string Address::makeSegwitAddressString() const
    {
        one_byte witness_version = {{0x00}};
        data_chunk hash_chunk = (payeeType == PayeeType::SCRIPT) ?
                                data_chunk(wsHash.begin(), wsHash.end()) :
                                data_chunk(hash.begin(), hash.end());
        data_chunk payload(to_chunk(witness_version));
        extend_data(payload, convertBits(BECH32_EXPANDED_BIT_SIZE, BECH32_CONTRACTED_BIT_SIZE, true,
                                         to_chunk(hash_chunk), 0));
        // Build the bech32 structure
        base32 bech32;
        bech32.prefix = getSegwitAddressPrefix(networkType);
        bech32.payload = payload;
        return encode_base32(bech32);
    }
    
    std::string Address::makeConclaveAddressString() const
    {
        data_chunk addressData{
            getConclaveAddressPrefix(networkType, payeeType),
            0x00, 0x00
        };
        extend_data(addressData, hash);
        unsigned long checksum = computeConclaveChecksum(addressData);
        addressData[0] |= (BYTE) (checksum >> 16);
        addressData[1] = (BYTE) (checksum >> 8);
        addressData[2] = (BYTE) checksum;
        return encode_base58(addressData);
    }
}
