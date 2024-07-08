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
 * @file        tee_crypto.c
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 * @}
 */

#include "tee_hashes.h"
#include "tee_crypto.h"

static tee_status_t tee_crypto_hash_dispatch(tee_operation_t operation, io_pack_t *in, io_pack_t *out) {
    switch(operation) {
    case TEE_HASH_SHA256_SETUP:
        return tee_hash_sha256_setup(in, out);
    case TEE_HASH_SHA256_UPDATE:
        return tee_hash_sha256_update(in, out);
    case TEE_HASH_SHA256_FINISH:
        return tee_hash_sha256_finish(in, out);
    default:
        return TEE_ERROR_NOT_SUPPORTED;
    }
}

tee_status_t tee_crypto_dispatch(tee_operation_t operation, io_pack_t *in, io_pack_t *out)
{
    switch(operation & TEE_OPERATION_MASK) {
        case TEE_HASH_OPERATION_BASE:
            return tee_crypto_hash_dispatch(operation, in, out);
        default:
            return TEE_ERROR_NOT_SUPPORTED;
    }
}
