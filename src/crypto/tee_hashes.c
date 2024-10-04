/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        tee_hashes.c
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#include "CYS/common.h"
#include "CYS/unprotected.h"

#include "cc3xx_hash.h"
#include "cc310_driver/cc310_registers.h"
#include "tee_secure_io.h"
#include "tee_io_sanitizer.h"

CYS_error_t tee_hash_sha256_setup(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_hash_sha256_ctx_t *ctx = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (ctx == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    TEE_CRYPTOCELL->ENABLE = 1;

    cc3xx_err_t status = cc3xx_lowlevel_hash_init(CC3XX_HASH_ALG_SHA256);
    if (status != CC3XX_ERR_SUCCESS) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    cc3xx_lowlevel_hash_get_state((struct cc3xx_hash_state_t *)ctx->data);

    cc3xx_lowlevel_hash_uninit();

    TEE_CRYPTOCELL->ENABLE = 0;

    (void) in;
    (void) in_len;
    return CYS_SUCCESS;
}

CYS_error_t tee_hash_sha256_update(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2)
    {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_hash_sha256_ctx_t *ctx = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *input = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);

    if (ctx == NULL || input == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t input_size = in[1].len;

    TEE_CRYPTOCELL->ENABLE = 1;

    cc3xx_lowlevel_hash_set_state((struct cc3xx_hash_state_t *)ctx->data);

    cc3xx_err_t status = cc3xx_lowlevel_hash_update(input, input_size);
    if (status != CC3XX_ERR_SUCCESS) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    cc3xx_lowlevel_hash_get_state((struct cc3xx_hash_state_t *)ctx->data);
    cc3xx_lowlevel_hash_uninit();

    TEE_CRYPTOCELL->ENABLE = 0;

    (void) out;
    (void) out_len;
    return CYS_SUCCESS;
}

CYS_error_t tee_hash_sha256_finish(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1)
    {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_hash_sha256_ctx_t *ctx = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *digest = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (ctx == NULL || digest == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    TEE_CRYPTOCELL->ENABLE = 1;

    cc3xx_lowlevel_hash_set_state((struct cc3xx_hash_state_t *)ctx->data);
    cc3xx_lowlevel_hash_finish((uint32_t *)digest, out[0].len);
    TEE_CRYPTOCELL->ENABLE = 0;
    return CYS_SUCCESS;
}
