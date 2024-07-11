/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        hashes.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef HASHES_H
#define HASHES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tee_secure_io.h"
#include "tee_status.h"

tee_status_t tee_sha256_setup(io_pack_t *in, io_pack_t *out);
tee_status_t tee_sha256_update(io_pack_t *in, io_pack_t *out);
tee_status_t tee_sha256_finish(io_pack_t *in, io_pack_t *out);

#ifdef __cplusplus
}
#endif

#endif /* HASHES_H */
/** @} */
