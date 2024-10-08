#include <string.h>
#include "random.h"
#include "CYS/common.h"
#include "CYS/sealed_key.h"

#include "tee_rot.h"
#include "tee_random.h"
#include "tee_ecc.h"
#include "tee_io_sanitizer.h"
#include "tee_crypto_common.h"

#include "cc3xx_ecdsa.h"

#define MAX_REPETITION                  (10)
#define TEE_ECC_P256_PRIV_KEY_SIZE      (32)
#define TEE_ECC_P256_PUB_KEY_SIZE       (65)
#define TEE_ECC_P256_SESSION_KEY_SIZE   (32)
#define TEE_ECC_P256_SIGNATURE_SIZE     (64)
#define TEE_ECC_P256_HASH_SIZE          (32)


static CYS_error_t tee_internal_ecc_genkey(cc3xx_ec_curve_id_t curve_id, uint8_t *privkey, size_t privkey_size)
{
    size_t priv_key_len = 0;
    NRF_CRYPTOCELL->ENABLE = 1;
    cc3xx_err_t err = cc3xx_lowlevel_ecdsa_genkey(curve_id, (uint32_t *)privkey, privkey_size, &priv_key_len);
    if (err != CC3XX_ERR_SUCCESS) {
        return tee_map_error_values(err);
    }
    NRF_CRYPTOCELL->ENABLE = 0;
    return CYS_SUCCESS;
}


static CYS_error_t tee_internal_ecc_derive(cc3xx_ec_curve_id_t curve_id, uint8_t *privkey, size_t privkey_size, uint8_t *output, size_t output_size)
{
    cc3xx_err_t err;

    size_t modulus_size = cc3xx_lowlevel_ec_get_modulus_size_from_curve(curve_id);

    uint32_t pubkey_x[modulus_size / sizeof(uint32_t)];
    uint32_t pubkey_y[modulus_size / sizeof(uint32_t)];
    size_t pubkey_x_size, pubkey_y_size;

    NRF_CRYPTOCELL->ENABLE = 1;
    err = cc3xx_lowlevel_ecdsa_getpub(curve_id, (uint32_t *)privkey, privkey_size, pubkey_x, sizeof(pubkey_x), &pubkey_x_size, pubkey_y, sizeof(pubkey_y), &pubkey_y_size);
    if (err != CC3XX_ERR_SUCCESS) {
        goto exit;
    }

    output[0] = 0x04;
    memcpy(&output[1], pubkey_x, pubkey_x_size);
    memcpy(&output[1 + pubkey_x_size], pubkey_y, pubkey_y_size);

exit:
    NRF_CRYPTOCELL->ENABLE = 0;
    return tee_map_error_values(err);
}

/* Sealed Key Operations */
CYS_error_t tee_prot_ecc_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (out_len != 2) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_error_t status;

    CYS_PROT_ecc_p256_key_t *sealed_key =
        (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (sealed_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t pubkey_size = out[1].len;
    uint8_t priv_tmp[TEE_ECC_P256_PRIV_KEY_SIZE];

    status = tee_internal_ecc_genkey(CC3XX_EC_CURVE_SECP_256_R1, priv_tmp, sizeof(priv_tmp));
    if (status != CYS_SUCCESS) {
        return status;
    }

    status = tee_internal_ecc_derive(CC3XX_EC_CURVE_SECP_256_R1, priv_tmp, sizeof(priv_tmp), pub_key, pubkey_size);
    if (status != CYS_SUCCESS) {
        return status;
    }

    status = tee_internal_generate_random_bytes(sealed_key->nonce, sizeof(sealed_key->nonce));
    if (status != CYS_SUCCESS) {
        return status;
    }

    return tee_rot_encrypt_key_ocb((uint8_t *)priv_tmp, sealed_key);
}

CYS_error_t tee_prot_ecc_p256_seal(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *priv_key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    CYS_PROT_ecc_p256_key_t *sealed_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (priv_key == NULL || sealed_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    CYS_error_t status = tee_internal_generate_random_bytes(sealed_key->nonce, sizeof(sealed_key->nonce));
    if (status != CYS_SUCCESS) {
        return status;
    }

    return tee_rot_encrypt_key_ocb(priv_key, sealed_key);
}

CYS_error_t tee_prot_ecc_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_PROT_ecc_p256_key_t *key = (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t pubkey_size = out[1].len;
    uint8_t key_clear[TEE_ECC_P256_PRIV_KEY_SIZE];
    CYS_error_t status = tee_rot_decrypt_key_ocb(key, key_clear);
    if (status != CYS_SUCCESS) {
        return status;
    }

    return tee_internal_ecc_derive(CC3XX_EC_CURVE_SECP_256_R1, key_clear, TEE_ECC_P256_PRIV_KEY_SIZE, pub_key, pubkey_size);
}

static CYS_error_t tee_internal_ecdsa_sign(cc3xx_ec_curve_id_t curve_id, uint8_t * privkey, size_t privkey_size, uint8_t *hash, size_t hash_len, uint8_t *signature)
{
    size_t modulus_size = cc3xx_lowlevel_ec_get_modulus_size_from_curve(curve_id);
    uint32_t sig_r[modulus_size / sizeof(uint32_t)];
    uint32_t sig_s[modulus_size / sizeof(uint32_t)];
    size_t sig_r_size, sig_s_size;

    NRF_CRYPTOCELL->ENABLE = 1;

    cc3xx_err_t err = cc3xx_lowlevel_ecdsa_sign(curve_id, (uint32_t *)privkey, privkey_size, (uint32_t *)hash, hash_len, sig_r, sizeof(sig_r), &sig_r_size, sig_s, sizeof(sig_s), &sig_s_size);

    NRF_CRYPTOCELL->ENABLE = 0;

    if (err != CC3XX_ERR_SUCCESS) {
        return tee_map_error_values(err);
    }

    memcpy(signature, sig_r, sig_r_size);
    memcpy(&signature[sig_r_size], sig_s, sig_s_size);

    return CYS_SUCCESS;
}

CYS_error_t tee_prot_ecc_p256_sign(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_PROT_ecc_p256_key_t *key =
        (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t hash_len = in[1].len;
    size_t signature_size = out[0].len;

    if (hash_len != TEE_ECC_P256_HASH_SIZE ||
        signature_size < TEE_ECC_P256_SIGNATURE_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t key_clear[TEE_ECC_P256_PRIV_KEY_SIZE];

    CYS_error_t status = tee_rot_decrypt_key_ocb(key, key_clear);
    if (status != CYS_SUCCESS) {
        return status;
    }

    return tee_internal_ecdsa_sign(CC3XX_EC_CURVE_SECP_256_R1, key_clear, TEE_ECC_P256_PRIV_KEY_SIZE, hash, hash_len, signature);
}

/* ECC Operations */
CYS_error_t tee_ecc_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 0 || out_len != 2) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *priv_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (priv_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t priv_key_len = out[0].len;
    size_t pub_key_len = out[1].len;

    if (priv_key_len != TEE_ECC_P256_PRIV_KEY_SIZE ||
        pub_key_len != TEE_ECC_P256_PUB_KEY_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_error_t status = tee_internal_ecc_genkey(CC3XX_EC_CURVE_SECP_256_R1, priv_key, priv_key_len);
    if (status != CYS_SUCCESS) {
        return status;
    }

    (void) in;
    (void) in_len;
    return tee_internal_ecc_derive(CC3XX_EC_CURVE_SECP_256_R1, priv_key, priv_key_len, pub_key, pub_key_len);
}

CYS_error_t tee_ecc_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *priv_key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    size_t priv_key_len = in[0].len;
    size_t pub_key_len = out[0].len;

    if (priv_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    return tee_internal_ecc_derive(CC3XX_EC_CURVE_SECP_256_R1, priv_key, priv_key_len, pub_key, pub_key_len);
}

CYS_error_t tee_ecc_p256_sign_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 2 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t hash_len = in[1].len;
    size_t signature_size = out[0].len;

    if (hash_len != TEE_ECC_P256_HASH_SIZE ||
        signature_size < TEE_ECC_P256_SIGNATURE_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    return tee_internal_ecdsa_sign(CC3XX_EC_CURVE_SECP_256_R1, key, TEE_ECC_P256_PRIV_KEY_SIZE, hash, hash_len, signature);
}

static CYS_error_t tee_internal_ecdsa_verify(cc3xx_ec_curve_id_t curve_id, uint8_t *pubkey, size_t pubkey_size, uint8_t *hash, size_t hash_len, uint8_t *signature, size_t signature_size)
{
    size_t mod_size = cc3xx_lowlevel_ec_get_modulus_size_from_curve(curve_id);
    size_t pub_xy_len = (pubkey_size - 1) / 2;
    size_t sig_rs_len = signature_size / 2;
    uint32_t pk_x[pub_xy_len];
    uint32_t pk_y[pub_xy_len];
    uint32_t sig_r[mod_size / sizeof(uint32_t)];
    uint32_t sig_s[mod_size / sizeof(uint32_t)];

    memcpy(pk_x, &pubkey[1], pub_xy_len);
    memcpy(pk_y, &pubkey[1 + pub_xy_len], pub_xy_len);
    memcpy(sig_r, signature, sig_rs_len);
    memcpy(sig_s, &signature[sig_rs_len], sig_rs_len);

    NRF_CRYPTOCELL->ENABLE = 1;
    cc3xx_err_t err = cc3xx_lowlevel_ecdsa_verify(curve_id,
                                                  pk_x, pub_xy_len,
                                                  pk_y, pub_xy_len,
                                                  (uint32_t *)hash, hash_len,
                                                  sig_r, sig_rs_len,
                                                  sig_s, sig_rs_len);
    NRF_CRYPTOCELL->ENABLE = 0;

    return tee_map_error_values(err);
}

CYS_error_t tee_ecc_p256_verify_hash(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 3) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *hash = cmse_check_address_range((void *)in[1].data, in[1].len, CMSE_NONSECURE);
    uint8_t *signature = cmse_check_address_range((void *)in[2].data, in[2].len, CMSE_NONSECURE);

    if (key == NULL || hash == NULL || signature == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    size_t key_len = in[0].len;
    size_t hash_len = in[1].len;
    size_t signature_len = in[2].len;

    if (key_len != TEE_ECC_P256_PUB_KEY_SIZE ||
        hash_len != TEE_ECC_P256_HASH_SIZE ||
        signature_len != TEE_ECC_P256_SIGNATURE_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    (void) out;
    (void) out_len;
    return tee_internal_ecdsa_verify(CC3XX_EC_CURVE_SECP_256_R1, key, key_len, hash, hash_len, signature, signature_len);
}
