#include "cc310_entropy.h"

CYS_error_t cc310_rng_get_entropy(uint32_t *buf, size_t len)
{
    if (len < (NRF_CC_RNG_OUTPUT_LEN/4)) {
        return CYS_ERROR_BUFFER_TOO_SMALL;
    }
    /* Enable CRYPTOCELL subsystem */
    TEE_CRYPTOCELL->ENABLE = CRYPTOCELL_ENABLE_ENABLE_Enabled;

    /* Enable engine clock */
    TEE_CC_RNG->RNG_CLK = TEE_CC_RNG_RNG_CLK_ENABLE_Enable;

    /* Reset engine */
    TEE_CC_RNG->RNG_SW_RESET = TEE_CC_RNG_RNG_SW_RESET_RESET_Enable;

    /* Configure sampling rate between consecutive bits */
    do {
        TEE_CC_RNG->RNG_CLK = TEE_CC_RNG_RNG_CLK_ENABLE_Enable;
        TEE_CC_RNG->SAMPLE_CNT = NRF_FICR_S->TRNG90B.ROSC2;
    } while ( TEE_CC_RNG->SAMPLE_CNT != NRF_FICR_S->TRNG90B.ROSC2 );

    /* Configure ROSC length */
    TEE_CC_RNG->TRNG_CONFIG = TEE_CC_RNG_TRNG_CONFIG_ROSC_LEN_ROSC2;

    /* Enable noise source */
    TEE_CC_RNG->NOISE_SOURCE = TEE_CC_RNG_NOISE_SOURCE_ENABLE_Enabled;

    /* Wait for random data to be sampled */
    while ((TEE_CC_RNG->RNG_ISR & TEE_CC_RNG_RNG_ISR_EHR_VALID_INT_Msk) == 0) {}

    /* Read random data */
    for (size_t i = 0; i < (NRF_CC_RNG_OUTPUT_LEN/4); i++) {
        buf[i] = TEE_CC_RNG->EHR_DATA[i];
    }

    return CYS_SUCCESS;
}
