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

#include <stddef.h>

typedef struct {
    void* data;
    size_t len;
} io_pack_t;

#ifdef __cplusplus
}
#endif

#endif /* TEE_SECURE_IO_H */
