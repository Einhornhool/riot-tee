#include <arm_cmse.h>
#include "tee_crypto_common.h"
#include "tee_io_sanitizer.h"
#include "tee_cipher.h"
#include "string.h"

CYS_error_t tee_internal_aes_encrypt_decrypt(cc3xx_aes_direction_t dir, cc3xx_aes_mode_t mode,
                                        cc3xx_aes_keysize_t keysize, cc3xx_aes_key_id_t key_id, const uint32_t *key, const uint32_t *iv, size_t iv_len, uint32_t *tag, uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, size_t *output_bytes) {
    NRF_CRYPTOCELL->ENABLE = 1;
    cc3xx_err_t status = cc3xx_lowlevel_aes_init(dir, mode, key_id, key, keysize, iv, iv_len);
    if (status != CC3XX_ERR_SUCCESS) {
        goto exit;
    }

    cc3xx_lowlevel_aes_set_output_buffer(output, output_len);

    status = cc3xx_lowlevel_aes_update(input, input_len);
    if (status != CC3XX_ERR_SUCCESS) {
        goto exit;
    }

    status = cc3xx_lowlevel_aes_finish(tag, output_bytes);
    if (status != CC3XX_ERR_SUCCESS) {
        goto exit;
    }

exit:
    cc3xx_lowlevel_aes_uninit();
    NRF_CRYPTOCELL->ENABLE = 0;
    return tee_map_error_values(status);
}

CYS_error_t tee_cipher_aes_128_ecb_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    size_t plain_len = in[1].len;
    size_t cipher_len = out[0].len;
    size_t output_bytes = 0;

    uint32_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range((void *)in[1].data, plain_len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range(out[0].data, cipher_len, CMSE_NONSECURE);

    if (plain == NULL || cipher == NULL || key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    return tee_internal_aes_encrypt_decrypt(CC3XX_AES_DIRECTION_ENCRYPT, CC3XX_AES_MODE_ECB, CC3XX_AES_KEYSIZE_128, CC3XX_AES_KEY_ID_USER_KEY, key, NULL, 0, NULL, plain, plain_len, cipher, cipher_len, &output_bytes);
}

CYS_error_t tee_cipher_aes_128_ecb_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    size_t cipher_len = in[1].len;
    size_t plain_len = out[0].len;
    size_t output_bytes = 0;

    uint32_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range((void *)in[1].data, cipher_len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range(out[0].data, plain_len, CMSE_NONSECURE);

    if (cipher == NULL || plain == NULL || key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    return tee_internal_aes_encrypt_decrypt(CC3XX_AES_DIRECTION_DECRYPT, CC3XX_AES_MODE_ECB, CC3XX_AES_KEYSIZE_128, CC3XX_AES_KEY_ID_USER_KEY, key, NULL, 0, NULL, cipher, cipher_len, plain, plain_len, &output_bytes);
}

CYS_error_t tee_cipher_aes_128_cbc_encrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    size_t iv_len = in[1].len;
    size_t plain_len = in[2].len;
    size_t cipher_len = out[0].len;
    size_t output_bytes = 0;

    uint32_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint32_t *iv = cmse_check_address_range((void *)in[1].data, iv_len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range((void *)in[2].data, plain_len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range(out[0].data, cipher_len, CMSE_NONSECURE);

    if (plain == NULL || cipher == NULL || key == NULL || iv == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    return tee_internal_aes_encrypt_decrypt(CC3XX_AES_DIRECTION_ENCRYPT, CC3XX_AES_MODE_CBC, CC3XX_AES_KEYSIZE_128, CC3XX_AES_KEY_ID_USER_KEY, key, iv, iv_len, NULL, plain, plain_len, cipher, cipher_len, &output_bytes);
}

CYS_error_t tee_cipher_aes_128_cbc_decrypt(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }
    
        size_t iv_len = in[1].len;
        size_t cipher_len = in[2].len;
        size_t plain_len = out[0].len;
        size_t output_bytes = 0;

    uint32_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint32_t *iv = cmse_check_address_range((void *)in[1].data, iv_len, CMSE_NONSECURE);
    uint8_t *cipher = cmse_check_address_range((void *)in[2].data, cipher_len, CMSE_NONSECURE);
    uint8_t *plain = cmse_check_address_range(out[0].data, plain_len, CMSE_NONSECURE);

    if (cipher == NULL || plain == NULL || key == NULL || iv == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    return tee_internal_aes_encrypt_decrypt(CC3XX_AES_DIRECTION_DECRYPT, CC3XX_AES_MODE_CBC, CC3XX_AES_KEYSIZE_128, CC3XX_AES_KEY_ID_USER_KEY, key, iv, iv_len, NULL, cipher, cipher_len, plain, plain_len, &output_bytes);
}
