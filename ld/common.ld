/* Linker script to configure memory regions. */

INCLUDE "nrf9160.ld"

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

_flash_start = FLASH_START_S;
_flash_len = FLASH_SIZE_S;

_flash_nsc_len = FLASH_SIZE_NSC;
_flash_nsc_start = FLASH_START_NSC;

_ram_start = RAM_START_S;
_ram_len = RAM_SIZE_S;

MEMORY
{
  FLASH (rx) : ORIGIN = _flash_start, LENGTH = _flash_len
  FLASH_NSC (rx) : ORIGIN = _flash_nsc_start, LENGTH = _flash_nsc_len
  RAM (rwx) : ORIGIN = _ram_start, LENGTH = _ram_len
}

INCLUDE "nrf_common.ld"

SECTIONS
{
  .gnu.sgstubs : ALIGN (32)
  {
      . = ALIGN(32);
      *(.gnu.sgstubs*)
      . = ALIGN(32);
  } > FLASH_NSC
}
