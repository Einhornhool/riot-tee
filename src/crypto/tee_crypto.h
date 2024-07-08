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
 * @file        tee_crypto.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_CRYPTO_H
#define TEE_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tee_status.h"
#include "secure_io.h"

#define TEE_CRYPTO_OPERATION_BASE (0x1000)
#define TEE_OPERATION_BASE_MASK   (0xF000)
#define TEE_OPERATION_MASK        (0xFF00)

#define TEE_HASH_OPERATION_BASE   (0x1100)
#define TEE_HASH_SHA256_SETUP     (TEE_HASH_OPERATION_BASE | 0x0001)
#define TEE_HASH_SHA256_UPDATE    (TEE_HASH_OPERATION_BASE | 0x0002)
#define TEE_HASH_SHA256_FINISH    (TEE_HASH_OPERATION_BASE | 0x0003)

typedef int32_t tee_operation_t;

tee_status_t tee_crypto_dispatch(tee_operation_t operation, io_pack_t *in, io_pack_t *out);

#ifdef __cplusplus
}
#endif

#endif /* TEE_CRYPTO_H */
/** @} */
