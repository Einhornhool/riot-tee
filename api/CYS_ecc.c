#include "CYS/common.h"
#include "CYS/os_mutex.h"
#include "CYS/sealed_key.h"
#include "CYS/unprotected.h"
#include "tee_secure_io.h"
#include "tee_operations.h"

/* Sealed Key Operations */
CYS_error_t CYS_PROT_ecc_p256_generate(CYS_PROT_ecc_p256_key_t *sealed_key, uint8_t *public_key)
{
    io_pack_out_t out[2] = {
        { .data = sealed_key, .len = sizeof(CYS_PROT_ecc_p256_key_t) },
        { .data = public_key, .len = CYS_PROT_ECC_P256_PUB_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_PROT_ECC_P256_GENERATE,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, NULL, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_PROT_ecc_p256_seal(const uint8_t *unsealed_key, CYS_PROT_ecc_p256_key_t *sealed_key)
{
    io_pack_in_t in[1] = {
        { .data = unsealed_key, .len = CYS_ECC_P256_KEY_SIZE }
    };

    io_pack_out_t out[1] = {
        { .data = sealed_key, .len = sizeof(CYS_PROT_ecc_p256_key_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_PROT_ECC_P256_SEAL,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_PROT_ecc_p256_derive(const CYS_PROT_ecc_p256_key_t *sealed_key, uint8_t *public_key)
{
    io_pack_in_t in[1] = {
        { .data = sealed_key, .len = sizeof(CYS_PROT_ecc_p256_key_t) }
    };

    io_pack_out_t out[1] = {
        { .data = public_key, .len = CYS_PROT_ECC_P256_PUB_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_PROT_ECC_P256_DERIVE,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };
    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_PROT_ecc_p256_sign(const CYS_PROT_ecc_p256_key_t *key, const uint8_t *hash, size_t hash_len, uint8_t *signature)
{
    io_pack_in_t in[2] = {
        { .data = key, .len = sizeof(CYS_PROT_ecc_p256_key_t) },
        { .data = hash, .len = hash_len }
    };

    io_pack_out_t out[1] = {
        { .data = signature, .len = CYS_PROT_ECC_P256_SIG_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_PROT_ECC_P256_SIGN,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

/* ECC Operations */
CYS_error_t CYS_ecc_p256_generate(uint8_t *private_key, uint8_t *public_key)
{
    io_pack_out_t out[2] = {
        { .data = private_key, .len = CYS_ECC_P256_KEY_SIZE },
        { .data = public_key, .len = CYS_ECC_P256_PUB_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_GENERATE,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, NULL, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_ecc_p256_derive(const uint8_t *private_key, uint8_t *public_key)
{
    io_pack_in_t in[1] = {
        { .data = private_key, .len = CYS_ECC_P256_KEY_SIZE }
    };

    io_pack_out_t out[1] = {
        { .data = public_key, .len = CYS_ECC_P256_PUB_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_DERIVE,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };
    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_ecc_p256_sign(const uint8_t *private_key, const uint8_t *hash, size_t hash_len, uint8_t *signature)
{
    io_pack_in_t in[2] = {
        { .data = private_key, .len = CYS_ECC_P256_KEY_SIZE },
        { .data = hash, .len = hash_len }
    };

    io_pack_out_t out[1] = {
        { .data = signature, .len = CYS_ECC_P256_SIG_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_SIGN_HASH,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_ecc_p256_verify(const uint8_t *public_key, const uint8_t *hash, size_t hash_len, const uint8_t *signature)
{
    io_pack_in_t in[3] = {
        { .data = public_key, .len = CYS_ECC_P256_PUB_SIZE },
        { .data = hash, .len = hash_len },
        { .data = signature, .len = CYS_ECC_P256_SIG_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_VERIFY_HASH,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = 0
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, NULL);
    os_release_mutex();

    return status;
}
