#ifndef TEE_RANDOM_H
#define TEE_RANDOM_H

#include "CYS/common.h"
#include "tee_secure_io.h"

CYS_error_t tee_generate_random_bytes(io_pack_in_t *in, size_t in_len, io_pack_out_t *out, size_t out_len);

#endif /* TEE_RANDOM_H */
