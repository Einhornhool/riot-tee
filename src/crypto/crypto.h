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
 * @file        tee_crypto.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef CRYPTO_H
#define CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tee_status.h"
#include "tee_crypto.h"
#include "tee_secure_io.h"

tee_status_t tee_crypto_dispatch(tee_operation_t operation, io_pack_t *in, io_pack_t *out);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_H */
/** @} */
