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
    (void) context;
    (void) key;
    (void) keySize;

    return CIPHER_INIT_SUCCESS;
}

/**
 * @warning This is a rudimentary driver for a PoC and probably not implemented correctly.
 *          Don't rely on this for anything security critical.
 */
static CYS_error_t cc310_aes_128_encrypt_decrypt_with_root_key(uint8_t *input, size_t input_len,
                uint8_t *output, size_t output_len, int mode)
{
    /* Enable CRYPTOCELL subsystem */
    TEE_CRYPTOCELL->ENABLE =  CRYPTOCELL_ENABLE_ENABLE_Enabled;

    /* Enable engine and DMA clock */
    TEE_CC_MISC->AES_CLK = TEE_CC_MISC_AES_CLK_ENABLE_Enable;
    TEE_CC_MISC->DMA_CLK = TEE_CC_MISC_DMA_CLK_ENABLE_Enable;

    TEE_CC_AES->AES_CONTROL = mode;

    /* Wait until crypto engine is Idle  */
    while (TEE_CC_CTL->CRYPTO_BUSY == TEE_CC_CTL_CRYPTO_BUSY_STATUS_Busy) { }

    /* Select HW Key */
    TEE_CC_HOST_RGF->HOST_CRYPTOKEY_SEL = 0;
    TEE_CC_AES->AES_SK = 1;

    /* Configure AES as cryptographic flow */
    TEE_CC_CTL->CRYPTO_CTL = TEE_CC_CTL_CRYPTO_CTL_MODE_AESActive;

    /* Configure DMA output destination address */
    TEE_CC_DOUT->DST_MEM_ADDR = (uint32_t) output;
    TEE_CC_DOUT->DST_MEM_SIZE = (uint32_t) output_len;

    /* Configure DMA input source address to start the cryptographic operation */
    TEE_CC_DIN->SRC_MEM_ADDR = (uint32_t) input;
    TEE_CC_DIN->SRC_MEM_SIZE = (uint32_t) input_len;

    /* Wait on DOUT DMA interrupt */
    while(!(TEE_CC_HOST_RGF->IRR & TEE_CC_HOST_RGF_IRR_DOUT_TO_MEM_INT_Msk)) { }

    TEE_CC_DOUT->DOUT_SW_RESET = 1;
    TEE_CC_AES->AES_SW_RESET = 1;
    TEE_CRYPTOCELL->ENABLE = ~CRYPTOCELL_ENABLE_ENABLE_Enabled;

    return CYS_SUCCESS;
}

int cc310_aes_encrypt_block(const cipher_context_t *context, const uint8_t *plain_block,
                uint8_t *cipher_block)
{
    (void) context;
    CYS_error_t ret = cc310_aes_128_encrypt_decrypt_with_root_key((uint8_t *)plain_block, AES_BLOCK_SIZE, cipher_block, AES_BLOCK_SIZE, TEE_CC_AES_ECB_ENCRYPT);
    if (ret != CYS_SUCCESS) {
        return CIPHER_ERR_ENC_FAILED;
    }

    return 1;
}

int cc310_aes_decrypt_block(const cipher_context_t *context, const uint8_t *cipher_block,
                uint8_t *plain_block)
{
    (void) context;
    CYS_error_t ret = cc310_aes_128_encrypt_decrypt_with_root_key((uint8_t *)cipher_block, AES_BLOCK_SIZE, plain_block, AES_BLOCK_SIZE, TEE_CC_AES_ECB_DECRYPT);

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
