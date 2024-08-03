/*
 * Copyright (C) 2013 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @brief       Pseudo Random Number Generator (PRNG)
 *
 * Various implementations of a PRNG are available:
 *  - Tiny Mersenne Twister (default)
 *  - Mersenne Twister
 *  - Simple Park-Miller PRNG
 *  - Musl C PRNG
 *  - Fortuna (CS)PRNG
 *  - Hardware Random Number Generator (non-seedable)
 *    HWRNG differ in how they generate random numbers and may not use a PRNG internally.
 *    Refer to the manual of your MCU for details.
 *
 * By default, the `auto_init_random` module is enabled, which initializes the
 * PRNG on startup. However, there is no lower limit on the entropy provided at
 * that time. Unless the `periph_hwrng` module is used, entropy may be as
 * little as zero (the constant may even be the same across devices).
 *
 * @{
 *
 * @file
 * @brief       Common interface to the software PRNG
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <inttypes.h>
#include <stddef.h>

/**
 * @brief initializes PRNG with a seed
 *
 * Users only need to call this if the `auto_init_random` module is disabled,
 * or provides insufficient quality entropy.
 *
 * @warning Currently, the random module uses a global state
 * => multiple calls to @ref random_init will reset the existing
 * state of the PRNG.
 *
 * @param s seed for the PRNG
 */
void random_init(uint32_t s);

/**
 * @brief initialize by an array with array-length
 * init_key is the array for initializing keys
 * key_length is its length
 * slight change for C++, 2004/2/26
 *
 * @param init_key array of keys (seeds) to initialize the PRNG
 * @param key_length number of lements in init_key
 */
void random_init_by_array(uint32_t init_key[], int key_length);

/**
 * @brief generates a random number on [0,0xffffffff]-interval
 * @return a random number on [0,0xffffffff]-interval
 */
uint32_t random_uint32(void);

/**
 * @brief writes random bytes in the [0,0xff]-interval to memory
 */
void random_bytes(void *buf, size_t size);

#endif /* RANDOM_H */
/** @} */
