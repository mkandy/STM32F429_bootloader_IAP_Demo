/**
 * @file stm32h7xx_flash_driver.h
 * @brief STM32H7xx CycloneBOOT flash driver
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2021 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.0.1
 **/

#ifndef _STM32H7XX_FLASH_DRIVER_H
#define _STM32H7XX_FLASH_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_flash_ex.h"
#include "iap/iap_driver.h"
#include "error.h"

//Flash sectors number
#define FLASH_SECTORS_NUMBER 16

//Flash Sector start address list (from STM32H7 datasheet)
#define FLASH_SECTOR_0_ADDR   0x08000000 //Base address of Sector 0,  End address at 0x0801FFFF, 128 Kbytes  | 128 Kbytes     (bank 1 sector 0 address)
#define FLASH_SECTOR_1_ADDR   0x08020000 //Base address of Sector 1,  End address at 0x0803FFFF, 128 Kbytes  | 256 Kbytes     (bank 1 sector 1 address)
#define FLASH_SECTOR_2_ADDR   0x08040000 //Base address of Sector 2,  End address at 0x0805FFFF, 128 Kbytes  | 384 Kbytes     (bank 1 sector 2 address)
#define FLASH_SECTOR_3_ADDR   0x08060000 //Base address of Sector 3,  End address at 0x0807FFFF, 128 Kbytes  | 512 Kbytes     (bank 1 sector 3 address)
#define FLASH_SECTOR_4_ADDR   0x08080000 //Base address of Sector 4,  End address at 0x0809FFFF, 128 Kbytes  | 640 Kbytes     (bank 1 sector 4 address)
#define FLASH_SECTOR_5_ADDR   0x080A0000 //Base address of Sector 5,  End address at 0x080BFFFF, 128 Kbytes  | 768 Kbytes     (bank 1 sector 5 address)
#define FLASH_SECTOR_6_ADDR   0x080C0000 //Base address of Sector 6,  End address at 0x080DFFFF, 128 Kbytes  | 896 Kbytes     (bank 1 sector 6 address)
#define FLASH_SECTOR_7_ADDR   0x080E0000 //Base address of Sector 7,  End address at 0x080FFFFF, 128 Kbytes  | 1024 Kbytes    (bank 1 sector 7 address)
#define FLASH_SECTOR_8_ADDR   0x08100000 //Base address of Sector 8,  End address at 0x0811FFFF, 128 Kbytes  | 1152 Kbytes    (bank 2 sector 0 address)
#define FLASH_SECTOR_9_ADDR   0x08120000 //Base address of Sector 9,  End address at 0x0813FFFF, 128 Kbytes  | 1280 Kbytes    (bank 2 sector 1 address)
#define FLASH_SECTOR_10_ADDR  0x08140000 //Base address of Sector 10, End address at 0x0815FFFF, 128 Kbytes  | 1408 Kbytes    (bank 2 sector 2 address)
#define FLASH_SECTOR_11_ADDR  0x08160000 //Base address of Sector 11, End address at 0x0817FFFF, 128 Kbytes  | 1536 Kbytes    (bank 2 sector 3 address)
#define FLASH_SECTOR_12_ADDR  0x08180000 //Base address of Sector 12, End address at 0x0819FFFF, 128 Kbytes  | 1664 Kbytes    (bank 2 sector 4 address)
#define FLASH_SECTOR_13_ADDR  0x081A0000 //Base address of Sector 13, End address at 0x081BFFFF, 128 Kbytes  | 1792 Kbytes    (bank 2 sector 5 address)
#define FLASH_SECTOR_14_ADDR  0x081C0000 //Base address of Sector 14, End address at 0x081DFFFF, 128 Kbytes  | 1920 Kbytes    (bank 2 sector 6 address)
#define FLASH_SECTOR_15_ADDR  0x081E0000 //Base address of Sector 15, End address at 0x081EFFFF, 128 Kbytes  | 2048 Kbytes    (bank 2 sector 7 address)

//Flash Sector size
#define FLASH_SECTOR_SIZE_16KB   0x4000
#define FLASH_SECTOR_SIZE_64KB   (FLASH_SECTOR_SIZE_16KB * 4)
#define FLASH_SECTOR_SIZE_128KB  (FLASH_SECTOR_SIZE_16KB * 8)

//Flash Sectors size list (from STM32H7 datasheet)
#define FLASH_SECTOR_0_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 0 size)
#define FLASH_SECTOR_1_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 1 size)
#define FLASH_SECTOR_2_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 2 size)
#define FLASH_SECTOR_3_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 3 size)
#define FLASH_SECTOR_4_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 4 size)
#define FLASH_SECTOR_5_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 5 size)
#define FLASH_SECTOR_6_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 6 size)
#define FLASH_SECTOR_7_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 1 sector 7 size)
#define FLASH_SECTOR_8_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 0 size)
#define FLASH_SECTOR_9_SIZE   FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 1 size)
#define FLASH_SECTOR_10_SIZE  FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 2 size)
#define FLASH_SECTOR_11_SIZE  FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 3 size)
#define FLASH_SECTOR_12_SIZE  FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 4 size)
#define FLASH_SECTOR_13_SIZE  FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 5 size)
#define FLASH_SECTOR_14_SIZE  FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 6 size)
#define FLASH_SECTOR_15_SIZE  FLASH_SECTOR_SIZE_128KB    //(bank 2 sector 7 size)


//Flash Banks ID list
#define FLASH_BANK_1_ID FLASH_BANK_1
#define FLASH_BANK_2_ID FLASH_BANK_2

//Flash Banks start address list
#define FLASH_BANK_1_ADDR FLASH_SECTOR_0_ADDR
#define FLASH_BANK_2_ADDR FLASH_SECTOR_8_ADDR

//Flash Bank 1 size   
#define FLASH_BANK_1_SIZE (FLASH_SECTOR_0_SIZE +   \
   FLASH_SECTOR_1_SIZE  +                          \
   FLASH_SECTOR_2_SIZE  +                          \
   FLASH_SECTOR_3_SIZE  +                          \
   FLASH_SECTOR_4_SIZE  +                          \
   FLASH_SECTOR_5_SIZE  +                          \
   FLASH_SECTOR_6_SIZE  +                          \
   FLASH_SECTOR_7_SIZE)
//Flash Bank 2 size
#define FLASH_BANK_2_SIZE (FLASH_SECTOR_8_SIZE +   \
   FLASH_SECTOR_9_SIZE +                           \
   FLASH_SECTOR_10_SIZE +                          \
   FLASH_SECTOR_11_SIZE +                          \
   FLASH_SECTOR_12_SIZE +                          \
   FLASH_SECTOR_13_SIZE +                          \
   FLASH_SECTOR_14_SIZE +                          \
   FLASH_SECTOR_15_SIZE)

//Flash size
#define FLASH_TOTAL_SIZE (FLASH_BANK_1_SIZE + FLASH_BANK_2_SIZE)
//Flash start addr
#define FLASH_ADDR FLASH_SECTOR_0_ADDR


//STM32H7xx flash driver related functions
error_t flashInit(void);
error_t flashGetStartAddr(uint32_t *address);
size_t flashGetBankSize(void);
error_t flashWriteByte(uint32_t address, uint8_t byte);
error_t flashWriteHalfWord(uint32_t address, uint16_t halfword);
error_t flashWriteWord(uint32_t address, uint32_t word);
error_t flashSwapBank(void);
void systemReset(void);

#endif //!_STM32H7XX_FLASH_DRIVER_H
