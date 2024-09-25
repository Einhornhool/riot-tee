#ifndef CC310_HASH_SHA_H
#define CC310_HASH_SHA_H

#include <stdint.h>
#include <stddef.h>
#include "CYS/common.h"
#include "CYS/unprotected.h"

CYS_error_t cc310_hash_sha256_init(CYS_hash_sha256_ctx_t *ctx);

CYS_error_t cc310_hash_sha256_update(CYS_hash_sha256_ctx_t *ctx, uint8_t *input, size_t input_len);

CYS_error_t cc310_hash_sha256_finish(CYS_hash_sha256_ctx_t *ctx, uint8_t *digest, size_t digest_len);

#endif /* CC310_HASH_SHA256_H */
