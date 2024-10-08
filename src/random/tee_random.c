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

#include "cc3xx_rng.h"
#include "cc3xx_drbg.h"
#include "cc3xx_error.h"
#include "CYS/common.h"

#include "tee_io_sanitizer.h"
#include "tee_secure_io.h"
#include "tee_crypto_common.h"

#define CC3XX_DRBG_INITIAL_ENTROPY_SIZE (32)
#define CC3XX_DRBG_RESEED_ENTROPY_SIZE  (32)

struct cc3xx_drbg_state_t state;

CYS_error_t tee_init_random(void)
{
    uint8_t initial_entropy[CC3XX_DRBG_INITIAL_ENTROPY_SIZE];

    NRF_CRYPTOCELL->ENABLE = 1;
    cc3xx_err_t err = cc3xx_lowlevel_rng_get_random(initial_entropy, sizeof(initial_entropy),
                                        CC3XX_RNG_CRYPTOGRAPHICALLY_SECURE);
    if (err != CC3XX_ERR_SUCCESS) {
        goto exit;
    }

    err = cc3xx_lowlevel_drbg_init(CC3XX_DRBG_HMAC, &state,
                                   initial_entropy, sizeof(initial_entropy),
                                   NULL, 0, NULL, 0);
    if (err != CC3XX_ERR_SUCCESS) {
        goto exit;
    }

exit:
    NRF_CRYPTOCELL->ENABLE = 0;
    return tee_map_error_values(err);
}

CYS_error_t tee_internal_generate_random_bytes(uint8_t * output, size_t output_size)
{
    NRF_CRYPTOCELL->ENABLE = 1;
    /* Size must be specified in bits, so it's size * 8  */
    cc3xx_err_t err = cc3xx_lowlevel_drbg_generate(&state, output_size * 8, output, NULL, 0);

    if (err == CC3XX_ERR_DRBG_RESEED_REQUIRED) {
        uint8_t reseed_entropy[CC3XX_DRBG_RESEED_ENTROPY_SIZE];
        err = cc3xx_lowlevel_rng_get_random(reseed_entropy, sizeof(reseed_entropy),
                                            CC3XX_RNG_CRYPTOGRAPHICALLY_SECURE);
        if (err != CC3XX_ERR_SUCCESS) {
            goto exit;
        }

        err = cc3xx_lowlevel_drbg_reseed(&state, reseed_entropy, sizeof(reseed_entropy), NULL, 0);
        if (err != CC3XX_ERR_SUCCESS) {
            goto exit;
        }

        err = cc3xx_lowlevel_drbg_generate(&state, output_size * 8, output, NULL, 0);
    }

exit:
    NRF_CRYPTOCELL->ENABLE = 0;
    return tee_map_error_values(err);
}

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

    return tee_internal_generate_random_bytes(buffer, size);
}
