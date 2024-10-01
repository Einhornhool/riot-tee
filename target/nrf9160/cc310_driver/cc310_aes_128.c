#include "cc310_registers.h"
#include "cc310_aes_128.h"

CYS_error_t cc310_aes_128_cbc_encrypt(uint32_t *key, uint32_t *iv, uint8_t *plain, size_t plain_len, uint8_t *cipher, size_t cipher_len)
{
    /* Enable CRYPTOCELL subsystem */
    NRF_CRYPTOCELL->ENABLE =  CRYPTOCELL_ENABLE_ENABLE_Enabled;

    /* Enable engine and DMA clock */
    NRF_CC_MISC->AES_CLK = CC_MISC_AES_CLK_ENABLE_Enable;
    NRF_CC_MISC->DMA_CLK = CC_MISC_DMA_CLK_ENABLE_Enable;

    /* Wait until crypto engine is Idle  */
    while (NRF_CC_CTL->CRYPTO_BUSY == CC_CTL_CRYPTO_BUSY_STATUS_Busy) { }

    /* Configure AES as cryptographic flow */
    NRF_CC_CTL->CRYPTO_CTL = CC_CTL_CRYPTO_CTL_MODE_AESActive;

    NRF_CC_AES->AES_CONTROL = CC_AES_CBC_ENCRYPT;

    /* Load the AES key value into the engine */
    NRF_CC_AES->AES_KEY_0[0] = key[0];
    NRF_CC_AES->AES_KEY_0[1] = key[1];
    NRF_CC_AES->AES_KEY_0[2] = key[2];
    NRF_CC_AES->AES_KEY_0[3] = key[3];

    /* Configure default init vector */
    NRF_CC_AES->AES_IV_0[0] = iv[0];
    NRF_CC_AES->AES_IV_0[1] = iv[1];
    NRF_CC_AES->AES_IV_0[2] = iv[2];
    NRF_CC_AES->AES_IV_0[3] = iv[3];

    /* Configure DMA output destination address */
    NRF_CC_DOUT->DST_MEM_ADDR = (uint32_t) cipher;
    NRF_CC_DOUT->DST_MEM_SIZE = (uint32_t) cipher_len;

    /* Configure DMA input source address to start the cryptographic operation */
    NRF_CC_DIN->SRC_MEM_ADDR = (uint32_t) plain;
    NRF_CC_DIN->SRC_MEM_SIZE = (uint32_t) plain_len;

    // /* Wait on DOUT DMA interrupt */
    while(!(NRF_CC_HOST_RGF->IRR & CC_HOST_RGF_IRR_DOUT_TO_MEM_INT_Msk)) { }

    NRF_CRYPTOCELL->ENABLE = ~CRYPTOCELL_ENABLE_ENABLE_Enabled;

    return CYS_SUCCESS;
}

CYS_error_t cc310_aes_128_cbc_decrypt(uint32_t *key, uint32_t *iv, uint8_t *cipher, size_t cipher_len, uint8_t *plain, size_t plain_len)
{
    /* Enable CRYPTOCELL subsystem */
    NRF_CRYPTOCELL->ENABLE =  CRYPTOCELL_ENABLE_ENABLE_Enabled;

    /* Enable engine and DMA clock */
    NRF_CC_MISC->AES_CLK = CC_MISC_AES_CLK_ENABLE_Enable;
    NRF_CC_MISC->DMA_CLK = CC_MISC_DMA_CLK_ENABLE_Enable;

    NRF_CC_DOUT->DOUT_SW_RESET = 1;

    /* Wait until crypto engine is Idle  */
    while (NRF_CC_CTL->CRYPTO_BUSY == CC_CTL_CRYPTO_BUSY_STATUS_Busy) { }

    /* Configure AES as cryptographic flow */
    NRF_CC_CTL->CRYPTO_CTL = CC_CTL_CRYPTO_CTL_MODE_AESActive;

    NRF_CC_AES->AES_CONTROL = CC_AES_CBC_DECRYPT;

    /* Load the AES key value into the engine */
    NRF_CC_AES->AES_KEY_0[0] = key[0];
    NRF_CC_AES->AES_KEY_0[1] = key[1];
    NRF_CC_AES->AES_KEY_0[2] = key[2];
    NRF_CC_AES->AES_KEY_0[3] = key[3];

    /* Configure default init vector */
    NRF_CC_AES->AES_IV_0[0] = iv[0];
    NRF_CC_AES->AES_IV_0[1] = iv[1];
    NRF_CC_AES->AES_IV_0[2] = iv[2];
    NRF_CC_AES->AES_IV_0[3] = iv[3];

    /* Configure DMA output destination address */
    NRF_CC_DOUT->DST_MEM_ADDR = (uint32_t) plain;
    NRF_CC_DOUT->DST_MEM_SIZE = (uint32_t) plain_len;

    /* Configure DMA input source address to start the cryptographic operation */
    NRF_CC_DIN->SRC_MEM_ADDR = (uint32_t) cipher;
    NRF_CC_DIN->SRC_MEM_SIZE = (uint32_t) cipher_len;

    // /* Wait on DOUT DMA interrupt */
    while(!(NRF_CC_HOST_RGF->IRR & CC_HOST_RGF_IRR_DOUT_TO_MEM_INT_Msk)) { }

    NRF_CRYPTOCELL->ENABLE = ~CRYPTOCELL_ENABLE_ENABLE_Enabled;

    return CYS_SUCCESS;
}
