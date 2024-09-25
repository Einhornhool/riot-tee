/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        tee_operation_table.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_OPERATION_TABLE_H
#define TEE_OPERATION_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CYS/common.h"

#include "tee_operations.h"
#include "tee_cipher.h"
#include "tee_hashes.h"
#include "tee_random.h"
#include "tee_ecc.h"

typedef CYS_error_t (*tee_operation_t)(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

static const tee_operation_t tee_operation_table[] = {
    [TEE_RANDOM_GENERATE]           = tee_generate_random_bytes,
    [TEE_HASH_SHA256_SETUP]         = tee_hash_sha256_setup,
    [TEE_HASH_SHA256_UPDATE]        = tee_hash_sha256_update,
    [TEE_HASH_SHA256_FINISH]        = tee_hash_sha256_finish,
    [TEE_CIPHER_AES_128_CBC_ENCRYPT]    = tee_cipher_aes_128_cbc_encrypt,
    [TEE_CIPHER_AES_128_CBC_DECRYPT]    = tee_cipher_aes_128_cbc_decrypt,
    [TEE_CIPHER_AES_128_ECB_ENCRYPT]    = tee_cipher_aes_128_ecb_encrypt,
    [TEE_CIPHER_AES_128_ECB_DECRYPT]    = tee_cipher_aes_128_ecb_decrypt,
    [TEE_ECC_P256_IMPORT]           = NULL,
    [TEE_ECC_P256_GENERATE]         = tee_ecc_p256_generate,
    [TEE_ECC_P256_SIGN_HASH]        = tee_ecc_p256_sign_hash,
    [TEE_ECC_P256_VERIFY_HASH]      = tee_ecc_p256_verify_hash,
    [TEE_PROT_ECC_P256_GENERATE]    = tee_prot_p256_generate,
    [TEE_PROT_ECC_P256_SIGN]        = tee_prot_p256_sign
};

#define TEE_OPERATION_TABLE_SIZE (sizeof(tee_operation_table)/sizeof(tee_operation_t))

#ifdef __cplusplus
}
#endif

#endif /* TEE_OPERATION_TABLE_H */
