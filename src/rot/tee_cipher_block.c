#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "CYS/common.h"
#include "crypto/ciphers.h"
#include "tee_cipher.h"

#define AES_BLOCK_SIZE      16
#define AES_KEY_SIZE_128    16

#define CC3XX_AES_KEY_ID_KDR    0

int cc310_aes_init(cipher_context_t *context, const uint8_t *key, uint8_t keySize)
{
    (void) context;
    (void) key;
    (void) keySize;

    return CIPHER_INIT_SUCCESS;
}

int cc310_aes_encrypt_block(const cipher_context_t *context, const uint8_t *plain_block,
                uint8_t *cipher_block)
{
    (void) context;
    // CYS_error_t ret = cc310_aes_128_encrypt_decrypt_with_root_key((uint8_t *)plain_block, AES_BLOCK_SIZE, cipher_block, AES_BLOCK_SIZE, TEE_CC_AES_ECB_ENCRYPT);
    size_t output_bytes = 0;
    CYS_error_t ret = tee_internal_aes_encrypt_decrypt(CC3XX_AES_DIRECTION_ENCRYPT, CC3XX_AES_MODE_ECB, CC3XX_AES_KEYSIZE_128, CC3XX_AES_KEY_ID_KDR, NULL, NULL, 0, NULL, plain_block, AES_BLOCK_SIZE, cipher_block, AES_BLOCK_SIZE, &output_bytes);
    if (ret != CYS_SUCCESS) {
        return CIPHER_ERR_ENC_FAILED;
    }

    return 1;
}

int cc310_aes_decrypt_block(const cipher_context_t *context, const uint8_t *cipher_block,
                uint8_t *plain_block)
{
    (void) context;
    size_t output_bytes = 0;
    CYS_error_t ret = tee_internal_aes_encrypt_decrypt(CC3XX_AES_DIRECTION_DECRYPT, CC3XX_AES_MODE_ECB, CC3XX_AES_KEYSIZE_128, CC3XX_AES_KEY_ID_KDR, NULL, NULL, 0, NULL, cipher_block, AES_BLOCK_SIZE, plain_block, AES_BLOCK_SIZE, &output_bytes);

    if (ret != CYS_SUCCESS) {
        return CIPHER_ERR_DEC_FAILED;
    }

    return 1;
}

/**
 * Interface to the aes cipher
 */
static const cipher_interface_t cc310_aes_interface = {
    AES_BLOCK_SIZE,
    cc310_aes_init,
    cc310_aes_encrypt_block,
    cc310_aes_decrypt_block
};

const cipher_id_t CIPHER_AES = &cc310_aes_interface;
