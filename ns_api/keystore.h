#pragma once

#include <stddef.h>

#include "uapi/ecall.h"

#define UAPI_KEYSTORE_P256_PUB_SIZE (65)
#define UAPI_KEYSTORE_P256_KEY_SIZE (32)
#define UAPI_KEYSTORE_P256_SEALED_KEY_SIZE (60)
#define UAPI_KEYSTORE_P256_SIG_SIZE (64)

#define UAPI_KEYSTORE_SUCCESS (0)
#define UAPI_KEYSTORE_ERROR (-1)

static inline int uapi_keystore_p256_import(const uint8_t *key_in, uint8_t *key_out)
{
    return _ecall2(ECALL_KEYSTORE_P256_IMPORT, (uint32_t)key_in, (uint32_t)key_out);
}

static inline int uapi_keystore_p256_generate(uint8_t *privkey, uint8_t *pubkey)
{
    return _ecall2(ECALL_KEYSTORE_P256_GENREATE, (uint32_t)privkey, (uint32_t)pubkey);
}

static inline int uapi_keystore_p256_sign(const uint8_t *privkey, const uint8_t *hash, 
    size_t hash_length, uint8_t *signature)
{
    return _ecall4(ECALL_KEYSTORE_P256_SIGN, (uint32_t)privkey, (uint32_t)hash, 
        (uint32_t)hash_length, (uint32_t)signature);
}

static inline int uapi_keystore_platform_sign(const uint8_t *hash, 
    size_t hash_length, uint8_t *signature)
{
    return _ecall3(ECALL_KEYSTORE_PLARFORM_SIGN, (uint32_t)hash, 
        (uint32_t)hash_length, (uint32_t)signature);
}

static inline int uapi_keystore_platform_pubkey(uint8_t *pubkey)
{
    return _ecall1(ECALL_KEYSTORE_PLATFORM_PUBKEY, (uint32_t)pubkey);
}

