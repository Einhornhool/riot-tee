#include "ecc.h"
#include "random.h"
#include "ocrypto_ecdsa_p256.h"

#define MAX_REPETITION              (10)
#define ECC_P256_PRIV_KEY_SIZE      (32)
#define ECC_P256_PUB_KEY_SIZE       (64)
#define ECC_P256_SESSION_KEY_SIZE   (32)
#define ECC_P256_SIGNATURE_SIZE     (64)
#define ECC_P256_HASH_SIZE          (32)

tee_status_t ecc_generate_p256r1_key_pair(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (out_len < 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;
    uint8_t *priv_key = out[0].data;
    uint8_t *pub_key = out[1].data;
    size_t priv_key_len = out[0].len;
    size_t pub_key_len = out[1].len;

    if (priv_key_len != ECC_P256_PRIV_KEY_SIZE ||
        pub_key_len != ECC_P256_PUB_KEY_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    do {
        random_bytes(priv_key, priv_key_len);
    } while (ocrypto_ecdsa_p256_public_key(pub_key, priv_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION)
    {
        return TEE_ERROR_GENERIC_ERROR;
    }

    (void) in;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_sign_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len < 2 || out_len < 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;
    const uint8_t *key = in[0].data;
    size_t key_len = in[0].len;

    const uint8_t *hash = in[1].data;
    size_t hash_len = in[1].len;

    uint8_t *signature = out[0].data;
    size_t signature_size = out[0].len;
    size_t *signature_length = out[1].data;

    if (key_len != ECC_P256_PRIV_KEY_SIZE ||
        hash_len != ECC_P256_HASH_SIZE ||
        signature_size < ECC_P256_SIGNATURE_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t session_key[ECC_P256_SESSION_KEY_SIZE];
    do {
        random_bytes(session_key, ECC_P256_SESSION_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_sign_hash(signature, hash, key, session_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    *signature_length = ECC_P256_SIGNATURE_SIZE;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_sign_message(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len < 2 || out_len < 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    int timeout = 0;
    const uint8_t *key = in[0].data;
    size_t key_len = in[0].len;

    const uint8_t *input = in[1].data;
    size_t input_len = in[1].len;

    uint8_t *signature = out[0].data;
    size_t signature_size = out[0].len;
    size_t *signature_length = out[1].data;

    if (key_len != ECC_P256_PRIV_KEY_SIZE ||
        signature_size < ECC_P256_SIGNATURE_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    uint8_t session_key[ECC_P256_SESSION_KEY_SIZE];
    do {
        random_bytes(session_key, ECC_P256_SESSION_KEY_SIZE);
    } while (ocrypto_ecdsa_p256_sign(signature, input, input_len, key, session_key) &&
             timeout++ < MAX_REPETITION);

    if (timeout >= MAX_REPETITION) {
        return TEE_ERROR_GENERIC_ERROR;
    }

    *signature_length = ECC_P256_SIGNATURE_SIZE;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_verify_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len < 3)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    const uint8_t *key = in[0].data;
    size_t key_len = in[0].len;

    const uint8_t *hash = in[1].data;
    size_t hash_len = in[1].len;

    const uint8_t *signature = in[2].data;
    size_t signature_len = in[2].len;

    if (key_len != ECC_P256_PUB_KEY_SIZE ||
        hash_len != ECC_P256_HASH_SIZE ||
        signature_len != ECC_P256_SIGNATURE_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    if (ocrypto_ecdsa_p256_verify_hash(signature, hash, key))
    {
        return TEE_ERROR_GENERIC_ERROR;
    }

    (void) out;
    return TEE_SUCCESS;
}

tee_status_t ecc_p256r1_verify_message(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len < 3)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    const uint8_t *key = in[0].data;
    size_t key_len = in[0].len;

    const uint8_t *input = in[1].data;
    size_t input_len = in[1].len;

    const uint8_t *signature = in[2].data;
    size_t signature_len = in[2].len;

    if (key_len != ECC_P256_PUB_KEY_SIZE ||
        signature_len != ECC_P256_SIGNATURE_SIZE)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    if (ocrypto_ecdsa_p256_verify(signature, input, input_len, key))
    {
        return TEE_ERROR_GENERIC_ERROR;
    }

    (void) out;
    return TEE_SUCCESS;
}
