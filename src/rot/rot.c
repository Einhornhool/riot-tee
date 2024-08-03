#include "nrf9160.h"
#include "nrf9120_bitfields.h"
#include "rot.h"
#include "random.h"
#include "ocrypto_aes_ecb.h"

#define ROT_AES_128_KEY_BYTES   (16)
#define ROT_KEY_AES_ID          (3)
#define ROT_KEY_P256_PRIVATE_ID (4)

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
#define ROT_KEY_AES_PERM           (0xFFFFFFFA)

tee_status_t rot_try_generate_aes_key(void)
{
    /* Select key slot 3 */
    NRF_KMU_S->SELECTKEYSLOT = ROT_KEY_AES_ID;

    /* Index N in UICR->KEYSLOT.KEY[N] and UICR->KEYSLOT.CONFIG[N]
    corresponds to KMU key slot ID=N+1. */
    if (NRF_UICR_S->KEYSLOT.CONFIG[ROT_KEY_AES_ID-1].PERM != 0xFFFFFFFF) {
        /* If KEYSLOT.CONFIG != 0xFFFFFFFF, this means this slot has already been configured */
        return TEE_ERROR_ALREADY_EXISTS;
    }

    /* Enable writes to flash */
    NRF_NVMC_S->CONFIG = NVMC_CONFIG_WEN_Wen;

    /* Generate AES key */
    uint32_t aes_key[ROT_AES_128_KEY_BYTES/4];
    random_bytes((uint8_t *)aes_key, ROT_AES_128_KEY_BYTES);

    /* Write AES key to hardware key slot */
    NRF_UICR_S->KEYSLOT.KEY[ROT_KEY_AES_ID-1].VALUE[0] = aes_key[0];
    NRF_UICR_S->KEYSLOT.KEY[ROT_KEY_AES_ID-1].VALUE[1] = aes_key[1];
    NRF_UICR_S->KEYSLOT.KEY[ROT_KEY_AES_ID-1].VALUE[2] = aes_key[2];
    NRF_UICR_S->KEYSLOT.KEY[ROT_KEY_AES_ID-1].VALUE[3] = aes_key[3];

    /* Configure key permissions */
    NRF_UICR_S->KEYSLOT.CONFIG[ROT_KEY_AES_ID-1].PERM = ROT_KEY_AES_PERM;

    /* Disable writes to flash */
    NRF_NVMC_S->CONFIG = NVMC_CONFIG_WEN_Ren;

    /* Deselect key slot 3 */
    NRF_KMU_S->SELECTKEYSLOT = 0;

    return TEE_SUCCESS;
}

tee_status_t rot_encrypt_key(uint8_t *key_in, size_t key_in_size, uint8_t *cipher_out, size_t cipher_out_size)
{
    /* Select key slot 3 */
    NRF_KMU_S->SELECTKEYSLOT = ROT_KEY_AES_ID;

    /* Check if permissions are set correctly */
    if (NRF_UICR_S->KEYSLOT.CONFIG[ROT_KEY_AES_ID-1].PERM != ROT_KEY_AES_PERM) {
        return TEE_ERROR_NOT_PERMITTED;
    }

    ocrypto_aes_ecb_encrypt(cipher_out, key_in, key_in_size, (uint8_t *)NRF_UICR_S->KEYSLOT.KEY[ROT_KEY_AES_ID-1].VALUE, ROT_AES_128_KEY_BYTES);

    /* Deselect key slot 3 */
    NRF_KMU_S->SELECTKEYSLOT = 0;

    return TEE_SUCCESS;
}

tee_status_t rot_decrypt_key(uint8_t *key_in, size_t key_in_size, uint8_t *key_out, size_t key_out_size)
{
    /* Select key slot 3 */
    NRF_KMU_S->SELECTKEYSLOT = ROT_KEY_AES_ID;

    /* Check if permissions are set correctly */
    if (NRF_UICR_S->KEYSLOT.CONFIG[ROT_KEY_AES_ID-1].PERM != ROT_KEY_AES_PERM) {
        return TEE_ERROR_NOT_PERMITTED;
    }

    ocrypto_aes_ecb_decrypt(key_out, key_in, key_in_size, (uint8_t *)NRF_UICR_S->KEYSLOT.KEY[ROT_KEY_AES_ID-1].VALUE, ROT_AES_128_KEY_BYTES);

    /* Deselect key slot 3 */
    NRF_KMU_S->SELECTKEYSLOT = 0;

    return TEE_SUCCESS;
}

tee_status_t rot_sign(io_pack_t *in, io_pack_t *out)
{
    (void) in;
    (void) out;
    return TEE_ERROR_NOT_SUPPORTED;
}

tee_status_t rot_export_public_key(io_pack_t *in, io_pack_t *out)
{
    (void) in;
    (void) out;
    return TEE_ERROR_NOT_SUPPORTED;
}
