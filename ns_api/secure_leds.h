/*
 * Copyright (C) 2024 Lena Boeckmann
 *
 * This file is subject to the terms and conditions of the GNU GENERAL PUBLIC LICENSE.
 * See the file LICENSE in the top level directory for more details.
 */

/**
 * @file        secure_leds.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef SECURE_LEDS_H
#define SECURE_LEDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Configures LEDs 1-4 as outputs
 */
void config_leds(void);

/**
 * @brief   Turn LED on
 *
 * @param   offset  Index of LED to turn on
 */
void led_on(int offset);

/**
 * @brief   Turn LED off
 *
 * @param   offset  Index of LED to turn off
 */
void led_off(int offset);

#ifdef __cplusplus
}
#endif

#endif /* SECURE_LEDS_H */
/** @} */
