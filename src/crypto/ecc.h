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
 * @file        ecc.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef ECC_H
#define ECC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "tee_status.h"
#include "tee_secure_io.h"

tee_status_t ecc_generate_p256r1_key_pair(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

tee_status_t ecc_p256r1_sign_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

tee_status_t ecc_p256r1_sign_message(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

tee_status_t ecc_p256r1_verify_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

tee_status_t ecc_p256r1_verify_message(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif /* ECC_H */
/** @} */
