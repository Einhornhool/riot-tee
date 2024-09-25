#ifndef TEE_CIPHER_H
#define TEE_CIPHER_H

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"
#include "tee_secure_io.h"

CYS_error_t tee_cipher_aes_128_ecb_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_cipher_aes_128_ecb_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_cipher_aes_128_cbc_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

CYS_error_t tee_cipher_aes_128_cbc_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

#endif /* TEE_CIPHER_H */
