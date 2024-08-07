/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @defgroup       <name> <description>
 * @{
 *
 * @file        tee_ecc.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_ECC_H
#define TEE_ECC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "tee_status.h"

#define TEE_ECC_P256_IMPORT         (0x21)
#define TEE_ECC_P256_GENERATE       (0x22)
#define TEE_ECC_P256_SIGN_HASH      (0x23)
#define TEE_ECC_P256_SIGN_MSG       (0x24)
#define TEE_ECC_P256_VERIFY_HASH    (0x25)
#define TEE_ECC_P256_VERIFY_MSG     (0x26)

/**
 * @brief   Generate a new ECC P-256 key pair
 *
 * @param[out]  priv_key_buffer
 * @param       priv_key_buffer_size
 * @param[out]  pub_key_buffer
 * @param       pub_key_buffer_size
 * @param[in]   priv_key_buffer_length
 * @param[in]   pub_key_buffer_length
 * @return tee_status_t
 */
tee_status_t tee_generate_ecc_p256r1_key_pair(uint8_t *priv_key_buffer,
                                              size_t priv_key_buffer_size,
                                              uint8_t *pub_key_buffer,
                                              size_t pub_key_buffer_size,
                                              size_t *priv_key_buffer_length,
                                              size_t *pub_key_buffer_length);

/**
 * @brief
 *
 * @param key_buffer
 * @param key_buffer_size
 * @param hash
 * @param hash_length
 * @param signature
 * @param signature_size
 * @param signature_length
 * @return tee_status_t
 */
tee_status_t tee_ecc_p256r1_sign_hash(const uint8_t *key_buffer, size_t key_buffer_size,
                                      const uint8_t *hash, size_t hash_length,
                                      uint8_t *signature, size_t signature_size,
                                      size_t *signature_length);

tee_status_t tee_ecc_p256r1_sign_message(const uint8_t *key_buffer, size_t key_buffer_size,
                                        const uint8_t *input, size_t input_length,
                                        uint8_t *signature, size_t signature_size,
                                        size_t *signature_length);

tee_status_t tee_ecc_p256r1_verify_hash(const uint8_t *key_buffer, size_t key_buffer_size,
                                        const uint8_t *hash, size_t hash_length,
                                        const uint8_t *signature, size_t signature_length);

tee_status_t tee_ecc_p256r1_verify_message(const uint8_t *key_buffer, size_t key_buffer_size,
                                        const uint8_t *input, size_t input_length,
                                        const uint8_t *signature, size_t signature_length);

#ifdef __cplusplus
}
#endif

#endif /* TEE_ECC_H */
/** @} */
