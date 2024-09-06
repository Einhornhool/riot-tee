#include "CYS/common.h"
#include "tee_secure_io.h"
#include "CYS/protected_key.h"
#include "tee_ecc.h"

CYS_error_t CYS_PROT_p256_sign(const CYS_PROT_p256_key_t *key, const uint8_t *hash, size_t hash_len, uint8_t *signature)
{
    return CYS_ERROR_NOT_SUPPORTED;
}

CYS_error_t CYS_PROT_p256_generate(CYS_PROT_p256_key_t *sealed_key, uint8_t *public_key)
{
    io_pack_out_t out[2] = {
        { .data = sealed_key, .len = sizeof(CYS_PROT_p256_key_t) },
        { .data = public_key, .len = CYS_PROT_P256_PUB_SIZE }
    };

    io_operation_info_t info = {
        .operation = CYS_PROT_P256_GENERATE,
        .in_len = 0,
        .out_len = sizeof(out)/sizeof(io_pack_out_t)
    };

    return ns_entry(&info, NULL, out);
}
