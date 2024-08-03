/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        rot_services.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef ROT_SERVICES_H
#define ROT_SERVICES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "tee_status.h"

#define TEE_ROT_SIGN            (0x34)
#define TEE_ROT_PUBKEY          (0x35)

/**
 * @brief   Sign input data with the platform ECC P256 key
 *
 * @param[in]   input               Data to sign
 * @param       input_length        Length of input data
 * @param[out]  signature           Signature output buffer
 * @param       signature_size      Size of signature output buffer
 * @param[out]  signature_length    Actual length of generated signature
 * @return tee_status_t
 */
tee_status_t tee_rot_sign(uint8_t *input, size_t input_length, uint8_t *signature, size_t signature_size, size_t *signature_length);

/**
 * @brief   Export a public key from the platform ECC P256 key
 *
 * @param[out]  key         Output buffer for the public key
 * @param       key_size    Size of the output buffer
 * @param[out]  key_length  Actual length of the public key
 * @return tee_status_t
 */
tee_status_t rot_export_public_key(uint8_t *key, size_t key_size, size_t *key_length);

#ifdef __cplusplus
}
#endif

#endif /* ROT_SERVICES_H */
