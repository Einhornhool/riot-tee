#ifndef TEE_CRYPTO_COMMON_H
#define TEE_CRYPTO_COMMON_H

#include "nrf9160.h"

#include "CYS/common.h"
#include "cc3xx_error.h"

#define NRF_CRYPTOCELL  (NRF_CRYPTOCELL_S)

CYS_error_t tee_map_error_values(cc3xx_err_t error);

#endif /* TEE_CRYPTO_COMMON_H */
