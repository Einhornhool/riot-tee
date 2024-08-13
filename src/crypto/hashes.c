/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        hashes.c
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#include "tee_hashes.h"
#include "tee_secure_io.h"
#include "ocrypto_sha256.h"
#include "io_sanitizer.h"

#define TEE_MAX_HASH_CTX        (8)

ocrypto_sha256_ctx sha256_ctx[TEE_MAX_HASH_CTX];
uint8_t sha256_ctx_cnt = 0;

tee_status_t hashes_sha256_setup(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (sha256_ctx_cnt >= TEE_MAX_HASH_CTX)
    {
        return TEE_ERROR_INSUFFICIENT_MEMORY;
    }

    if (out_len != 1)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    tee_hash_ctx_t *ctx = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (ctx == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    ocrypto_sha256_init(&sha256_ctx[sha256_ctx_cnt]);
    *ctx = sha256_ctx_cnt;

    sha256_ctx_cnt++;

    (void) in;
    (void) in_len;
    return TEE_SUCCESS;
}

tee_status_t hashes_sha256_update(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    tee_hash_ctx_t *ctx = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *input = cmse_check_address_range(in[1].data, in[1].len, CMSE_NONSECURE);

    if (ctx == NULL || input == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t input_size = in[1].len;

    ocrypto_sha256_update(&sha256_ctx[*ctx], input, input_size);

    (void) out;
    (void) out_len;
    return TEE_SUCCESS;
}

tee_status_t hashes_sha256_finish(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 2)
    {
        return TEE_ERROR_INVALID_ARGUMENT;
    }

    tee_hash_ctx_t *ctx = cmse_check_address_range(in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash_out = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    size_t *hash_len_out = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (ctx == NULL || hash_out == NULL || hash_len_out == NULL) {
        return TEE_ERROR_CORRUPTION_DETECTED;
    }

    size_t hash_out_size = out[0].len;

    ocrypto_sha256_final(&sha256_ctx[*ctx], hash_out);

    *hash_len_out = ocrypto_sha256_BYTES;

    return TEE_SUCCESS;
}
