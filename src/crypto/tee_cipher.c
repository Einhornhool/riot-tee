#include <arm_cmse.h>
#include "cc310_driver/cc310_aes_128.h"
#include "cc310_driver/cc310_registers.h"
#include "tee_io_sanitizer.h"
#include "tee_cipher.h"
#include "string.h"

CYS_error_t tee_cipher_aes_128_ecb_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (plain == NULL || cipher == NULL || key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t plain_len = in[1].len;
    size_t cipher_len = out[0].len;

    return cc310_aes_128_encrypt_decrypt((uint32_t *)key, NULL, plain, plain_len, cipher, cipher_len, TEE_CC_AES_ECB_ENCRYPT);
}

CYS_error_t tee_cipher_aes_128_ecb_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (cipher == NULL || plain == NULL || key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t cipher_len = in[1].len;
    size_t plain_len = out[0].len;

    return cc310_aes_128_encrypt_decrypt((uint32_t *)key, NULL, cipher, cipher_len, plain, plain_len, TEE_CC_AES_ECB_DECRYPT);
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
    size_t cipher_len = out[0].len;

    return cc310_aes_128_encrypt_decrypt((uint32_t *)key, (uint32_t *)iv, plain, plain_len, cipher, cipher_len, TEE_CC_AES_CBC_ENCRYPT);
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
    size_t plain_len = out[0].len;

    return cc310_aes_128_encrypt_decrypt((uint32_t *)key, (uint32_t *)iv, cipher, cipher_len, plain, plain_len, TEE_CC_AES_CBC_DECRYPT);
}
