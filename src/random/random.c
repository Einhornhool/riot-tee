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

#include "ocrypto_sha256.h"

/* state size is digest length of SHA-256 */
#define STATE_SIZE (ocrypto_sha256_BYTES)

typedef ocrypto_sha256_ctx shax_context_t;

/* allocate SHA context */
static shax_context_t ctx;

static uint32_t datapos = STATE_SIZE;
static int8_t digestdata[STATE_SIZE];
static int8_t prng_state[STATE_SIZE];

static void _updatestate(int8_t *state)
{
    signed last = 1;
    char zf = 0;

    for (int i = 0; i < STATE_SIZE; i++) {
        signed v;
        char t;
        /* add two bytes */
        v = (int)state[i] + (int)digestdata[i] + last;

        /* result is lower 8 bits */
        t = (char)(v & 0xff);

        /* check for state collision */
        zf = zf | (state[i] != t);

        /* store result, */
        state[i] = t;

        /* high 8 bits are carry, store for next iteration */
        last = (v >> 8);
    }

    /* make sure at least one bit changes! */
    if (!zf) {
       state[0]++;
    }
}

void random_bytes(uint8_t *bytes, size_t size)
{
    uint32_t loc = 0;
    while (loc < size)
    {
        int copy;
        /* find min between remaining out bytes and so far unused seed bytes */
        if ( (size-loc) < (STATE_SIZE - datapos) )
        {
            copy = size-loc;
        }
        else
        {
            /* in first iteration this will be 0 */
            copy = STATE_SIZE - datapos;
        }
        if (copy > 0)
        {
            /* directly copy data to out buffer */
            memcpy(bytes + loc, digestdata + datapos, copy);
            datapos += copy;
            loc += copy;
        }
        /* no out data ready, (re)fill internal buffer */
        else
        {
            /* reset SHA internal state */
            ocrypto_sha256_init(&ctx);

            /* update SHA internal state with PRNG state */
            ocrypto_sha256_update(&ctx, prng_state, sizeof(prng_state));

            /* get the digest */
            ocrypto_sha256_final(&ctx, digestdata);

            /* update PRNG state for next round */
            _updatestate(prng_state);

            /* reset counter for buffer position */
            datapos = 0;
        }
    }
}

void random_init_by_array(uint32_t init_key[], int key_length)
{
    ocrypto_sha256_init(&ctx);
    ocrypto_sha256_update(&ctx, (uint8_t *)init_key, key_length * sizeof(uint32_t));
    ocrypto_sha256_final(&ctx, digestdata);

    /* copy SHA digestdata to PRNG state */
    memcpy(prng_state, digestdata, STATE_SIZE);

    /* reset position indicator */
    datapos = STATE_SIZE;
}

void random_init(uint32_t seed)
{
    random_init_by_array((uint32_t *)&seed, 1);
}

uint32_t random_uint32(void)
{
    uint32_t ret;
    uint8_t bytes[sizeof(uint32_t)];
    random_bytes(bytes, sizeof(bytes));

    ret = ((uint32_t)(bytes[0] & 0xff) << 24)
        | ((uint32_t)(bytes[1] & 0xff) << 16)
        | ((uint32_t)(bytes[2] & 0xff) <<  8)
        | ((uint32_t)(bytes[3] & 0xff));

    return ret;
}
