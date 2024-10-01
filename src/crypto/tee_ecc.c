#include "random.h"
#include "CYS/common.h"
#include "CYS/sealed_key.h"

#include "tee_rot.h"
#include "tee_ecc.h"
#include "tee_io_sanitizer.h"

#include "ocrypto_ecdsa_p256.h"

#define MAX_REPETITION                  (10)
#define TEE_ECC_P256_PRIV_KEY_SIZE      (32)
#define TEE_ECC_P256_PUB_KEY_SIZE       (65)
#define TEE_ECC_P256_SESSION_KEY_SIZE   (32)
#define TEE_ECC_P256_SIGNATURE_SIZE     (64)
#define TEE_ECC_P256_HASH_SIZE          (32)

/* Sealed Key Operations */
CYS_error_t tee_prot_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (out_len != 2) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;

    CYS_PROT_ecc_p256_key_t *sealed_key =
        (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (sealed_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    uint8_t priv_tmp[TEE_ECC_P256_PRIV_KEY_SIZE];

    pub_key[0] = 0x04;

    do {
        random_bytes(priv_tmp, sizeof(priv_tmp));
    } while (ocrypto_ecdsa_p256_public_key(&pub_key[1], priv_tmp) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    random_bytes(sealed_key->nonce, sizeof(sealed_key->nonce));

    if (tee_rot_encrypt_key_ocb(priv_tmp, sealed_key) < 0) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

CYS_error_t tee_prot_p256_seal(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *priv_key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    CYS_PROT_ecc_p256_key_t *sealed_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (priv_key == NULL || sealed_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    random_bytes(sealed_key->nonce, sizeof(sealed_key->nonce));

    if (tee_rot_encrypt_key_ocb(priv_key, sealed_key)) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

CYS_error_t tee_prot_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_PROT_ecc_p256_key_t *key = (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    uint8_t key_clear[TEE_ECC_P256_PRIV_KEY_SIZE];
    if (tee_rot_decrypt_key_ocb(key, key_clear) < 0) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    pub_key[0] = 0x04;
    if (ocrypto_ecdsa_p256_public_key(&pub_key[1], key_clear)) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

CYS_error_t tee_prot_p256_sign(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }
    int timeout = 0;

    CYS_PROT_ecc_p256_key_t *key =
        (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t hash_len = in[1].len;
    size_t signature_size = out[0].len;

    if (hash_len != TEE_ECC_P256_HASH_SIZE ||
        signature_size < TEE_ECC_P256_SIGNATURE_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t key_clear[TEE_ECC_P256_PRIV_KEY_SIZE];
    uint8_t session_key[TEE_ECC_P256_SESSION_KEY_SIZE];

    if (tee_rot_decrypt_key_ocb(key, key_clear) < 0) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    do {
        random_bytes(session_key, TEE_ECC_P256_SESSION_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_sign_hash(signature, hash, key_clear, session_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

/* ECC Operations */
CYS_error_t tee_ecc_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 0 || out_len != 2) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;

    uint8_t *priv_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (priv_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t priv_key_len = out[0].len;
    size_t pub_key_len = out[1].len;

    if (priv_key_len != TEE_ECC_P256_PRIV_KEY_SIZE ||
        pub_key_len != TEE_ECC_P256_PUB_KEY_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    pub_key[0] = 0x04;

    do {
        random_bytes(priv_key, TEE_ECC_P256_PRIV_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_public_key(&pub_key[1], priv_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    (void) in;
    (void) in_len;
    return CYS_SUCCESS;
}

CYS_error_t tee_ecc_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *priv_key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (priv_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    pub_key[0] = 0x04;
    if (ocrypto_ecdsa_p256_public_key(&pub_key[1], priv_key)) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

CYS_error_t tee_ecc_p256_sign_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t hash_len = in[1].len;
    size_t signature_size = out[0].len;

    if (hash_len != TEE_ECC_P256_HASH_SIZE ||
        signature_size < TEE_ECC_P256_SIGNATURE_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t session_key[TEE_ECC_P256_SESSION_KEY_SIZE];

    do {
        random_bytes(session_key, TEE_ECC_P256_SESSION_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_sign_hash(signature, hash, key, session_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

CYS_error_t tee_ecc_p256_verify_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range((void *)in[2].data, in[2].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t hash_len = in[1].len;
    size_t signature_len = in[2].len;

    if (key_len != TEE_ECC_P256_PUB_KEY_SIZE ||
        hash_len != TEE_ECC_P256_HASH_SIZE ||
        signature_len != TEE_ECC_P256_SIGNATURE_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    if (ocrypto_ecdsa_p256_verify_hash(signature, hash, &key[1])) {
        return CYS_ERROR_INVALID_SIGNATURE;
    }

    (void) out;
    (void) out_len;
    return CYS_SUCCESS;
}
