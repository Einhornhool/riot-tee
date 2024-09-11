/* Linker script to configure memory regions. */

FLASH_START         = 0x0;
FLASH_SIZE          = 0x100000;   /*< 1024 MB */

FLASH_START_S       = FLASH_START;
FLASH_SIZE_S        = 0x10000;    /*< 64 KB*/

FLASH_SIZE_NSC      = 0x20;  /*< 32 B */
FLASH_START_NSC     = (FLASH_START_S + FLASH_SIZE_S) - FLASH_SIZE_NSC;

FLASH_START_NS      = FLASH_START_S + FLASH_SIZE_S;
FLASH_SIZE_NS       = FLASH_SIZE - FLASH_SIZE_S;

RAM_START           = 0x20000000;
RAM_SIZE            = 0x40000;     /*< 256 KB */

RAM_START_S         = RAM_START;
RAM_SIZE_S          = 0x16000;   /*< 90 KB*/

RAM_START_NS        = RAM_START_S + RAM_SIZE_S;
RAM_SIZE_NS         = RAM_SIZE - RAM_SIZE_S;
