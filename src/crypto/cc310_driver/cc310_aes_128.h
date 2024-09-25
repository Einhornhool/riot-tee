#ifndef CC310_AES_128_H
#define CC310_AES_128_H

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"

CYS_error_t cc310_aes_128_cbc_encrypt(uint32_t *key, uint32_t *iv, uint8_t *plain, size_t plain_len, uint8_t *cipher, size_t cipher_len);

CYS_error_t cc310_aes_128_cbc_decrypt(uint32_t *key, uint32_t *iv, uint8_t *cipher, size_t cipher_len, uint8_t *plain, size_t plain_len);

#endif /* CC310_AES_128_H */
