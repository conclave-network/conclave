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
#include <bitcoin/system.hpp>
#include <bitcoin/system/math/checksum.hpp>
#include <vector>
#include <string>
#include <openssl/bn.h>

using namespace bc::system;
namespace conclave
{
    //
    // Helper functions
    //
    
    static data_chunk convertBits(uint32_t from_bits, uint32_t to_bits, bool pad, const data_chunk& in, size_t
    in_offset)
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
        uint64_t checksum = BN_mod_word(addressBignum, CONCLAVE_CHECKSUM_MODULUS);
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
    
    static Address::StandardAddressPrefix getStandardAddressPrefix(const Address::NetworkType& networkType,
                                                                   const Address::StandardAddressType standardAddressType)
    {
        if (networkType == Address::NetworkType::MAINNET) {
            if (standardAddressType == Address::StandardAddressType::P2PKH) {
                return Address::StandardAddressPrefix::BITCOIN_MAINNET_P2PKH;
            } else if (standardAddressType == Address::StandardAddressType::P2SH) {
                return Address::StandardAddressPrefix::BITCOIN_MAINNET_P2SH;
            }
        } else if (networkType == Address::NetworkType::TESTNET) {
            if (standardAddressType == Address::StandardAddressType::P2PKH) {
                return Address::StandardAddressPrefix::BITCOIN_TESTNET_P2PKH;
            } else if (standardAddressType == Address::StandardAddressType::P2SH) {
                return Address::StandardAddressPrefix::BITCOIN_TESTNET_P2SH;
            }
        } else {
            throw std::logic_error("Failed to decide standard address prefix because of unknown network type");
        }
        throw std::logic_error("Failed to decide standard address prefix because of unknown standard address type");
    }
    
    static BYTE getConclaveAddressPrefix(const Address::NetworkType& networkType,
                                         const Address::ConclaveAddressType conclaveAddressType)
    {
        BYTE addressPrefix = 0b00000000;
        if (networkType == Address::NetworkType::MAINNET) {
            addressPrefix |= 0b10000000;
        } else if (networkType == Address::NetworkType::TESTNET) {
            addressPrefix |= 0b00000000;
        } else {
            throw std::logic_error("Failed to decide conclave_chain address prefix because of unknown network type");
        }
        if (conclaveAddressType == Address::ConclaveAddressType::P2PKH_ANALOG) {
            addressPrefix |= 0b01010000;
        } else if (conclaveAddressType == Address::ConclaveAddressType::P2SH_ANALOG) {
            addressPrefix |= 0b00100000;
        } else {
            throw std::logic_error(
                "Failed to decide conclave_chain address prefix because of unknown Conclave address type");
        }
        return addressPrefix;
    }
    
    static std::string getWitnessAddressPrefix(const Address::NetworkType& networkType)
    {
        return networkType == Address::NetworkType::MAINNET ? WITNESS_MAINNET_PREFIX : WITNESS_TESTNET_PREFIX;
    }
    
    //
    // Address class functions start here
    //
    
    Address::Address(const Hash160& hash, const NetworkType& networkType,
                     const StandardAddressType& standardAddressType)
        : addressFormat(AddressFormat::STANDARD), networkType(networkType),
          standardAddressType(standardAddressType), hash(hash), string(makeStandardAddressString())
    {
    }
    
    Address::Address(const Hash160& hash, const NetworkType& networkType,
                     const ConclaveAddressType& conclaveAddressType)
        : addressFormat(AddressFormat::CONCLAVE), networkType(networkType),
          conclaveAddressType(conclaveAddressType), hash(hash), string(makeConclaveAddressString())
    {
    }
    
    Address::Address(const Hash160& hash, const NetworkType& networkType,
                     const Bech32AddressType& bech32AddressType)
        : addressFormat(AddressFormat::BECH32), networkType(networkType),
          bech32AddressType(bech32AddressType), hash(hash), string(makeBech32AddressString())
    {
        if (bech32AddressType == Bech32AddressType::WITNESS_SCRIPT) {
            throw std::logic_error("Address constructor called with " +
                                   std::to_string(SMALL_HASH_SIZE_BYTES) +
                                   "-length hash but WITNESS_SCRIPT Bech32 address type");
        }
    }
    
    Address::Address(const Hash256& p2wshHash, const NetworkType& networkType,
                     const Bech32AddressType& bech32AddressType)
        : addressFormat(AddressFormat::BECH32), networkType(networkType),
          bech32AddressType(bech32AddressType), p2wshHash(p2wshHash), string(makeBech32AddressString())
    {
        if (bech32AddressType != Bech32AddressType::WITNESS_SCRIPT) {
            throw std::logic_error("Address constructor called with " +
                                   std::to_string(SMALL_HASH_SIZE_BYTES) +
                                   "-length p2wshHash but not WITNESS_SCRIPT Bech32 address type");
        }
    }
    
    Address::Address(const std::string& addressString)
    {
        data_chunk addressData;
        base32 bech32;
        if (is_base58(addressString)) {
            decode_base58(addressData, addressString);
            if (addressData.size() == STANDARD_ADDRESS_DECODED_LENGTH) {
                constructStandardAddress(addressData);
            } else if (addressData.size() == CONCLAVE_ADDRESS_DECODED_LENGTH) {
                constructConclaveAddress(addressData);
            } else {
                throw std::runtime_error("Invalid address: " + addressString);
            }
        } else if (decode_base32(bech32, addressString)) {
            constructBech32Address(bech32);
        } else {
            throw std::runtime_error("Unable to parse address: " + addressString);
        }
    }
    
    Address::Address(const char* addressString)
        : Address(std::string(addressString))
    {
    }
    
    const Hash160& Address::getHash() const
    {
        if (addressFormat == AddressFormat::BECH32 && bech32AddressType == Bech32AddressType::WITNESS_SCRIPT) {
            throw std::logic_error("getHash() called on WITNESS_SCRIPT address");
        }
        return hash;
    }
    
    const Hash256& Address::getP2wshHash() const
    {
        if (!(addressFormat == AddressFormat::BECH32 && bech32AddressType == Bech32AddressType::WITNESS_SCRIPT)) {
            throw std::logic_error("getP2wshHash() called on non WITNESS_SCRIPT address");
        }
        return p2wshHash;
    }
    
    const std::vector<BYTE> Address::getHashData() const
    {
        if (addressFormat == AddressFormat::BECH32 && bech32AddressType == Bech32AddressType::WITNESS_SCRIPT) {
            return std::vector<BYTE>(p2wshHash.begin(), p2wshHash.end());
        } else {
            return std::vector<BYTE>(hash.begin(), hash.end());
        }
    }
    
    const bool Address::isStandard() const
    {
        return (addressFormat == AddressFormat::STANDARD);
    }
    
    const bool Address::isConclave() const
    {
        return (addressFormat == AddressFormat::CONCLAVE);
    }
    
    const bool Address::isBech32() const
    {
        return (addressFormat == AddressFormat::BECH32);
    }
    
    const bool Address::isToPublicKeyHash() const
    {
        switch (addressFormat) {
            case AddressFormat::STANDARD:
                return (standardAddressType == StandardAddressType::P2PKH);
            case AddressFormat::CONCLAVE:
                return (conclaveAddressType == ConclaveAddressType::P2PKH_ANALOG);
            case AddressFormat::BECH32:
                return (bech32AddressType == Bech32AddressType::WITNESS_PUBKEY);
            default:
                return false;
        }
    }
    
    const bool Address::isToScriptHash() const
    {
        switch (addressFormat) {
            case AddressFormat::STANDARD:
                return (standardAddressType == StandardAddressType::P2SH);
            case AddressFormat::CONCLAVE:
                return (conclaveAddressType == ConclaveAddressType::P2SH_ANALOG);
            case AddressFormat::BECH32:
                return (bech32AddressType == Bech32AddressType::WITNESS_SCRIPT);
            default:
                return false;
        }
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
        return (hash == other.hash);
    }
    
    bool Address::operator!=(const Address& other) const
    {
        return (hash != other.hash);
    }
    
    Address::operator std::string() const
    {
        return this->string;
    }
    
    std::ostream& operator<<(std::ostream& os, const Address& address)
    {
        os << static_cast<std::string>(address);
        return os;
    }
    
    void Address::constructStandardAddress(const data_chunk& addressData)
    {
        if (addressData.size() != STANDARD_ADDRESS_DECODED_LENGTH) {
            throw std::runtime_error("Invalid address length");
        } else if (!verify_checksum(addressData)) {
            throw std::runtime_error("Invalid address checksum");
        } else {
            addressFormat = AddressFormat::STANDARD;
            switch (addressData[0]) {
                case StandardAddressPrefix::BITCOIN_MAINNET_P2PKH:
                    networkType = NetworkType::MAINNET;
                    standardAddressType = StandardAddressType::P2PKH;
                    break;
                case StandardAddressPrefix::BITCOIN_MAINNET_P2SH:
                    networkType = NetworkType::MAINNET;
                    standardAddressType = StandardAddressType::P2SH;
                    break;
                case StandardAddressPrefix::BITCOIN_TESTNET_P2PKH:
                    networkType = NetworkType::TESTNET;
                    standardAddressType = StandardAddressType::P2PKH;
                    break;
                case StandardAddressPrefix::BITCOIN_TESTNET_P2SH:
                    networkType = NetworkType::TESTNET;
                    standardAddressType = StandardAddressType::P2SH;
                    break;
                default:
                    throw std::runtime_error("Unknown address prefix: " + std::to_string(addressData[0]));
            }
            std::memcpy(&hash[0], &addressData[1], SMALL_HASH_SIZE_BYTES);
            string = makeStandardAddressString();
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
                    conclaveAddressType = ConclaveAddressType::P2PKH_ANALOG;
                    break;
                case 0b0010:
                    conclaveAddressType = ConclaveAddressType::P2SH_ANALOG;
                    break;
                default:
                    throw std::runtime_error("Unsupported address type");
            }
            std::memcpy(&hash[0], &addressData[3], SMALL_HASH_SIZE_BYTES);
            string = makeConclaveAddressString();
        }
    }
    
    void Address::constructBech32Address(const base32& bech32)
    {
        addressFormat = AddressFormat::BECH32;
        if (bech32.prefix == WITNESS_MAINNET_PREFIX) {
            networkType = NetworkType::MAINNET;
        } else if (bech32.prefix == WITNESS_TESTNET_PREFIX) {
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
            bech32AddressType = Bech32AddressType::WITNESS_PUBKEY;
            std::memcpy(&hash[0], &expanded[0], SMALL_HASH_SIZE_BYTES);
        } else if (expanded.size() == LARGE_HASH_SIZE_BYTES) {
            bech32AddressType = Bech32AddressType::WITNESS_SCRIPT;
            std::memcpy(&p2wshHash[0], &expanded[0], LARGE_HASH_SIZE_BYTES);
        } else {
            throw std::runtime_error("Invalid bech32 payload length:" + std::to_string(bech32.payload.size()));
        }
        string = makeBech32AddressString();
    }
    
    std::string Address::makeStandardAddressString()
    {
        one_byte addr_prefix = {
            {getStandardAddressPrefix(networkType, standardAddressType)}
        };
        data_chunk prefix_pubkey_checksum(to_chunk(addr_prefix));
        extend_data(prefix_pubkey_checksum, hash);
        append_checksum(prefix_pubkey_checksum);
        return encode_base58(prefix_pubkey_checksum);
    }
    
    std::string Address::makeConclaveAddressString()
    {
        data_chunk addressData{
            getConclaveAddressPrefix(networkType, conclaveAddressType),
            0x00, 0x00
        };
        extend_data(addressData, hash);
        unsigned long checksum = computeConclaveChecksum(addressData);
        addressData[0] |= (BYTE) (checksum >> 16);
        addressData[1] = (BYTE) (checksum >> 8);
        addressData[2] = (BYTE) checksum;
        return encode_base58(addressData);
    }
    
    std::string Address::makeBech32AddressString()
    {
        one_byte witness_version = {{0x00}};
        data_chunk hash_chunk = bech32AddressType == Bech32AddressType::WITNESS_SCRIPT ?
                                data_chunk(p2wshHash.begin(), p2wshHash.end()) :
                                data_chunk(hash.begin(), hash.end());
        data_chunk payload(to_chunk(witness_version));
        extend_data(payload, convertBits(BECH32_EXPANDED_BIT_SIZE, BECH32_CONTRACTED_BIT_SIZE, true,
                                         to_chunk(hash_chunk), 0));
        // Build the bech32 structure
        base32 bech32;
        bech32.prefix = getWitnessAddressPrefix(networkType);
        bech32.payload = payload;
        return encode_base32(bech32);
    }
}
