#include "tee_hashes.h"
#include "tee_secure_io.h"

tee_status_t tee_hashes_sha256_setup(tee_hash_ctx_t *ctx)
{
    io_pack_out_t out[1] = {
        { .data = ctx, .len = sizeof(tee_hash_ctx_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_HASH_SHA256_SETUP,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, NULL, out);
}

tee_status_t tee_hashes_sha256_update(tee_hash_ctx_t *ctx,
                                const uint8_t *input,
                                size_t input_length)
{
    io_pack_in_t in[2] = {
        { .data = ctx, .len = sizeof(tee_hash_ctx_t) },
        { .data = (uint8_t *)input, .len = input_length}
    };

    io_operation_info_t info = {
        .operation = TEE_HASH_SHA256_UPDATE,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = 0
    };

    return ns_entry(&info, in, NULL);
}

tee_status_t tee_hashes_sha256_finish(tee_hash_ctx_t *ctx,
                                uint8_t *hash,
                                size_t hash_size,
                                size_t *hash_length)
{
    io_pack_in_t in[1] = {
        { .data = ctx, .len = sizeof(tee_hash_ctx_t) }
    };

    io_pack_out_t out[2] = {
        { .data = hash, .len = hash_size },
        { .data = hash_length, .len = sizeof(size_t)}
    };

    io_operation_info_t info = {
        .operation = TEE_HASH_SHA256_FINISH,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, in, out);
}
