#include "cc310_registers.h"
#include "cc310_aes_128.h"

#define AES_128_KEY_LEN     (16)

CYS_error_t cc310_aes_128_encrypt_decrypt(uint32_t *key, uint32_t *iv, uint8_t *input, size_t input_len, uint8_t *output, size_t output_len, int mode)
{
    /* Enable CRYPTOCELL subsystem */
    TEE_CRYPTOCELL->ENABLE =  CRYPTOCELL_ENABLE_ENABLE_Enabled;

    /* Enable engine and DMA clock */
    TEE_CC_MISC->AES_CLK = TEE_CC_MISC_AES_CLK_ENABLE_Enable;
    TEE_CC_MISC->DMA_CLK = TEE_CC_MISC_DMA_CLK_ENABLE_Enable;

    TEE_CC_DOUT->DOUT_SW_RESET = 1;

    /* Wait until crypto engine is Idle  */
    while (TEE_CC_CTL->CRYPTO_BUSY == TEE_CC_CTL_CRYPTO_BUSY_STATUS_Busy) { }

    /* Configure AES as cryptographic flow */
    TEE_CC_CTL->CRYPTO_CTL = TEE_CC_CTL_CRYPTO_CTL_MODE_AESActive;

    TEE_CC_AES->AES_CONTROL = mode;

    /* Load the AES key value into the engine */
    for (int i = 0; i < 4; i++) {
        TEE_CC_AES->AES_KEY_0[i] = key[i];
    }

    if (iv) {
        for (int i = 0; i < 4; i++) {
            TEE_CC_AES->AES_IV_0[i] = iv[i];
        }
    }
    else {
        /* If IV == NULL, set IV to 0 */
        for (int i = 0; i < 4; i++) {
            TEE_CC_AES->AES_IV_0[i] = 0x00;
        }
    }

    /* Configure DMA output destination address */
    TEE_CC_DOUT->DST_MEM_ADDR = (uint32_t) output;
    TEE_CC_DOUT->DST_MEM_SIZE = (uint32_t) output_len;

    /* Configure DMA input source address to start the cryptographic operation */
    TEE_CC_DIN->SRC_MEM_ADDR = (uint32_t) input;
    TEE_CC_DIN->SRC_MEM_SIZE = (uint32_t) input_len;

    /* Wait on DOUT DMA interrupt */
    while(!(TEE_CC_HOST_RGF->IRR & TEE_CC_HOST_RGF_IRR_DOUT_TO_MEM_INT_Msk)) { }

    TEE_CRYPTOCELL->ENABLE = ~CRYPTOCELL_ENABLE_ENABLE_Enabled;

    return CYS_SUCCESS;
}
