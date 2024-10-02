#include "CYS/common.h"
#include "CYS/os_mutex.h"
#include "CYS/unprotected.h"
#include "tee_operations.h"
#include "tee_secure_io.h"

CYS_error_t CYS_random_generate(uint8_t *buffer, size_t size)
{
    io_pack_out_t out[1] = {
        { .data = buffer, .len = size },
    };

    io_operation_info_t info = {
        .operation = TEE_RANDOM_GENERATE,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    while (os_get_mutex() != CYS_SUCCESS) {};
    CYS_error_t status = tee_secure_entry(&info, NULL, out);
    os_release_mutex();

    return status;
}
