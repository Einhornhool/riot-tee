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

#include <stdint.h>
#include <stddef.h>
#include "tee_status.h"
#include "tee_crypto.h"

#define TEE_CRYPTO_OPERATION_BASE (0x1000)
#define TEE_OPERATION_BASE_MASK   (0xF000)
#define TEE_OPERATION_MASK        (0xFF00)

typedef int32_t tee_operation_t;

#ifdef __cplusplus
}
#endif

#endif /* TEE_CRYPTO_H */
/** @} */
