#include "nrfx.h"
void secure_config_leds(void)
{
    NRF_P0_S->DIRSET |= (1<<5) | (1<<4) | (1<<3) | (1<<2);
}

void secure_led_on(int offset)
{
    NRF_P0_S->OUTSET |= 1<<(offset+2);
}

void secure_led_off(int offset)
{
    NRF_P0_S->OUTCLR |= 1<<(offset+2);
}
