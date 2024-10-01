/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        keystorage.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef KEYSTORAGE_H
#define KEYSTORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tee_secure_io.h"
#include "CYS/common.h"
#include "CYS/sealed_key.h"

#define KEYSTORAGE_PLATFORM_AES_KEY     (0x1)
#define KEYSTORAGE_PLATFORM_P256_KEY    (0x2)

/**
 * @brief   Try to generate a new platform AES key
 *
 *          When booting a platform for the first time, this function generates
 *          a platform AES key and stores it in a hardware key slot on the platform.
 *          Should be executed during secure side setup.
 *
 *          Key only becomes available after reset.
 *
 *          Skips if the key already exists.
 *
 * @warning For testing and development purposes, the current implementation
 *          sets a hard coded test key. This is insecure and will be changed in the future.
 *
 * @return  CYS_SUCCESS
 *          CYS_ERROR_ALREADY_EXISTS
 */
CYS_error_t tee_rot_try_generate_aes_key(void);

/**
 * @brief   Encrypt a key with the RoT ECC P256 key
 *
 * @param   key_in
 * @param   key_in_size
 * @param   tag_size
 * @param   nonce
 * @param   nonce_size
 * @param   cipher_out
 * @return  CYS_error_t
 */
CYS_error_t tee_rot_encrypt_key_ocb(uint8_t *key_in, CYS_PROT_ecc_p256_key_t *sealed_key);

/**
 * @brief   Decrypt a key with the RoT ECC P256 key
 *
 * @param   sealed_key
 * @param   key_out
 * @return  CYS_error_t
 */
CYS_error_t tee_rot_decrypt_key_ocb(CYS_PROT_ecc_p256_key_t *sealed_key, uint8_t *key_out);
/**
 * @brief   Sign some data with the RoT ECC P256 key
 *
 * @param   in
 * @param   out
 * @return CYS_error_t
 */
CYS_error_t rot_sign(const io_pack_in_t *in, const size_t in_len, io_pack_out_t *out, const size_t out_len);

/**
 * @brief   Generate the public key of the RoT ECC P256 key
 *
 * @param   in
 * @param   out
 * @return CYS_error_t
 */
CYS_error_t rot_export_public_key(const io_pack_in_t *in, const size_t in_len, io_pack_out_t *out, const size_t out_len);

/**
 * @brief   Initializes the random number generator with the seed generated from SRAM
 */
void rot_init_random_with_seed(void);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTORAGE_H */
