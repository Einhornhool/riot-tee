/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        secure_io.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_SECURE_IO_H
#define TEE_SECURE_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"

typedef struct {
    const void* data;
    size_t len;
} io_pack_in_t;

typedef struct {
    void* data;
    size_t len;
} io_pack_out_t;

typedef struct {
    const int32_t operation;
    const size_t in_len;
    const size_t out_len;
} io_operation_info_t;

extern CYS_error_t tee_secure_entry(io_operation_info_t *op_info,
                                    io_pack_in_t *in,
                                    io_pack_out_t *out);

#ifdef __cplusplus
}
#endif

#endif /* TEE_SECURE_IO_H */
