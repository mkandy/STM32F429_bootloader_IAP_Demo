/**
 * @file iap_driver.h
 * @brief CycloneBOOT IAP driver interface
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

#ifndef _IAP_DRIVER_H
#define _IAP_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "iap_config.h"
#include "error.h"

//Flash driver version 1.0.1
#define FLASH_DRIVER_MAJOR_VERS  (0x01 << 16)   //Flash driver major version
#define FLASH_DRIVER_MINOR_VERS  (0x00 << 8)    //Flash driver minor version  
#define FLASH_DRIVER_PATCH_VERS  (0x01 << 0)    //Flash driver patch version
#define FLASH_DRIVER_VERSION     (FLASH_DRIVER_MAJOR_VERS || FLASH_DRIVER_MINOR_VERS || FLASH_DRIVER_PATCH_VERS)

#define FLASH_SECTOR_MAX_NUMBER     64    //Maximum number of flash sector
#define FLASH_BANK_MAX_NUMBER       2     //Maximum number of flash banks
#define FLASH_WRITE_TYPE_BYTE       0     //Byte 8-bits write type
#define FLASH_WRITE_TYPE_HALFWORD   1     //Half Word 16-bits write type 
#define FLASH_WRITE_TYPE_WORD       2     //Word 32-bits write type


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief Flash Sector description
 **/

typedef struct{
  uint32_t id;
  unsigned long size;    // Sector Size in Bytes
  uint32_t addr;  // Address of Sector
}FlashSectors;


/**
 * @brief Flash Bank description
 **/

typedef FlashSectors FlashBanks;


/**
 * @brief Common interface for flash memory
 **/

typedef struct
{
   uint32_t version;
   uint32_t addr;
   unsigned long size;
   uint8_t writeType;
   FlashBanks banks[FLASH_BANK_MAX_NUMBER]; 
   FlashSectors sectors[FLASH_SECTOR_MAX_NUMBER];
} FlashDriver;


//IAP driver common flash interface related functions 
extern error_t flashInit(void);
extern error_t flashGetStartAddr(uint32_t *address);
extern size_t flashGetBankSize(void);
extern error_t flashWriteByte(uint32_t address, uint8_t byte);
extern error_t flashWriteHalfWord(uint32_t address, uint16_t halfword);
extern error_t flashWriteWord(uint32_t address, uint32_t word);
extern error_t flashSwapBank(void);
extern void systemReset(void);

#endif //!_IAP_DRIVER_H
