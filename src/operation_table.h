/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        operation_table.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef OPERATION_TABLE_H
#define OPERATION_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hashes.h"
#include "tee_hashes.h"
#include "tee_ecc.h"
#include "rot_services.h"

typedef tee_status_t (*tee_operation_t)(io_pack_t *in, io_pack_t *out);

static const tee_operation_t tee_operation_table[] = {
    [TEE_HASH_SHA256_SETUP]         = tee_sha256_setup,
    [TEE_HASH_SHA256_UPDATE]        = tee_sha256_update,
    [TEE_HASH_SHA256_FINISH]        = tee_sha256_finish,
    [TEE_ECC_P256_IMPORT]           = NULL,
    [TEE_ECC_P256_GENERATE]         = NULL,
    [TEE_ECC_P256_SIGN_HASH]        = NULL,
    [TEE_ECC_P256_SIGN_MSG]         = NULL,
    [TEE_ECC_P256_VERIFY_HASH]      = NULL,
    [TEE_ECC_P256_VERIFY_MSG]       = NULL,
    [TEE_ROT_SIGN]                  = NULL,
    [TEE_ROT_PUBKEY]                = NULL
};

#define TEE_OPERATION_TABLE_SIZE (sizeof(tee_operation_table)/sizeof(tee_operation_t))

#ifdef __cplusplus
}
#endif

#endif /* OPERATION_TABLE_H */
