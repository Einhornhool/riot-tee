/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @defgroup       <name> <description>
 * @{
 *
 * @file        tee_hashes.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_HASHES_H
#define TEE_HASHES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tee_status.h"
#include "tee_secure_io.h"
#include "tee_crypto.h"

#define TEE_HASH_OPERATION_BASE   (0x1100)
#define TEE_HASH_SHA256_SETUP     (TEE_HASH_OPERATION_BASE | 0x0001)
#define TEE_HASH_SHA256_UPDATE    (TEE_HASH_OPERATION_BASE | 0x0002)
#define TEE_HASH_SHA256_FINISH    (TEE_HASH_OPERATION_BASE | 0x0003)

typedef uint8_t tee_hash_ctx_t;

static inline tee_status_t tee_hashes_sha256_setup(void *ctx)
{
    io_pack_t out[1] = {
        { .data = ctx, .len = 0 }
    };

    io_pack_t *in = NULL;

    return ns_entry(TEE_HASH_SHA256_SETUP, in, out);
}

static inline tee_status_t tee_hashes_sha256_update(void *ctx,
                                const uint8_t *input,
                                size_t input_length)
{
    io_pack_t in[2] = {
        { .data = ctx, .len = 0 },
        { .data = (uint8_t *)input, .len = input_length}
    };

    io_pack_t *out = NULL;

    return ns_entry(TEE_HASH_SHA256_UPDATE, in, out);
}

static inline tee_status_t tee_hashes_sha256_finish(void *ctx,
                                uint8_t *hash,
                                size_t hash_size,
                                size_t *hash_length)
{
    io_pack_t in[1] = {
        { .data = ctx, .len = 0 }
    };

    io_pack_t out[2] = {
        { .data = hash, .len = hash_size },
        { .data = hash_length, .len = sizeof(size_t)}
    };

    return ns_entry(TEE_HASH_SHA256_UPDATE, in, out);
}

#ifdef __cplusplus
}
#endif

#endif /* TEE_HASHES_H */
/** @} */
