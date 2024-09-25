#include "nrf9160.h"
#include "nrf9120_bitfields.h"

#include "ocb.h"
#include "random.h"
#include "CYS/common.h"
#include "CYS/sealed_key.h"
#include "cc310_driver/cc310_registers.h"

#include "tee_rot.h"
// #include "cc310_driver/cc310_aes_128.c"

#define TEE_ROT_KEY_AES_ID          (3)
#define TEE_ROT_AES_128_KEY_BYTES   (16)

static const AES_TEST_KEY[TEE_ROT_AES_128_KEY_BYTES] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};

/**
 * @brief   Permissions for AES key.
 *
 *          Bit 0 [Write]   : 0     Not allowed
 *          Bit 1 [Read]    : 1     Allowed
 *          Bit 2 [Push]    : 0     Disabled
 *          Bit 16[State]   : 1     Active
 *
 * @warning These are insecure, because the CPU can read the key from the UICR.
 *          This is a placeholder, until there is a Cryptocell driver implementation available.
 */
#define TEE_ROT_KEY_AES_PERM           (0xFFFFFFFA)

CYS_error_t tee_rot_try_generate_aes_key(void)
{
    /* Select key slot 3 */
    // NRF_KMU_S->SELECTKEYSLOT = TEE_ROT_KEY_AES_ID;

    /* Index N in UICR->KEYSLOT.KEY[N] and UICR->KEYSLOT.CONFIG[N]
    corresponds to KMU key slot ID=N+1. */
    // if (NRF_UICR_S->KEYSLOT.CONFIG[TEE_ROT_KEY_AES_ID-1].PERM != 0xFFFFFFFF) {
    //     /* If KEYSLOT.CONFIG != 0xFFFFFFFF, this means this slot has already been configured */
    //     NRF_KMU_S->SELECTKEYSLOT = 0;
    //     return CYS_ERROR_ALREADY_EXISTS;
    // }

    // /* Enable writes to flash */
    // NRF_NVMC_S->CONFIG = NVMC_CONFIG_WEN_Wen;

    /* Generate AES key */
    // uint32_t aes_key[TEE_ROT_AES_128_KEY_BYTES/4];
    // random_bytes((uint8_t *)aes_key, TEE_ROT_AES_128_KEY_BYTES);

    /* Write AES key to hardware key slot */
    // NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[0] = aes_key[0];
    // NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[1] = aes_key[1];
    // NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[2] = aes_key[2];
    // NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[3] = aes_key[3];
    if (TEE_CC_HOST_RGF->KDR0) {
        return CYS_ERROR_ALREADY_EXISTS;
    }

    TEE_CC_HOST_RGF->KDR0 = AES_TEST_KEY[0];
    TEE_CC_HOST_RGF->KDR1 = AES_TEST_KEY[1];
    TEE_CC_HOST_RGF->KDR2 = AES_TEST_KEY[2];
    TEE_CC_HOST_RGF->KDR3 = AES_TEST_KEY[3];

    /* Configure key permissions */
    // NRF_UICR_S->KEYSLOT.CONFIG[TEE_ROT_KEY_AES_ID-1].PERM = TEE_ROT_KEY_AES_PERM;

    /* Disable writes to flash */
    // NRF_NVMC_S->CONFIG = NVMC_CONFIG_WEN_Ren;

    /* Deselect key slot 3 */
    // NRF_KMU_S->SELECTKEYSLOT = 0;

    if (TEE_CC_HOST_RGF->KDR0) {
        return CYS_SUCCESS;
    }

    return CYS_ERROR_GENERIC_ERROR;
}

CYS_error_t tee_rot_encrypt_key_ocb(uint8_t *key_in, CYS_PROT_ecc_p256_key_t *sealed_key)
{
    /* Select key slot 3 */
    // NRF_KMU_S->SELECTKEYSLOT = TEE_ROT_KEY_AES_ID;

    /* Check if permissions are set correctly */
    // if (NRF_UICR_S->KEYSLOT.CONFIG[TEE_ROT_KEY_AES_ID-1].PERM != TEE_ROT_KEY_AES_PERM) {
    //     NRF_KMU_S->SELECTKEYSLOT = 0;
    //     return CYS_ERROR_GENERIC_ERROR;
    // }

    // /* This is unsecure and only temporary until cryptocell can be used directly */
    // uint32_t aes_key[TEE_ROT_AES_128_KEY_BYTES/4];
    // aes_key[0] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[0];
    // aes_key[1] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[1];
    // aes_key[2] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[2];
    // aes_key[3] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[3];

    cipher_t cipher;
    cipher_init(&cipher, CIPHER_AES, (uint8_t *)aes_key, TEE_ROT_AES_128_KEY_BYTES);

    int32_t result = cipher_encrypt_ocb(&cipher, NULL, 0, CYS_PROT_SEAL_TAG_SIZE, sealed_key->nonce, CYS_PROT_SEAL_NONCE_SIZE, key_in, CYS_PROT_ECC_P256_KEY_SIZE, sealed_key->private_key);

    /* Deselect key slot 3 */
    // NRF_KMU_S->SELECTKEYSLOT = 0;

    if(result == CYS_PROT_ECC_P256_KEY_SIZE + CYS_PROT_SEAL_TAG_SIZE) {
        return CYS_SUCCESS;
    }

    return CYS_ERROR_GENERIC_ERROR;
}

CYS_error_t tee_rot_decrypt_key_ocb(CYS_PROT_ecc_p256_key_t *sealed_key, uint8_t *key_out)
{
    // /* Select key slot 3 */
    // NRF_KMU_S->SELECTKEYSLOT = TEE_ROT_KEY_AES_ID;

    // /* Check if permissions are set correctly */
    // if (NRF_UICR_S->KEYSLOT.CONFIG[TEE_ROT_KEY_AES_ID-1].PERM != TEE_ROT_KEY_AES_PERM) {
    //     NRF_KMU_S->SELECTKEYSLOT = 0;
    //     return CYS_ERROR_NOT_PERMITTED;
    // }

    // /* This is unsecure and only temporary until cryptocell can be used directly */
    // uint32_t aes_key[TEE_ROT_AES_128_KEY_BYTES/4];
    // aes_key[0] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[0];
    // aes_key[1] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[1];
    // aes_key[2] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[2];
    // aes_key[3] = NRF_UICR_S->KEYSLOT.KEY[TEE_ROT_KEY_AES_ID-1].VALUE[3];

    cipher_t cipher;
    cipher_init(&cipher, CIPHER_AES, (uint8_t *)aes_key, TEE_ROT_AES_128_KEY_BYTES);

    int32_t result = cipher_decrypt_ocb(&cipher, NULL, 0, CYS_PROT_SEAL_TAG_SIZE, sealed_key->nonce, CYS_PROT_SEAL_NONCE_SIZE, sealed_key->private_key, CYS_PROT_ECC_P256_KEY_SIZE+CYS_PROT_SEAL_TAG_SIZE, key_out);

    /* Deselect key slot 3 */
    // NRF_KMU_S->SELECTKEYSLOT = 0;

    return CYS_SUCCESS;
}

