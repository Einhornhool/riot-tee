/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @{
 *
 * @file        tee_hashes.c
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 * @}
 */

#include "tee_hashes.h"
#include "ocrypto_sha256.h"

tee_status_t tee_hash_sha256_setup(io_pack_t *in, io_pack_t *out)
{
    ocrypto_sha256_ctx *ctx = in[0].data;
    ocrypto_sha256_init(ctx);

    (void) out;
    return TEE_SUCCESS;
}

tee_status_t tee_hash_sha256_update(io_pack_t *in, io_pack_t *out)
{
    ocrypto_sha256_ctx *ctx = in[0].data;
    uint8_t *input = in[1].data;
    size_t input_size = in[1].len;

    ocrypto_sha256_update(ctx, input, input_size);

    (void) out;
    return TEE_SUCCESS;
}

tee_status_t tee_hash_sha256_finish(io_pack_t *in, io_pack_t *out)
{
    ocrypto_sha256_ctx *ctx = in[0].data;
    uint8_t *hash_out = out[0].data;
    size_t *hash_len_out = out[1].data;
    ocrypto_sha256_final(ctx, hash_out);

    *hash_len_out = ocrypto_sha256_BYTES;

    return TEE_SUCCESS;
}
