#include <stddef.h>
#include <stdint.h>

#include "tee_key_storage.h"

#define TEE_KEYSTORE_P256_PUB_SIZE          (65)
#define TEE_KEYSTORE_P256_KEY_SIZE          (32)
#define TEE_KEYSTORE_P256_SEALED_KEY_SIZE   (60)
#define TEE_KEYSTORE_P256_SIG_SIZE          (64)

tee_status_t tee_import_key_p256(const uint8_t *key_in, const size_t key_in_len, uint8_t *key_out)
{
    if (key_in_len != TEE_KEYSTORE_P256_KEY_SIZE) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    io_pack_t in[1] = {
        { .data = key_in, .len = key_in_len}
    };

    io_pack_t out[1] = {
        { .data = key_out, .len = 0 }
    };

    return ns_entry(TEE_KEYSTORE_P256_IMPORT, in, out);
}

tee_status_t tee_generate_key_p256(uint8_t *privkey, const size_t privkey_buf_size, uint8_t *pubkey, const size_t pubkey_buf_size)
{
    if (privkey_buf_size < TEE_KEYSTORE_P256_KEY_SIZE ||
        pubkey_buf_size < TEE_KEYSTORE_P256_PUB_SIZE) {
        return TEE_ERROR_BUFFER_TOO_SMALL;
    }

    io_pack_t in[1] = {
        { .data = privkey, .len = privkey_buf_size },
    };

    io_pack_t out[1] = {
        { .data = pubkey, .len = pubkey_buf_size },
    };

    return ns_entry(TEE_KEYSTORE_P256_GENERATE, in, out);
}

tee_status_t tee_sign_key_p256(const uint8_t *privkey, const size_t privkey_len,
                                    const uint8_t *hash, const size_t hash_length,
                                    uint8_t *signature, const size_t sig_buf_size)
{
    if (privkey_len != TEE_KEYSTORE_P256_KEY_SIZE) {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    if (sig_buf_size < TEE_KEYSTORE_P256_SIG_SIZE) {
        return TEE_ERROR_BUFFER_TOO_SMALL;
    }

    io_pack_t in[2] = {
        { .data = privkey, .len = privkey_len },
        { .data = hash, .len = hash_length }
    };

    io_pack_t out[1] = {
        { .data = signature, .len = sig_buf_size }
    };

    return ns_entry(TEE_KEYSTORE_P256_SIGN, in, out);
}

tee_status_t tee_platform_sign_key(const uint8_t *hash,
    size_t hash_length, uint8_t *signature)
{
    (void) hash;
    (void) hash_length;
    (void) signature;
    return TEE_ERROR_NOT_SUPPORTED;
}

tee_status_t tee_platform_generate_pubkey(uint8_t *pubkey)
{
    (void) pubkey;
    return TEE_ERROR_NOT_SUPPORTED;
}
