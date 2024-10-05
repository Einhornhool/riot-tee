/*
 * Copyright (C) 2024 Lena Boeckmann
 *
 * This file is subject to the terms and conditions of the GNU GENERAL PUBLIC LICENSE.
 * See the file LICENSE in the top level directory for more details.
 *
 * The content extends the code posted here:
 * https://devzone.nordicsemi.com/f/nordic-q-a/96093/nrf9160-porting-the-modem-library-to-work-with-bare-metal-application
 */

/**
 * @file        main_s.c
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#include <stdint.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>

#include <arm_cmse.h>

#include "nrf_spu.h"
#include "nrfx.h"

#include "CYS/common.h"

#include "cc3xx_init.h"
#include  "cc310_driver/cc310_registers.h"
#include "tee_random.h"
#include "tee_rot.h"

extern unsigned int FLASH_START_NS;

/* Define the start of the non-secure vector table */
const unsigned long TZ_START_NS = (unsigned int) (&FLASH_START_NS);

/* Define the function pointer type for the non-secure reset handler */
typedef int __attribute__((cmse_nonsecure_call)) nsfunc(void);

int main(void)
{
    /* Flash is separated into 32 * 32KB regions.
       The first four (128KB) are secure, all others are NS */
    for (uint32_t i = 4; i < 32; i++) {
        /* Initial value is 0x0001 0101
           NS value is 0x0000 0101 */
        NRF_SPU_S->FLASHREGION[i].PERM &=
            ~(SPU_FLASHREGION_PERM_SECATTR_Msk);
    }

    /* RAM is separated into 32 * 8 KB regions.
       The first 8 are secure, all others are NS */
    for (uint32_t i = 11; i < 32; ++i) {
        /* Initial value is 0x0001 0101
           NS value is 0x0000 0101 */
        NRF_SPU_S->RAMREGION[i].PERM &=
            ~(SPU_RAMREGION_PERM_SECATTR_Msk);
    }

    /* NSC flash region needs to be configured at the
       end of the last flash region.
       There are two possible NSC regions, so
       flash_nsc_id can be 0 or 1. We only have one
       region, so we use ID 0 */
    int flash_nsc_id = 0;
    int flash_region = 3;

    /* We configure the end of flash region 1 as NSC
       NSC region is 32B, which is the smallest possible size */
    NRF_SPU_S->FLASHNSC[flash_nsc_id].REGION = flash_region;
    NRF_SPU_S->FLASHNSC[flash_nsc_id].SIZE = NRF_SPU_NSC_SIZE_32B;

    /* Configure GPIO P0 for NS access */
    NRF_SPU_S->PERIPHID[NRFX_PERIPHERAL_ID_GET(NRF_P0_NS)].PERM &= ~(SPU_FLASHREGION_PERM_SECATTR_Msk);

    /* Configure UARTE0 for NS access */
    NRF_SPU_S->PERIPHID[NRFX_PERIPHERAL_ID_GET(NRF_UARTE0_NS)].PERM &= ~(SPU_FLASHREGION_PERM_SECATTR_Msk);

    /* Configure TIMER0 for NS access */
    NRF_SPU_S->PERIPHID[NRFX_PERIPHERAL_ID_GET(NRF_TIMER0_NS)].PERM &= ~(SPU_FLASHREGION_PERM_SECATTR_Msk);

    /* Configure TIMER1 for NS access */
    NRF_SPU_S->PERIPHID[NRFX_PERIPHERAL_ID_GET(NRF_TIMER1_NS)].PERM &= ~(SPU_FLASHREGION_PERM_SECATTR_Msk);

    /* Set GPIO P0 pin attributes to 0 (= non-secure) */
    NRF_SPU_S->GPIOPORT[0].PERM = 0x00000000ul;

    /* Make sure floating point registers are cleared when returning to non-secure world */
    FPU->FPCCR |= FPU_FPCCR_TS_Msk | FPU_FPCCR_CLRONRET_Msk | FPU_FPCCR_CLRONRETS_Msk;

    /* Raise NS exception priority to 0x80 to prevent preemption of secure fault exceptions */
    SCB->AIRCR |= SCB_AIRCR_PRIS_Msk;

    /* Initialize the random number generator */
    CYS_error_t status = tee_init_random();
    if (status != CYS_SUCCESS) {
        puts("Random number generator initialization failed");
    }

    /* Initialize the CryptoCell */
    TEE_CRYPTOCELL->ENABLE = 1;
    cc3xx_lowlevel_init();
    TEE_CRYPTOCELL->ENABLE = 0;

    status = tee_rot_try_generate_aes_key();
    if (status != CYS_SUCCESS && status != CYS_ERROR_ALREADY_EXISTS) {
        puts("AES Platform key generation failed");
    }

    /* Write NS vector table to SCB_NS->VTOR to be able to jump to NS image*/
    SCB_NS->VTOR = TZ_START_NS;
    uint32_t* vtor = (uint32_t*)TZ_START_NS;

    /* Set non-secure main stack pointer to beginning of NS stack */
    __TZ_set_MSP_NS(vtor[0]);

    /* Call the non-secure reset handler */
    nsfunc *ns_reset_handler = (nsfunc*)(vtor[1]);
    ns_reset_handler();

    while (1) {}

    /* Execution should not come here during normal operation */
    return ( EXIT_FAILURE );
}
