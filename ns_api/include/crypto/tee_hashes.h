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

#include <stddef.h>
#include "tee_status.h"

#define TEE_HASH_SHA256_SETUP     (0x11)
#define TEE_HASH_SHA256_UPDATE    (0x12)
#define TEE_HASH_SHA256_FINISH    (0x13)

typedef uint8_t tee_hash_ctx_t;

tee_status_t tee_hashes_sha256_setup(void *ctx);

tee_status_t tee_hashes_sha256_update(void *ctx,
                                const uint8_t *input,
                                size_t input_length);

tee_status_t tee_hashes_sha256_finish(void *ctx,
                                uint8_t *hash,
                                size_t hash_size,
                                size_t *hash_length);

#ifdef __cplusplus
}
#endif

#endif /* TEE_HASHES_H */
/** @} */
