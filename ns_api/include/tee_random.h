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
 * @file        tee_random.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_RANDOM_H
#define TEE_RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "tee_status.h"

#define TEE_RANDOM    (0x0F)

tee_status_t tee_generate_random(uint8_t *output, size_t output_size);

#ifdef __cplusplus
}
#endif

#endif /* TEE_RANDOM_H */
/** @} */
