/*
 * Copyright (C) Lena Boeckmann
 *
 * This file is subject to the terms and conditions of the GNU GENERAL PUBLIC LICENSE.
 * See the file LICENSE in the top level directory for more details.
 */

/**
 * @file        non_secure_entry.c
 * @brief       These are non-secure entry functions to
 *              configure and toggle LEDs on the secure side
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 */
#include <arm_cmse.h>

#include "secure_secure_leds.h"

void __attribute__((cmse_nonsecure_entry)) config_leds(void)
{
    secure_config_leds();
}

void __attribute__((cmse_nonsecure_entry)) led_on(int offset)
{
    secure_led_on(offset);
}

void __attribute__((cmse_nonsecure_entry)) led_off(int offset)
{
    secure_led_off(offset);
}
