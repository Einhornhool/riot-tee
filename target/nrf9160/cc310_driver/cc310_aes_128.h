#ifndef CC310_AES_128_H
#define CC310_AES_128_H

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"

/**
 * @brief   Encrypt or decrypt data using the Cryptocell AES Engine
 *
 * @warning This is a rudimentary driver for a PoC and probably not implemented correctly.
 *          Don't rely on this for anything security critical.
 *
 * @param key
 * @param input
 * @param input_len
 * @param output
 * @param output_len
 * @param mode
 * @return CYS_error_t
 */
CYS_error_t cc310_aes_128_encrypt_decrypt(uint32_t *key, uint32_t *iv, uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int mode);

#endif /* CC310_AES_128_H */
