#include "random.h"
#include "CYS/common.h"
#include "CYS/sealed_key.h"
#include "CYS/unprotected.h"

#include "tee_rot.h"
#include "tee_ecc.h"
#include "tee_io_sanitizer.h"

#include "ocrypto_ecdsa_p256.h"

#define MAX_REPETITION                  (10)
#define TEE_ECC_P256_PRIV_KEY_SIZE      (32)
#define TEE_ECC_P256_PUB_KEY_SIZE       (65)
#define TEE_ECC_P256_SESSION_KEY_SIZE   (32)
#define TEE_ECC_P256_SIGNATURE_SIZE     (64)
#define TEE_ECC_P256_HASH_SIZE          (32)

/* Sealed Key Operations */
CYS_error_t tee_prot_p256_generate(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (out_len != 2) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    CYS_PROT_ecc_p256_key_t *sealed_key =
        (CYS_PROT_ecc_p256_key_t *)cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[1].data, out[1].len, CMSE_NONSECURE);

    if (sealed_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    uint8_t priv_tmp[TEE_ECC_P256_PRIV_KEY_SIZE];

    CYS_error_t status = CYS_ecc_p256_generate(priv_tmp, pub_key);
    if (status != CYS_SUCCESS) {
        return status;
    }

    random_bytes(sealed_key->nonce, sizeof(sealed_key->nonce));

    if (tee_rot_encrypt_key_ocb(priv_tmp, sealed_key) < 0) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_SUCCESS;
}

CYS_error_t tee_prot_p256_seal(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    (void) in;
    (void) in_len;
    (void) out;
    (void) out_len;
    return CYS_ERROR_NOT_SUPPORTED;
}

CYS_error_t tee_prot_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    (void) in;
    (void) in_len;
    (void) out;
    (void) out_len;
    return CYS_ERROR_NOT_SUPPORTED;
}

CYS_error_t tee_prot_p256_sign(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
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

    if (hash_len != TEE_ECC_P256_HASH_SIZE) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t key_clear[TEE_ECC_P256_PRIV_KEY_SIZE];

    if (tee_rot_decrypt_key_ocb(key, key_clear) < 0) {
        return CYS_ERROR_GENERIC_ERROR;
    }

    return CYS_ecc_p256_sign(key_clear, hash, hash_len, signature);
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

    (void) in;
    (void) in_len;
    return CYS_ecc_p256_generate(priv_key, pub_key);
}

CYS_error_t tee_ecc_p256_derive(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len)
{
    if (in_len != 1 || out_len != 1) {
        return CYS_ERROR_INVALID_ARGUMENT;
    }

    uint8_t *priv_key = cmse_check_address_range((void *)in[0].data, in[0].len, CMSE_NONSECURE);
    uint8_t *pub_key = cmse_check_address_range(out[0].data, out[0].len, CMSE_NONSECURE);

    if (priv_key == NULL || pub_key == NULL) {
        return CYS_ERROR_CORRUPTION_DETECTED;
    }

    (void) in;
    (void) in_len;
    return CYS_ecc_p256_derive(priv_key, pub_key);
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

    return CYS_ecc_p256_sign(key, hash, hash_len, signature);
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

    size_t hash_len = in[1].len;

    (void) out;
    (void) out_len;
    return CYS_ecc_p256_verify(key, hash, hash_len, signature);
}
