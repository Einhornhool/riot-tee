/*
 * Copyright (C) Lena Boeckmann
 *
 * This file is subject to the terms and conditions of the GNU GENERAL PUBLIC LICENSE.
 * See the file LICENSE in the top level directory for more details.
 */

/**
 * @file        non_secure_entry.c
 * @brief       These are non-secure entry functions to
 *              configure and toggle LEDs on the secure side
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 */
#include <arm_cmse.h>
#include <stdint.h>
#include "crypto.h"
#include "tee_status.h"
#include "tee_secure_io.h"

__attribute__((cmse_nonsecure_entry))
tee_status_t ns_entry(int32_t operation,
                      io_pack_t *in,
                      io_pack_t *out)
{
    switch(operation & TEE_OPERATION_BASE_MASK) {
        case TEE_CRYPTO_OPERATION_BASE:
            return tee_crypto_dispatch(operation, in, out);
        default:
            return TEE_ERROR_NOT_SUPPORTED;
    }
}
