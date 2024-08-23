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
#include "random.h"
#include "ecc.h"

#include "tee_random.h"
#include "tee_hashes.h"
#include "tee_ecc.h"


typedef tee_status_t (*tee_operation_t)(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

static const tee_operation_t tee_operation_table[] = {
    [TEE_RANDOM]                    = generate_random_bytes,
    [TEE_HASH_SHA256_SETUP]         = hashes_sha256_setup,
    [TEE_HASH_SHA256_UPDATE]        = hashes_sha256_update,
    [TEE_HASH_SHA256_FINISH]        = hashes_sha256_finish,
    [TEE_ECC_P256_IMPORT]           = ecc_p256r1_import_key_pair,
    [TEE_ECC_P256_GENERATE]         = ecc_p256r1_generate_key_pair,
    [TEE_ECC_P256_SIGN_HASH]        = ecc_p256r1_sign_hash,
    [TEE_ECC_P256_SIGN_MSG]         = ecc_p256r1_sign_message,
    [TEE_ECC_P256_VERIFY_HASH]      = ecc_p256r1_verify_hash,
    [TEE_ECC_P256_VERIFY_MSG]       = ecc_p256r1_verify_message
};

#define TEE_OPERATION_TABLE_SIZE (sizeof(tee_operation_table)/sizeof(tee_operation_t))

#ifdef __cplusplus
}
#endif

#endif /* OPERATION_TABLE_H */
