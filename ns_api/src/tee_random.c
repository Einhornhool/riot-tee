#include "tee_random.h"
#include "tee_secure_io.h"

tee_status_t tee_generate_random(uint8_t *output, size_t output_size)
{
    io_pack_out_t out[1] = {
        { .data = output, .len = output_size }
    };

    io_operation_info_t info = {
        .operation = TEE_RANDOM,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, NULL, out);
}
