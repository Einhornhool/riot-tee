#ifndef CC310_ENTROPY_H
#define CC310_ENTROPY_H

#include <stddef.h>
#include "CYS/common.h"
#include "cc310_registers.h"

#define NRF_CC_RNG_OUTPUT_LEN (24)

/**
 * @brief   Get entropy from the CC310 RNG
 *
 * @warning This is a rudimentary driver for a PoC and probably not implemented correctly.
 *          Don't rely on this for anything security critical.
 *
 * @param buf
 * @param len
 * @return CYS_error_t
 */
CYS_error_t cc310_rng_get_entropy(uint32_t *buf, size_t len);

#endif /* CC310_ENTROPY_H */
