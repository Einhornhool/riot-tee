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
 * @file        tee_io_sanitizer.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef TEE_IO_SANITIZER_H
#define TEE_IO_SANITIZER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <arm_cmse.h>

/**
 * @brief Needs to be defined as 18 for address range check
 *
 * Source: https://arm-software.github.io/acle/cmse/cmse.html#address-range-check-intrinsic-for-cmse
 */
#define CMSE_NONSECURE              18

#ifdef __cplusplus
}
#endif

#endif /* TEE_IO_SANITIZER_H */
/** @} */
