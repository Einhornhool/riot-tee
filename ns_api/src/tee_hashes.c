#include "tee_hashes.h"
#include "tee_secure_io.h"

extern tee_status_t ns_entry(int32_t operation,
                            io_pack_t *in,
                            io_pack_t *out);

tee_status_t tee_hashes_sha256_setup(void *ctx)
{
    io_pack_t out[1] = {
        { .data = ctx, .len = 0 }
    };

    io_pack_t *in = NULL;

    return ns_entry(TEE_HASH_SHA256_SETUP, in, out);
}

tee_status_t tee_hashes_sha256_update(void *ctx,
                                const uint8_t *input,
                                size_t input_length)
{
    io_pack_t in[2] = {
        { .data = ctx, .len = 0 },
        { .data = (uint8_t *)input, .len = input_length}
    };

    io_pack_t *out = NULL;

    return ns_entry(TEE_HASH_SHA256_UPDATE, in, out);
}

tee_status_t tee_hashes_sha256_finish(void *ctx,
                                uint8_t *hash,
                                size_t hash_size,
                                size_t *hash_length)
{
    io_pack_t in[1] = {
        { .data = ctx, .len = 0 }
    };

    io_pack_t out[2] = {
        { .data = hash, .len = hash_size },
        { .data = hash_length, .len = sizeof(size_t)}
    };

    return ns_entry(TEE_HASH_SHA256_FINISH, in, out);
}
