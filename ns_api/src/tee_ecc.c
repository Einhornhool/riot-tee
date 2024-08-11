#include "tee_ecc.h"
#include "tee_secure_io.h"

tee_status_t tee_generate_ecc_p256r1_key_pair(uint8_t *priv_key_buffer,
                                              uint8_t *pub_key_buffer,
                                              size_t *priv_key_buffer_length,
                                              size_t *pub_key_buffer_length)
{
    io_pack_out_t out[4] = {
        { .data = priv_key_buffer, .len = *priv_key_buffer_length },
        { .data = pub_key_buffer, .len = *pub_key_buffer_length },
        { .data = priv_key_buffer_length, .len = sizeof(size_t) },
        { .data = pub_key_buffer_length, .len = sizeof(size_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_GENERATE,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, NULL, out);
}

tee_status_t tee_ecc_p256r1_sign_hash(const uint8_t *key_buffer, size_t key_buffer_size,
                                      const uint8_t *hash, size_t hash_length,
                                      uint8_t *signature, size_t signature_size,
                                      size_t *signature_length)
{
    io_pack_in_t in[2] = {
        { .data = key_buffer, .len = key_buffer_size },
        { .data = hash, .len = hash_length }
    };

    io_pack_out_t out[2] = {
        { .data = signature, .len = signature_size },
        { .data = signature_length, .len = sizeof(size_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_SIGN_HASH,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, in, out);
}

tee_status_t tee_ecc_p256r1_sign_message(const uint8_t *key_buffer, size_t key_buffer_size,
                                        const uint8_t *input, size_t input_length,
                                        uint8_t *signature, size_t signature_size,
                                        size_t *signature_length)
{
    io_pack_in_t in[2] = {
        { .data = key_buffer, .len = key_buffer_size },
        { .data = input, .len = input_length }
    };

    io_pack_out_t out[2] = {
        { .data = signature, .len = signature_size },
        { .data = signature_length, .len = sizeof(size_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_SIGN_MSG,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, in, out);
}

tee_status_t tee_ecc_p256r1_verify_hash(const uint8_t *key_buffer, size_t key_buffer_size,
                                        const uint8_t *hash, size_t hash_length,
                                        const uint8_t *signature, size_t signature_length)
{
    io_pack_in_t in[3] = {
        { .data = key_buffer, .len = key_buffer_size },
        { .data = hash, .len = hash_length },
        { .data = signature, .len = signature_length }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_VERIFY_HASH,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = 0
    };

    return ns_entry(&info, in, NULL);
}

tee_status_t tee_ecc_p256r1_verify_message(const uint8_t *key_buffer, size_t key_buffer_size,
                                        const uint8_t *input, size_t input_length,
                                        const uint8_t *signature, size_t signature_length)
{
    io_pack_in_t in[3] = {
        { .data = key_buffer, .len = key_buffer_size },
        { .data = input, .len = input_length },
        { .data = signature, .len = signature_length }
    };

    io_operation_info_t info = {
        .operation = TEE_ECC_P256_VERIFY_MSG,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = 0
    };

    return ns_entry(&info, in, NULL);
}
