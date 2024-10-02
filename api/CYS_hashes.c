#include "CYS/common.h"
#include "CYS/os_mutex.h"
#include "CYS/unprotected.h"
#include "tee_secure_io.h"
#include "tee_operations.h"

CYS_error_t CYS_hash_sha256_init(CYS_hash_sha256_ctx_t *ctx)
{
    io_pack_out_t out[1] = {
        { .data = ctx, .len = sizeof(CYS_hash_sha256_ctx_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_HASH_SHA256_SETUP,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, NULL, out);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_hash_sha256_update(CYS_hash_sha256_ctx_t *ctx, const uint8_t *data, size_t len)
{
    io_pack_in_t in[2] = {
        { .data = ctx, .len = sizeof(CYS_hash_sha256_ctx_t) },
        { .data = data, .len = len }
    };

    io_operation_info_t info = {
        .operation = TEE_HASH_SHA256_UPDATE,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = 0
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, NULL);
    os_release_mutex();

    return status;
}

CYS_error_t CYS_hash_sha256_finalize(CYS_hash_sha256_ctx_t *ctx, uint8_t *digest)
{
    io_pack_in_t in[1] = {
        { .data = ctx, .len = sizeof(CYS_hash_sha256_ctx_t) }
    };

    io_pack_out_t out[1] = {
        { .data = digest, .len = CYS_HASH_SHA256_DIGEST_SIZE }
    };

    io_operation_info_t info = {
        .operation = TEE_HASH_SHA256_FINISH,
        .in_len = sizeof(in)/sizeof(io_pack_in_t),
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, in, out);
    os_release_mutex();

    return status;
}
