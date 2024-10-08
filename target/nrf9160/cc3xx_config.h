#ifndef CC3XX_CONFIG_H
#define CC3XX_CONFIG_H

#define CC3XX_BASE_S                (0x50841000)
#define CC3XX_CONFIG_BASE_ADDRESS   (CC3XX_BASE_S)

#define CC3XX_CONFIG_HASH_SHA256_ENABLE

#define CC3XX_CONFIG_AES_ECB_ENABLE
#define CC3XX_CONFIG_AES_CBC_ENABLE

#define CC3XX_CONFIG_RNG_ENABLE
#define CC3XX_CONFIG_DRBG_HMAC_ENABLE
#define CC3XX_CONFIG_ENABLE_RANDOM_HMAC_DRBG

/* RNG config has been copied from TF-M target musca_s1*/
/* The number of times the TRNG will be re-read when it fails a statical test
 * before an error is returned.
 */
#ifndef CC3XX_CONFIG_RNG_MAX_ATTEMPTS
#define CC3XX_CONFIG_RNG_MAX_ATTEMPTS 16
#endif /* CC3XX_CONFIG_RNG_MAX_ATTEMPTS */

/* This is the number of cycles between consecutive samples of the oscillator
 * output. It needs to be set to a _reasonably_ large number, though It's
 * unclear exactly what sort of number is reasonable. In general, if the
 * statistical tests keep failing then increase it, if the RNG is too slow then
 * decrease it.  A sensible default is set here, and has worked correctly with a
 * variety of cc3xx implementations.
 */
#ifndef CC3XX_CONFIG_RNG_SUBSAMPLING_RATE
#define CC3XX_CONFIG_RNG_SUBSAMPLING_RATE 0x1337
#endif /* !CC_RNG_SUBSAMPLING_RATE */

/* Between 0 and 3 inclusive. 0 should be the fastest oscillator ring */
#ifndef CC3XX_CONFIG_RNG_RING_OSCILLATOR_ID
#define CC3XX_CONFIG_RNG_RING_OSCILLATOR_ID 0
#endif /* !CC_RNG_RING_OSCILLATOR_ID */

/* How many virtual registers can be allocated in the PKA engine */
#ifndef CC3XX_CONFIG_PKA_MAX_VIRT_REG_AMOUNT
#define CC3XX_CONFIG_PKA_MAX_VIRT_REG_AMOUNT 64
#endif /* CC3XX_CONFIG_PKA_MAX_VIRT_REG_AMOUNT */

/* Whether barrett tags will be calculated if they are not known. Note that
 * barrett tags are required for modular reduction. If disabled, this may
 * decrease code size.
 */
#define CC3XX_CONFIG_PKA_CALC_NP_ENABLE

/* Whether PKA operations will be inlined to increase performance at the cost of
 * code size
 */
#define CC3XX_CONFIG_PKA_INLINE_FOR_PERFORMANCE

/* Whether PKA variables will be aligned to word-size to increase performance at
 * the cost of code size
 */
#define CC3XX_CONFIG_PKA_ALIGN_FOR_PERFORMANCE

#define CC3XX_CONFIG_EC_CURVE_TYPE_WEIERSTRASS_ENABLE
// #define CC3XX_CONFIG_EC_CURVE_TYPE_TWISTED_EDWARDS_ENABLE

#define CC3XX_CONFIG_EC_CURVE_SECP_256_R1_ENABLE
// #define CC3XX_CONFIG_EC_CURVE_ED25519_ENABLE

/* Whether various ECDSA features are enabled */
#define CC3XX_CONFIG_ECDSA_SIGN_ENABLE
#define CC3XX_CONFIG_ECDSA_VERIFY_ENABLE
#define CC3XX_CONFIG_ECDSA_KEYGEN_ENABLE

#endif // CC3XX_CONFIG_H
