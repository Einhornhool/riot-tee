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
#include <stddef.h>

#include "tee_status.h"
#include "operation_table.h"

__attribute__((cmse_nonsecure_entry))
tee_status_t ns_entry(io_operation_info_t *op_info,
                      io_pack_in_t *in,
                      io_pack_out_t *out)
{
    tee_operation_t function = tee_operation_table[op_info->operation];
    if (function == NULL) {
        return TEE_ERROR_NOT_SUPPORTED;
    }

    return function(in, op_info->in_len , out, op_info->out_len);
}
