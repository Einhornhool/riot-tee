#include "cc310_registers.h"
#include "cc310_hash_sha.h"
#include "CYS/unprotected.h"

static const uint32_t SHA256_INIT[8] = {
    0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
    0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};

typedef struct {
    /** global state */
    uint32_t state[8];
    /** processed bits counter */
    uint32_t count[2];
    /** data buffer */
    unsigned char buf[64];
} sha2xx_context_t;

CYS_error_t cc310_hash_sha256_init(CYS_hash_sha256_ctx_t *ctx)
{
    sha2xx_context_t *sha256_ctx = (sha2xx_context_t *) ctx;

    sha256_ctx->state[0] = SHA256_INIT[0];
    sha256_ctx->state[1] = SHA256_INIT[1];
    sha256_ctx->state[2] = SHA256_INIT[2];
    sha256_ctx->state[3] = SHA256_INIT[3];
    sha256_ctx->state[4] = SHA256_INIT[4];
    sha256_ctx->state[5] = SHA256_INIT[5];
    sha256_ctx->state[6] = SHA256_INIT[6];
    sha256_ctx->state[7] = SHA256_INIT[7];

    return CYS_SUCCESS;
}

CYS_error_t cc310_hash_sha256_update(CYS_hash_sha256_ctx_t *ctx, uint8_t *input, size_t input_len)
{
    sha2xx_context_t *sha256_ctx = (sha2xx_context_t *) ctx;

    if (input_len > 32) {
        return CYS_ERROR_NOT_SUPPORTED;
    }

    /* Enable CRYPTOCELL subsystem */
    NRF_CRYPTOCELL->ENABLE = CRYPTOCELL_ENABLE_ENABLE_Enabled;

    /* Enable engine and DMA clock */
    NRF_CC_MISC->HASH_CLK = CC_MISC_HASH_CLK_ENABLE_Enable;
    NRF_CC_MISC->DMA_CLK = CC_MISC_DMA_CLK_ENABLE_Enable;

    /* Wait until hash engine is Idle  */
    while (NRF_CC_CTL->HASH_BUSY == CC_CTL_HASH_BUSY_STATUS_Busy) {}

    /* Clear all interrupts */
    NRF_CC_HOST_RGF->ICR = 0xFFFFFFFF;

    /* Configure HASH as cryptographic flow */
    NRF_CC_CTL->CRYPTO_CTL = CC_CTL_CRYPTO_CTL_MODE_HashActive;

    /* Configure engine for SHA256 */
    NRF_CC_HASH->HASH_CONTROL = CC_HASH_HASH_CONTROL_MODE_SHA256;

    /* Configure initial SHA256 values */
    NRF_CC_HASH->HASH_H[7] = sha256_ctx->state[7];
    NRF_CC_HASH->HASH_H[6] = sha256_ctx->state[6];
    NRF_CC_HASH->HASH_H[5] = sha256_ctx->state[5];
    NRF_CC_HASH->HASH_H[4] = sha256_ctx->state[4];
    NRF_CC_HASH->HASH_H[3] = sha256_ctx->state[3];
    NRF_CC_HASH->HASH_H[2] = sha256_ctx->state[2];
    NRF_CC_HASH->HASH_H[1] = sha256_ctx->state[1];
    NRF_CC_HASH->HASH_H[0] = sha256_ctx->state[0];

    /* Configure DMA input source address to start the cryptographic operation */
    NRF_CC_DIN->SRC_MEM_ADDR = (uint32_t) input;
    NRF_CC_DIN->SRC_MEM_SIZE = (uint32_t) input_len;

    /* Wait on DIN DMA interrupt indicating data has been fetched */
    while(!(NRF_CC_HOST_RGF->IRR & CC_HOST_RGF_IRR_MEM_TO_DIN_INT_Msk)) {}

    /* Wait until hash engine is Idle */
    while (NRF_CC_CTL->HASH_BUSY == CC_CTL_HASH_BUSY_STATUS_Busy) {}

    sha256_ctx->state[0] = NRF_CC_HASH->HASH_H[0];
    sha256_ctx->state[1] = NRF_CC_HASH->HASH_H[1];
    sha256_ctx->state[2] = NRF_CC_HASH->HASH_H[2];
    sha256_ctx->state[3] = NRF_CC_HASH->HASH_H[3];
    sha256_ctx->state[4] = NRF_CC_HASH->HASH_H[4];
    sha256_ctx->state[5] = NRF_CC_HASH->HASH_H[5];
    sha256_ctx->state[6] = NRF_CC_HASH->HASH_H[6];
    sha256_ctx->state[7] = NRF_CC_HASH->HASH_H[7];

    /* Disable CRYPTOCELL subsystem */
    NRF_CRYPTOCELL->ENABLE = ~CRYPTOCELL_ENABLE_ENABLE_Enabled;

    return CYS_SUCCESS;
}

CYS_error_t cc310_hash_sha256_finish(CYS_hash_sha256_ctx_t *ctx, uint8_t *digest, size_t digest_len)
{
    sha2xx_context_t *sha256_ctx = (sha2xx_context_t *) ctx;
    uint32_t *digest_ptr = (uint32_t *) digest;

    digest_ptr[0] = sha256_ctx->state[0];
    digest_ptr[1] = sha256_ctx->state[1];
    digest_ptr[2] = sha256_ctx->state[2];
    digest_ptr[3] = sha256_ctx->state[3];
    digest_ptr[4] = sha256_ctx->state[4];
    digest_ptr[5] = sha256_ctx->state[5];
    digest_ptr[6] = sha256_ctx->state[6];
    digest_ptr[7] = sha256_ctx->state[7];

    return CYS_SUCCESS;
}
