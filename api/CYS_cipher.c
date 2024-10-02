#include "CYS/common.h"
#include "CYS/os_mutex.h"
#include "CYS/unprotected.h"
#include "tee_secure_io.h"
#include "tee_operations.h"

CYS_error_t CYS_aes_128_ecb_encrypt(const uint8_t *key, const uint8_t *message, size_t message_len, uint8_t *ciphertext)
{
    io_pack_in_t in[2] = {
        { .data = key, .len = CYS_AES_128_KEY_SIZE },
        { .data = message, .len = message_len }
    };

    io_pack_out_t out[1] = {
        { .data = ciphertext, .len = message_len }
    };

    io_operation_info_t info = {
        .operation = TEE_CIPHER_AES_128_ECB_ENCRYPT,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t),
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_aes_128_ecb_decrypt(const uint8_t *key, const uint8_t *ciphertext, size_t ciphertext_len, uint8_t *message)
{
    io_pack_in_t in[2] = {
        { .data = key, .len = CYS_AES_128_KEY_SIZE },
        { .data = ciphertext, .len = ciphertext_len }
    };

    io_pack_out_t out[1] = {
        { .data = message, .len = ciphertext_len }
    };

    io_operation_info_t info = {
        .operation = TEE_CIPHER_AES_128_ECB_DECRYPT,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t),
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_aes_128_cbc_encrypt(const uint8_t *key, const uint8_t *nonce, const uint8_t *message, size_t message_len, uint8_t *ciphertext)
{
    io_pack_in_t in[3] = {
        { .data = key, .len = CYS_AES_128_KEY_SIZE },
        { .data = nonce, .len = CYS_AES_128_NONCE_SIZE },
        { .data = message, .len = message_len }
    };

    io_pack_out_t out[1] = {
        { .data = ciphertext, .len = message_len }
    };

    io_operation_info_t info = {
        .operation = TEE_CIPHER_AES_128_CBC_ENCRYPT,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t),
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_aes_128_cbc_decrypt(const uint8_t *key, const uint8_t *nonce, const uint8_t *ciphertext, size_t ciphertext_len, uint8_t *message)
{
    io_pack_in_t in[3] = {
        { .data = key, .len = CYS_AES_128_KEY_SIZE },
        { .data = nonce, .len = CYS_AES_128_NONCE_SIZE },
        { .data = ciphertext, .len = ciphertext_len }
    };

    io_pack_out_t out[1] = {
        { .data = message, .len = ciphertext_len }
    };

    io_operation_info_t info = {
        .operation = TEE_CIPHER_AES_128_CBC_DECRYPT,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t),
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}
