#ifndef CC310_AES_128_H
#define CC310_AES_128_H

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"

CYS_error_t cc310_aes_128_encrypt_decrypt(uint32_t *key, uint32_t *iv, uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int mode);

#endif /* CC310_AES_128_H */
