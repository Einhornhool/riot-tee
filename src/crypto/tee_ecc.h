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

#ifndef TEE_ECC_H
#define TEE_ECC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CYS/common.h"
#include "tee_secure_io.h"

CYS_error_t tee_prot_ecc_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_prot_ecc_p256_seal(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_prot_ecc_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_prot_ecc_p256_sign(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

/* ECC Operations */
CYS_error_t tee_ecc_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_ecc_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_ecc_p256_sign_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_ecc_p256_verify_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif /* TEE_ECC_H */
/** @} */
