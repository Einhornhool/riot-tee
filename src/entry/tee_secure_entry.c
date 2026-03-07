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
#include <stddef.h>
#include <stdint.h>

#include "CYS/common.h"
#include "tee_io_sanitizer.h"
#include "tee_operation_table.h"

__attribute__((cmse_nonsecure_entry))
CYS_error_t
tee_secure_entry(io_operation_info_t* op_info,
                 io_pack_in_t* in,
                 io_pack_out_t* out) {
    // op_info should be in NS memory
    if (cmse_check_address_range(op_info, sizeof(io_operation_info_t), CMSE_NONSECURE) == NULL) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    size_t in_len = op_info->in_len;
    size_t out_len = op_info->out_len;

    // in and out should also point to NS memory
    if (cmse_check_address_range(in, sizeof(io_pack_in_t) * in_len, CMSE_NONSECURE) == NULL ||
        cmse_check_address_range(out, sizeof(io_pack_out_t) * out_len, CMSE_NONSECURE) == NULL) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    int operation = op_info->operation;
    if (operation < 0 || operation >= sizeof(tee_operation_table) / sizeof(tee_operation_t)) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    tee_operation_t function = tee_operation_table[operation];
    if (function == NULL) {
        return CYS_ERROR_NOT_SUPPORTED;
    }

    return function(in, in_len, out, out_len);
}
