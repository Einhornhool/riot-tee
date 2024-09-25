/*
 * Copyright (C) 2018, 2020 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * Code referring to Docjar
 * (http://www.docjar.net/html/api/gnu/java/security/provider/SHA1PRNG.java.html)
 */

 /**
 * @{
 * @file
 *
 * @author Peter Kietzmann <peter.kietzmann@haw-hamburg.de>
 * @}
 */

#include <stdint.h>
#include <string.h>

#include "random.h"
#include "CYS/common.h"
#include "tee_io_sanitizer.h"

#include "tee_secure_io.h"

CYS_error_t tee_generate_random_bytes(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 0 || out_len != 1)
    {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *buffer = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    size_t size = out[0].len;

    if (buffer == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    random_bytes(buffer, size);
    return CYS_SUCCESS;
}
