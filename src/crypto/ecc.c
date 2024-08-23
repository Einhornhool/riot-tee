#include "rot.h"
#include "ecc.h"
#include "random.h"
#include "io_sanitizer.h"
#include "ocrypto_ecdsa_p256.h"

#include <stdio.h>
#define MAX_REPETITION              (10)
#define ECC_P256_PRIV_KEY_SIZE      (32)
#define ECC_P256_PUB_KEY_SIZE       (64)
#define ECC_P256_SESSION_KEY_SIZE   (32)
#define ECC_P256_SIGNATURE_SIZE     (64)
#define ECC_P256_HASH_SIZE          (32)

tee_status_t ecc_p256r1_generate_key_pair(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (out_len != 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;

    uint8_t *priv_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (priv_key == NULL || pub_key == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t priv_key_len = out[0].len;
    size_t pub_key_len = out[1].len;

    if (priv_key_len != ECC_P256_PRIV_KEY_SIZE ||
        pub_key_len != ECC_P256_PUB_KEY_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t priv_tmp[ECC_P256_PRIV_KEY_SIZE];

    do {
        random_bytes(priv_tmp, sizeof(priv_tmp));
    } while (ocrypto_ecdsa_p256_public_key(pub_key, priv_tmp) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    if (rot_encrypt_key(priv_tmp, sizeof(priv_tmp), priv_key, priv_key_len)) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    (void) in;
    (void) in_len;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_import_and_seal_key_pair(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key_in = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *priv_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (key_in == NULL || priv_key == NULL || pub_key == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t priv_key_len = out[0].len;
    size_t pub_key_len = out[1].len;

    if (key_len != ECC_P256_PRIV_KEY_SIZE ||
        priv_key_len != ECC_P256_PRIV_KEY_SIZE ||
        pub_key_len != ECC_P256_PUB_KEY_SIZE) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    if (ocrypto_ecdsa_p256_public_key(pub_key, key_in)) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    if (rot_encrypt_key(key_in, key_len, priv_key, priv_key_len)) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_sign_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;
    uint8_t *key = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range(in[1].data, in[1].len, CMSE_NONSECURE);

    uint8_t *signature = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    size_t *signature_length = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL || signature_length == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t hash_len = in[1].len;
    size_t signature_size = out[0].len;

    if (key_len != ECC_P256_PRIV_KEY_SIZE ||
        hash_len != ECC_P256_HASH_SIZE ||
        signature_size < ECC_P256_SIGNATURE_SIZE) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t key_clear[ECC_P256_PRIV_KEY_SIZE];
    uint8_t session_key[ECC_P256_SESSION_KEY_SIZE];

    if (rot_decrypt_key(key, key_len, key_clear, sizeof(key_clear))) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    do {
        random_bytes(session_key, ECC_P256_SESSION_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_sign_hash(signature, hash, key_clear, session_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    *signature_length = ECC_P256_SIGNATURE_SIZE;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_sign_message(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;
    uint8_t *key = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *input = cmse_check_address_range(in[1].data, in[1].len, CMSE_NONSECURE);

    uint8_t *signature = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    size_t *signature_length = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (key == NULL || input == NULL || signature == NULL || signature_length == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t input_len = in[1].len;
    size_t signature_size = out[0].len;

    if (key_len != ECC_P256_PRIV_KEY_SIZE ||
        signature_size < ECC_P256_SIGNATURE_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t key_clear[ECC_P256_PRIV_KEY_SIZE];
    uint8_t session_key[ECC_P256_SESSION_KEY_SIZE];

    if (rot_decrypt_key(key, key_len, key_clear, sizeof(key_clear))) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    do {
        random_bytes(session_key, ECC_P256_SESSION_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_sign(signature, input, input_len, key_clear, session_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    *signature_length = ECC_P256_SIGNATURE_SIZE;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_verify_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range(in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range(in[2].data, in[2].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t hash_len = in[1].len;
    size_t signature_len = in[2].len;

    if (key_len != ECC_P256_PUB_KEY_SIZE ||
        hash_len != ECC_P256_HASH_SIZE ||
        signature_len != ECC_P256_SIGNATURE_SIZE) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    if (ocrypto_ecdsa_p256_verify_hash(signature, hash, key)) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    (void) out;
    (void) out_len;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_verify_message(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *input = cmse_check_address_range(in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range(in[2].data, in[2].len, CMSE_NONSECURE);

    if (key == NULL || input == NULL || signature == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t input_len = in[1].len;
    size_t signature_len = in[2].len;

    if (key_len != ECC_P256_PUB_KEY_SIZE ||
        signature_len != ECC_P256_SIGNATURE_SIZE) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    if (ocrypto_ecdsa_p256_verify(signature, input, input_len, key)) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    (void) out;
    (void) out_len;
    return TEE_SUCCESS;
}
