#ifndef CC310_REGISTERS_H
#define CC310_REGISTERS_H

#include <stdint.h>
#include "nrf9160.h"
#include "nrf9160_bitfields.h"

typedef struct {
    __IM uint32_t RESERVED[64];           /*0x000*/
    __IOM uint32_t RNG_IMR;               /*0x100*/
    __IM uint32_t RNG_ISR;                /*0x104*/
    __OM uint32_t RNG_ICR;                /*0x108*/
    __IOM uint32_t TRNG_CONFIG;           /*0x10C*/
    __IM uint32_t TRNG_VALID;             /*0x110*/
    __IM uint32_t EHR_DATA[6];            /*0x114*/
    __IOM uint32_t NOISE_SOURCE;            /*0x12C*/
    __IOM uint32_t SAMPLE_CNT;              /*0x130*/
    __IOM uint32_t AUTOCORR_STATISTIC;      /*0x134*/
    __IOM uint32_t TRNG_DEBUG;              /*0x138*/
    __IM uint32_t RESERVED1;              /*0x13C*/
    __OM uint32_t RNG_SW_RESET;            /*0x140*/
    __IM uint32_t RESERVED2[29];           /*0x144*/
    __IM uint32_t RNG_BUSY;                /*0x1B8*/
    __OM uint32_t TRNG_RESET;              /*0x1BC*/
    __IM uint32_t RNG_HW_FLAGS;            /*0x1C0*/
    __OM uint32_t RNG_CLK;	                /*0x1C4*/
    __IOM uint32_t RNG_DMA;	                /*0x1C8*/
    __IOM uint32_t RNG_DMA_ROSC_LEN;        /*0x1CC*/
    __IOM uint32_t RNG_DMA_SRAM_ADDR;       /*0x1D0*/
    __IOM uint32_t RNG_DMA_SAMPLES_NUM;     /*0x1D4*/
    __IOM uint32_t RNG_WATCHDOG_VAL;        /*0x1D8*/
    __IM uint32_t RNG_DMA_BUSY;            /*0x1DC*/
} TEE_CC_RNG_Type;

typedef struct {
    __IM  uint32_t RESERVED[256];           /*0x000*/
    __OM  uint32_t AES_KEY_0[8];	        /*0x400*/
    __IM  uint32_t RESERVED1[8];            /*0x420*/
    __IOM uint32_t AES_IV_0[4];	            /*0x440*/
    __IM uint32_t RESERVED2[4];             /*0x450*/
    __IOM uint32_t AES_CTR[4];              /*0x460*/
    __IM  uint32_t AES_BUSY;	            /*0x470*/
    __IM  uint32_t RESERVED3;	            /*0x474*/
    __OM  uint32_t AES_SK;	                /*0x478*/
    __OM  uint32_t AES_CMAC_INIT;	        /*0x47C*/
    __IM  uint32_t RESERVED4[15];           /*0x480*/
    __IOM uint32_t AES_REMAINING_BYTES;	    /*0x4BC*/
    __IOM uint32_t AES_CONTROL;	            /*0x4C0*/
    __IM uint32_t RESERVED5;	            /*0x4C4*/
    __IM  uint32_t AES_HW_FLAGS;	        /*0x4C8*/
    __IM  uint32_t RESERVED6[3];            /*0x4CC*/
    __IOM uint32_t AES_CTR_NO_INCREMENT;	/*0x4D8*/
    __IM  uint32_t RESERVED7[6];            /*0x4DC*/
    __OM  uint32_t AES_SW_RESET;	        /*0x4F4*/
    __IM  uint32_t RESERVED8[11];           /*0x4F8*/
    __OM  uint32_t AES_CMAC_SIZE0_KICK;	    /*0x524*/
} TEE_CC_AES_Type;

typedef struct {
    __IM uint32_t RESERVED[400];            /*0x000*/
    __IOM uint32_t HASH_H[8];               /*0x640*/
    __IM uint32_t RESERVED1[9];             /*0x660*/
    __OM uint32_t HASH_PAD_AUTO;            /*0x684*/
    __IM uint32_t RESERVED2[3];             /*0x688*/
    __OM uint32_t HASH_INIT_STATE;          /*0x694*/
    __IM uint32_t RESERVED3[70];            /*0x698*/
    __IM uint32_t HASH_VERSION;             /*0x7B0*/
    __IM uint32_t RESERVED4[3];             /*0x7B4*/
    __IOM uint32_t HASH_CONTROL;            /*0x7C0*/
    __IOM uint32_t HASH_PAD;                /*0x7C4*/
    __IOM uint32_t HASH_PAD_FORCE;          /*0x7C8*/
    __IOM uint32_t HASH_CUR_LEN_0;          /*0x7CC*/
    __IOM uint32_t HASH_CUR_LEN_1;          /*0x7D0*/
    __IM uint32_t RESERVED5[2];             /*0x7D4*/
    __IM uint32_t HASH_HW_FLAGS;            /*0x7DC*/
    __IM uint32_t RESERVED6;                /*0x7E0*/
    __OM uint32_t HASH_SW_RESET;            /*0x7E4*/
    __IOM uint32_t HASH_ENDIANNESS;         /*0x7E8*/
} TEE_CC_HASH_Type;

typedef struct {
    __IOM uint32_t AHBM_SINGLES;            /*0xB00*/
    __IOM uint32_t AHBM_HPROT;              /*0xB04*/
    __IOM uint32_t AHBM_HMASTLOCK;          /*0xB08*/
    __IOM uint32_t AHBM_HNONSEC;            /*0xB0C*/
} TEE_CC_AHB_Type;

typedef struct {
    __IM uint32_t RESERVED[576];            /*0x000*/
    __OM uint32_t CRYPTO_CTL;               /*0x900*/
    __IM uint32_t RESERVED1[3];             /*0x904*/
    __IM uint32_t CRYPTO_BUSY;              /*0x910*/
    __IM uint32_t RESERVED2[2];             /*0x914*/
    __IM uint32_t HASH_BUSY;                /*0x91C*/
    __IM uint32_t RESERVED3[4];             /*0x920*/
    __IOM uint32_t CONTEXT_ID;              /*0x930*/
} TEE_CC_CTL_Type;

typedef struct {
    __IM uint32_t RESERVED[768];            /*0x000*/
    __OM uint32_t DIN_BUFFER[8];            /*0xC00*/
    __IM uint32_t DIN_DMA_MEM_BUSY;	        /*0xC20*/
    __IM uint32_t RESERVED1;	            /*0xC24*/
    __OM uint32_t SRC_MEM_ADDR;	            /*0xC28*/
    __OM uint32_t SRC_MEM_SIZE;	            /*0xC2C*/
    __IOM uint32_t SRC_SRAM_ADDR;	        /*0xC30*/
    __OM uint32_t SRC_SRAM_SIZE;	        /*0xC34*/
    __IM uint32_t DIN_DMA_SRAM_BUSY;	    /*0xC38*/
    __IOM uint32_t DIN_DMA_SRAM_ENDIANNESS;	/*0xC3C*/
    __IM uint32_t RESERVED2;	            /*0xC40*/
    __OM uint32_t DIN_SW_RESET;	            /*0xC44*/
    __OM uint32_t DIN_CPU_DATA;	            /*0xC48*/
    __OM uint32_t DIN_WRITE_ALIGN;	        /*0xC4C*/
    __IM uint32_t DIN_FIFO_EMPTY;	        /*0xC50*/
    __IM uint32_t RESERVED3;	            /*0xC54*/
    __OM uint32_t DIN_FIFO_RESET;           /*0xC58*/
} TEE_CC_DIN_Type;

typedef struct {
    __IM uint32_t RESERVED[768];            /*0x000*/
    __OM uint32_t DOUT_BUFFER[8];           /*0xC00*/
    __IM uint32_t RESERVED1[64];            /*0xC20*/
    __IM uint32_t DOUT_DMA_MEM_BUSY;	    /*0xD20*/
    __IM uint32_t RESERVED2;	            /*0xD24*/
    __OM uint32_t DST_MEM_ADDR;	            /*0xD28*/
    __OM uint32_t DST_MEM_SIZE;	            /*0xD2C*/
    __IOM uint32_t DST_SRAM_ADDR;	        /*0xD30*/
    __OM uint32_t DST_SRAM_SIZE;	        /*0xD34*/
    __IM uint32_t DOUT_DMA_SRAM_BUSY;	    /*0xD38*/
    __IOM uint32_t DOUT_DMA_SRAM_ENDIANNESS;	/*0xD3C*/
    __IM uint32_t RESERVED3;	            /*0xD40*/
    __OM uint32_t DOUT_READ_ALIGN;          /*0xD44*/
    __IM uint32_t RESERVED4[2];	            /*0xD48*/
    __IM uint32_t DOUT_FIFO_EMPTY;	        /*0xD50*/
    __IM uint32_t RESERVED5;	            /*0xD54*/
    __OM uint32_t DOUT_SW_RESET;          /*0xD58*/
} TEE_CC_DOUT_Type;

typedef struct {
    __IM  uint32_t RESERVED[640];           /*0x000*/
    __IM  uint32_t IRR;	                    /*0xA00*/
    __IOM uint32_t IMR;	                    /*0xA04*/
    __OM  uint32_t ICR;	                    /*0xA08*/
    __IOM uint32_t ENDIANNESS;              /*0xA0C*/
    __IM  uint32_t RESERVED1[5];            /*0xA10*/
    __IM  uint32_t HOST_SIGNATURE;          /*0xA24*/
    __IM  uint32_t HOST_BOOT;	            /*0xA28*/
    __IM  uint32_t RESERVED2[3];            /*0xA2C*/
    __IOM uint32_t HOST_CRYPTOKEY_SEL;      /*0xA38*/
    __IM  uint32_t RESERVED3[4];            /*0xA3C*/
    __IOM uint32_t HOST_IOT_KPRTL_LOCK;     /*0xA4C*/
    __IOM uint32_t HOST_IOT_KDR0;	        /*0xA50*/
    __OM  uint32_t HOST_IOT_KDR1;	        /*0xA54*/
    __OM  uint32_t HOST_IOT_KDR2;	        /*0xA58*/
    __OM  uint32_t HOST_IOT_KDR3;	        /*0xA5C*/
    __IOM uint32_t HOST_IOT_LCS;            /*0xA60*/
} TEE_CC_HOST_RGF_Type;

typedef struct {
    __IM uint32_t RESERVED[516];           /*0x000*/
    __OM uint32_t AES_CLK;                 /*0x810*/
    __IM uint32_t RESERVED1;               /*0x814*/
    __OM uint32_t HASH_CLK;                /*0x818*/
    __OM uint32_t PKA_CLK;                 /*0x81C*/
    __OM uint32_t DMA_CLK;                 /*0x820*/
    __IM uint32_t CLK_STATUS;              /*0x824*/
    __IM uint32_t RESERVED2[12];           /*0x828*/
    __OM uint32_t CHACHA_CLK;              /*0x858*/
} TEE_CC_MISC_Type;

#define TEE_CC_AES_BASE          (0x50841000)
#define TEE_CC_HASH_BASE         (0x50841000)
#define TEE_CC_AHB_BASE          (0x50841000)
#define TEE_CC_CTL_BASE          (0x50841000)
#define TEE_CC_DIN_BASE          (0x50841000)
#define TEE_CC_DOUT_BASE         (0x50841000)
#define TEE_CC_MISC_BASE         (0x50841000)
#define TEE_CC_HOST_RGF_BASE     (0x50841000)
#define TEE_CC_RNG_BASE          (0x50841000)

#define TEE_CC_AES               ((TEE_CC_AES_Type *) TEE_CC_AES_BASE)
#define TEE_CC_HASH             ((TEE_CC_HASH_Type *) TEE_CC_HASH_BASE)
#define TEE_CC_AHB               ((TEE_CC_AHB_Type *) TEE_CC_AHB_BASE)
#define TEE_CC_CTL               ((TEE_CC_CTL_Type *) TEE_CC_CTL_BASE)
#define TEE_CC_DIN               ((TEE_CC_DIN_Type *) TEE_CC_DIN_BASE)
#define TEE_CC_DOUT             ((TEE_CC_DOUT_Type *) TEE_CC_DOUT_BASE)
#define TEE_CC_MISC             ((TEE_CC_MISC_Type *) TEE_CC_MISC_BASE)
#define TEE_CC_HOST_RGF     ((TEE_CC_HOST_RGF_Type *) TEE_CC_HOST_RGF_BASE)
#define TEE_CC_RNG               ((TEE_CC_RNG_Type *) TEE_CC_RNG_BASE)

#define TEE_CC_MISC_AES_CLK_ENABLE_Enable       (1UL)
#define TEE_CC_MISC_HASH_CLK_ENABLE_Enable      (1UL)
#define TEE_CC_MISC_DMA_CLK_ENABLE_Enable       (1UL)
#define TEE_CC_RNG_RNG_CLK_ENABLE_Enable        (1UL)

#define TEE_CC_RNG_RNG_SW_RESET_RESET_Enable    (1UL)

#define TEE_CC_RNG_TRNG_CONFIG_ROSC_LEN_ROSC2   (1UL << 1)
#define TEE_CC_RNG_NOISE_SOURCE_ENABLE_Enabled  (1UL)

#define TEE_CC_CTL_CRYPTO_BUSY_STATUS_Busy      (1UL)
#define TEE_CC_CTL_HASH_BUSY_STATUS_Busy        (1UL)

#define TEE_CC_CTL_CRYPTO_CTL_MODE_AESActive    (0x01UL)
#define TEE_CC_CTL_CRYPTO_CTL_MODE_HashActive   (0x07UL)

#define TEE_CC_AES_ECB_ENCRYPT                  (0x00000000UL)
#define TEE_CC_AES_ECB_DECRYPT                  (0x00000001UL)

#define TEE_CC_AES_CBC_ENCRYPT                  (0x00000100UL)
#define TEE_CC_AES_CBC_DECRYPT                  (0x00000101UL)

#define TEE_CC_HASH_HASH_CONTROL_MODE_SHA256    (0x00000002UL)
#define TEE_CC_HASH_HASH_CONTROL_MODE_SHA224    (0x0000000AUL)

#define TEE_CC_HOST_RGF_IRR_MEM_TO_DIN_INT_Msk      (1UL << 6)
#define TEE_CC_HOST_RGF_IRR_DOUT_TO_MEM_INT_Msk     (1UL << 7)
#define TEE_CC_RNG_RNG_ISR_EHR_VALID_INT_Msk        (1UL << 0)
#endif /* CC310_REGISTERS_H */
