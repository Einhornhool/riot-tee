/*
 * Copyright (C) 2018 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     sys_puf_sram
 *
 * @{
 * @file
 *
 * @author      Peter Kietzmann <peter.kietzmann@haw-hamburg.de>
 *
 * @}
 */
#include <stdint.h>
#include "ocrypto_sha256.h"
#include "random.h"

extern uint32_t RAM_START;

/**
 * @brief   Attribute for memory sections required by SRAM PUF
 */
#define PUF_SRAM_ATTRIBUTES __attribute__((used, section(".puf")))

/**
 * @brief SRAM length considered for seeding
 */
#ifndef SEED_RAM_LEN
#define SEED_RAM_LEN     (2048 / sizeof(uint32_t))
#endif

/**
 * @brief SRAM marker to detect reboot without power-off
 *
 * Source: https://www.random.org/bytes/
 */
#define PUF_SRAM_MARKER  (0xACAB1312)

/* Allocation of the PUF seed variable */
PUF_SRAM_ATTRIBUTES uint32_t puf_sram_seed[ocrypto_sha256_BYTES];

/* Allocation of the PUF seed state */
PUF_SRAM_ATTRIBUTES uint32_t puf_sram_state;

/* Allocation of the PUF soft reset counter */
PUF_SRAM_ATTRIBUTES uint32_t puf_sram_softreset_cnt;

/* Allocation of the memory marker */
PUF_SRAM_ATTRIBUTES uint32_t puf_sram_marker;

void rot_get_random_seed(void)
{
    if (puf_sram_marker == PUF_SRAM_MARKER) {
        /* RAM already initialized */
        puf_sram_state = 1;

        /* increment number of detected soft resets */
        puf_sram_softreset_cnt++;

        /* generate alterntive seed value */
        puf_sram_seed[0] ^= puf_sram_softreset_cnt;
        ocrypto_sha256((uint8_t *)&puf_sram_seed, (uint8_t *)&puf_sram_seed, sizeof(puf_sram_seed));
    }
    else {
        /* build hash from start-up pattern */
        ocrypto_sha256((uint8_t *)puf_sram_seed, (uint8_t *)&RAM_START, (size_t)SEED_RAM_LEN);
        /* write marker to a defined section for subsequent reset detection */
        puf_sram_marker = PUF_SRAM_MARKER;
        /* reset counter of detected soft resets */
        puf_sram_softreset_cnt = 0;
    }
}

void rot_init_random_with_seed(void)
{
    random_init_by_array((uint32_t*)puf_sram_seed, sizeof(puf_sram_seed)/sizeof(uint32_t));
}
