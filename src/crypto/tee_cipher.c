#include <arm_cmse.h>
#include "tee_io_sanitizer.h"
#include "tee_cipher.h"
#include "CYS/unprotected.h"

CYS_error_t tee_cipher_aes_128_ecb_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    (void)in;
    (void)in_len;
    (void)out;
    (void)out_len;

    return CYS_ERROR_NOT_SUPPORTED;
}

CYS_error_t tee_cipher_aes_128_ecb_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    (void)in;
    (void)in_len;
    (void)out;
    (void)out_len;

    return CYS_ERROR_NOT_SUPPORTED;
}

CYS_error_t tee_cipher_aes_128_cbc_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *iv = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range((void *)in[2].data, in[2].len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (plain == NULL || cipher == NULL || key == NULL || iv == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t plain_len = in[2].len;

    return CYS_aes_128_cbc_encrypt(key, iv, plain, plain_len, cipher);
}

CYS_error_t tee_cipher_aes_128_cbc_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *iv = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range((void *)in[2].data, in[2].len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (cipher == NULL || plain == NULL || key == NULL || iv == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t cipher_len = in[2].len;

    return CYS_aes_128_cbc_decrypt(key, iv, cipher, cipher_len, plain);
}
