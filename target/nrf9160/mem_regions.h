/*
 * Copyright (C) 2024 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v3. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @defgroup       <name> <description>
 * @{
 *
 * @file        mem_regions.h
 * @brief
 *
 * @author      Lena Boeckmann <lena.boeckmann@haw-hamburg.de>
 *
 */

#ifndef MEM_REGIONS_H
#define MEM_REGIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_START         (0x0)
#define FLASH_SIZE          (0xfa000)   /*< 1024 MB */

#define RAM_START           (0x20000000)
#define RAM_SIZE            (0x40000)     /*< 256 KB */

#define FLASH_START_S       (FLASH_START)
#define FLASH_SIZE_S        (0x10000)    /*< 64 KB*/

#define RAM_START_S         (RAM_START)
#define RAM_SIZE_S          (0x16000)   /*< 90 KB*/

#define FLASH_SIZE_NSC      (0x20)  /*< 32 B */
#define FLASH_START_NSC     ((FLASH_START_S + FLASH_SIZE_S) - FLASH_SIZE_NSC)

#ifdef __cplusplus
}
#endif

#endif /* MEM_REGIONS_H */
/** @} */
