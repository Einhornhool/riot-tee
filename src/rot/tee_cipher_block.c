#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "CYS/common.h"
#include "crypto/ciphers.h"
#include "cc310_driver/cc310_aes_128.h"
#include "cc310_driver/cc310_registers.h"

#define AES_BLOCK_SIZE      16
#define AES_KEY_SIZE_128    16

int cc310_aes_init(cipher_context_t *context, const uint8_t *key, uint8_t keySize)
{
    uint8_t i;

    if (keySize != AES_KEY_SIZE_128) {
        return CIPHER_ERR_INVALID_KEY_SIZE;
    }

    context->key_size = keySize;

    /* Make sure that context is large enough. If this is not the case,
       you should build with -DAES */
    if (CIPHER_MAX_CONTEXT_SIZE < keySize) {
        return CIPHER_ERR_BAD_CONTEXT_SIZE;
    }

    /* key must be at least CIPHERS_MAX_KEY_SIZE Bytes long */
    if (keySize < CIPHERS_MAX_KEY_SIZE) {
        /* fill up by concatenating key to as long as needed */
        for (i = 0; i < CIPHERS_MAX_KEY_SIZE; i++) {
            context->context[i] = key[(i % keySize)];
        }
    }
    else {
        for (i = 0; i < CIPHERS_MAX_KEY_SIZE; i++) {
            context->context[i] = key[i];
        }
    }

    return CIPHER_INIT_SUCCESS;
}

int cc310_aes_encrypt_block(const cipher_context_t *context, const uint8_t *plain_block,
                uint8_t *cipher_block)
{
    CYS_error_t ret = cc310_aes_128_encrypt_decrypt((uint32_t *)context->context, NULL, (uint8_t *)plain_block, AES_BLOCK_SIZE, cipher_block, AES_BLOCK_SIZE, TEE_CC_AES_ECB_ENCRYPT);

    if (ret != CYS_SUCCESS) {
        return CIPHER_ERR_ENC_FAILED;
    }

    return 1;
}

int cc310_aes_decrypt_block(const cipher_context_t *context, const uint8_t *cipher_block,
                uint8_t *plain_block)
{
    CYS_error_t ret = cc310_aes_128_encrypt_decrypt((uint32_t *)context->context, NULL, (uint8_t *)cipher_block, AES_BLOCK_SIZE, plain_block, AES_BLOCK_SIZE, TEE_CC_AES_ECB_DECRYPT);

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
