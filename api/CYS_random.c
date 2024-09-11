#include "CYS/common.h"
#include "CYS/unprotected.h"
#include "tee_operations.h"
#include "tee_secure_io.h"

CYS_error_t CYS_random_generate(uint8_t *buffer, size_t size, size_t *len)
{
    io_pack_out_t out[2] = {
        { .data = buffer, .len = size },
        { .data = len, .len = sizeof(size_t) }
    };

    io_operation_info_t info = {
        .operation = TEE_RANDOM_GENERATE,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return tee_secure_entry(&info, NULL, out);
}
