/*
 * Copyright (C) 2024 Lena Boeckmann
 *
 * This file is subject to the terms and conditions of the GNU GENERAL PUBLIC LICENSE.
 * See the file LICENSE in the top level directory for more details.
 */

/**
 * @file        ns_entry.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef NS_ENTRY_H
#define NS_ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "secure_io.h"
#include "tee_status.h"

/**
 * @brief   Generic NS entry function
 *
 * @param in
 * @param out
 */
tee_status_t ns_entry(int32_t operation,
                      io_pack_t *in,
                      io_pack_t *out);

#ifdef __cplusplus
}
#endif

#endif /* NS_ENTRY_H */
/** @} */
