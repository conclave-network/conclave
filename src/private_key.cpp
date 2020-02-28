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

#include "util/hex.h"
#include "private_key.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <openssl/obj_mac.h>
#include <openssl/ec.h>
#include <openssl/bn.h>

/**
 * Low level function for deriving the secp256k1 public key's bytes from private key.
 * @param privKeyBytes - private key bytes
 * @return public key bytes
 */
bool derivePubKey(const std::vector<BYTE>& privKeyBytes, std::vector<BYTE>& pubKeyBytes)
{
    BIGNUM* privKeyBn = nullptr;
    EC_KEY* ecKey = nullptr;
    EC_POINT* ecPoint = nullptr;
    BN_CTX* bnCtx = nullptr;
    const EC_GROUP* group;
    bool ret = false;
    
    // Make private key into BIGNUM
    if ((privKeyBn = BN_bin2bn(privKeyBytes.data(), PRIVKEY_SIZE_BYTES, nullptr)) == nullptr) {
        goto done;
    }
    
    // create new ec keypair
    if ((ecKey = EC_KEY_new_by_curve_name(NID_secp256k1)) == nullptr) {
        goto done;
    }
    
    // get group of the keypair
    if ((group = EC_KEY_get0_group(ecKey)) == nullptr) {
        goto done;
    }
    
    // create new point - public key
    if ((ecPoint = EC_POINT_new(group)) == nullptr) {
        goto done;
    }
    
    // create context (consider using BN_CTX_secure_new)
    if ((bnCtx = BN_CTX_new()) == nullptr) {
        goto done;
    }
    
    // perform the multiplication
    if (!EC_POINT_mul(group, ecPoint, privKeyBn, 0, 0, bnCtx)) {
        goto done;
    }
    
    // Copy the result into the vector
    if (EC_POINT_point2oct(group, ecPoint, POINT_CONVERSION_COMPRESSED, pubKeyBytes.data(), PUBKEY_SIZE_BYTES,
                           bnCtx) != PUBKEY_SIZE_BYTES) {
        goto done;
    }
    
    // Success
    ret = true;
done:
    if (bnCtx) BN_CTX_free(bnCtx);
    if (ecPoint) EC_POINT_free(ecPoint);
    if (ecKey) EC_KEY_free(ecKey);
    if (privKeyBn) BN_free(privKeyBn);
    return ret;
}

PrivateKey::PrivateKey(const std::string& hex)
    : PrivateKey(hexStringToByteVector(hex))
{
}

PrivateKey::PrivateKey(const std::vector<BYTE>& privKeyBytes)
{
    if (privKeyBytes.size() != PRIVKEY_SIZE_BYTES) {
        throw std::invalid_argument("PrivateKey should be " +
                                    std::to_string(PRIVKEY_SIZE_BYTES) + " bytes, not " +
                                    std::to_string(privKeyBytes.size()));
    }
    std::vector<BYTE> pubKeyBytes(PUBKEY_SIZE_BYTES);
    if (!derivePubKey(privKeyBytes, pubKeyBytes)) {
        throw std::runtime_error("Error deriving public key");
    }
    this->bytes = privKeyBytes;
    this->publicKey.reset(new PublicKey(pubKeyBytes));
}

const PublicKey& PrivateKey::getPublicKey() const
{
    return *publicKey;
}

