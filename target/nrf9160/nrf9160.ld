/* Linker script to configure memory regions. */

FLASH_START         = 0x0;
FLASH_SIZE          = 0xfa000;   /*< 1024 MB */

RAM_START           = 0x20000000;
RAM_SIZE            = 0x3e8;     /*< 256 KB */

FLASH_START_S       = FLASH_START;
FLASH_SIZE_S        = 0xfa00;    /*< 64 KB*/

RAM_START_S         = RAM_START;
RAM_SIZE_S          = 0x16000;   /*< 90 KB*/

FLASH_SIZE_NSC      = 0x20;  /*< 32 B */
FLASH_START_NSC     = (FLASH_START_S + FLASH_SIZE_S) - FLASH_SIZE_NSC;
