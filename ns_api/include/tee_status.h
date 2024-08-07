/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        tee_status.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_STATUS_H
#define TEE_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define TEE_SUCCESS                     (0)
#define TEE_ERROR_NOT_PERMITTED         (-133)
#define TEE_ERROR_NOT_SUPPORTED         (-134)
#define TEE_ERROR_INVALID_ARGUMENT      (-135)
#define TEE_ERROR_BUFFER_TOO_SMALL      (-138)
#define TEE_ERROR_ALREADY_EXISTS        (-139)
#define TEE_ERROR_INSUFFICIENT_MEMORY   (-141)

typedef int32_t tee_status_t;

#ifdef __cplusplus
}
#endif

#endif /* TEE_STATUS_H */
/** @} */
