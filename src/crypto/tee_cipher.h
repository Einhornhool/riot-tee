#ifndef TEE_CIPHER_H
#define TEE_CIPHER_H

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"
#include "cc3xx_aes.h"
#include "tee_secure_io.h"

#define TEE_CIPHER_AES_128_KEY_SIZE (16)

CYS_error_t tee_internal_aes_encrypt_decrypt(cc3xx_aes_direction_t dir, cc3xx_aes_mode_t mode,
                                        cc3xx_aes_keysize_t keysize, cc3xx_aes_key_id_t key_id, const uint32_t *key, const uint32_t *iv, size_t iv_len, uint32_t *tag, uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, size_t *output_bytes);

CYS_error_t tee_cipher_aes_128_ecb_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_cipher_aes_128_ecb_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_cipher_aes_128_cbc_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_cipher_aes_128_cbc_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

#endif /* TEE_CIPHER_H */
