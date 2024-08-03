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

#include "tee_status.h"
#include "tee_secure_io.h"

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
 * @return  TEE_SUCCESS
 *          TEE_ERROR_ALREADY_EXISTS
 */
tee_status_t rot_try_generate_aes_key(void);

/**
 * @brief   Try to generate a new platform ECC P256 key
 *
 *          When booting a platform for the first time, this function generates
 *          a platform P256 key and stores it in a hardware key slot on the platform.
 *          Should be executed during secure side setup.
 *
 *          Key only becomes available after reset.
 *
 *          Skips if the key already exists.
 *
 * @return  TEE_SUCCESS
 *          TEE_ERROR_ALREADY_EXISTS
 */
tee_status_t rot_try_generate_p256_key(void);

/**
 * @brief   Encrypt key data with the platform AES key
 *
 * @param[in]   key_in          Pointer to plain key to encrypt
 * @param       key_in_size     Size of plain key
 * @param[out]  cipher_out      Pointer to buffer to store encrypted key
 * @param       cipher_out_size Size of buffer to store encrypted key
 *
 * @return  TEE_SUCCESS
 *          TEE_ERROR_NOT_PERMITTED
 */
tee_status_t rot_encrypt_key(uint8_t *key_in, size_t key_in_size, uint8_t *cipher_out, size_t cipher_out_size);

/**
 * @brief   Decrypt key data with the platform AES key
 *
 * @param[in]   key_in          Pointer to encrypted key
 * @param       key_in_size     Size of encrypted key
 * @param[out]  key_out         Pointer to buffer to store decrypted key
 * @param       key_out_size    Size of buffer to store decrypted key
 * @return  TEE_SUCCESS
 *          TEE_ERROR_NOT_PERMITTED
 */
tee_status_t rot_decrypt_key(uint8_t *key_in, size_t key_in_size, uint8_t *key_out, size_t key_out_size);

/**
 * @brief   Sign some data with the RoT ECC P256 key
 *
 * @param   in
 * @param   out
 * @return tee_status_t
 */
tee_status_t rot_sign(io_pack_t *in, io_pack_t *out);

/**
 * @brief   Generate the public key of the RoT ECC P256 key
 *
 * @param   in
 * @param   out
 * @return tee_status_t
 */
tee_status_t rot_export_public_key(io_pack_t *in, io_pack_t *out);

/**
 * @brief Builds hash from uninitialized SRAM and stores it to use as random seed
 */
void rot_get_random_seed(void);

/**
 * @brief   Initializes the random number generator with the seed generated from SRAM
 */
void rot_init_random_with_seed(void);

#ifdef __cplusplus
}
#endif

#endif /* KEYSTORAGE_H */
