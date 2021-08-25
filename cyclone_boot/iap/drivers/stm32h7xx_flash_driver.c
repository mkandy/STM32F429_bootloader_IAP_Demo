/**
 * @file stm32h7xx_flash_driver.c
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

//Switch to the appropriate trace level
#define TRACE_LEVEL IAP_TRACE_LEVEL

//Dependencies
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "iap/drivers/stm32h7xx_flash_driver.h"
#include "debug.h"

#define getFB_IDX(BANK_ID) !(BANK_ID == FLASH_BANK_1_ID)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//STM32H7xx flash driver private related functions
int32_t flashGetSector(uint32_t address);
error_t flashGetCurrentBank(uint8_t *fBankID);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief STM32H767ZI Flash driver
 **/

const FlashDriver flashDriver =
{
   FLASH_DRIVER_VERSION,
   FLASH_ADDR,
   FLASH_TOTAL_SIZE,
   FLASH_WRITE_TYPE_WORD,
   {
      {FLASH_BANK_1_ID, FLASH_BANK_1_SIZE, FLASH_BANK_1_ADDR},
      {FLASH_BANK_2_ID, FLASH_BANK_2_SIZE, FLASH_BANK_2_ADDR}
   },
   {
      {FLASH_SECTOR_0,  FLASH_SECTOR_0_SIZE,   FLASH_SECTOR_0_ADDR},
      {FLASH_SECTOR_1,  FLASH_SECTOR_1_SIZE,   FLASH_SECTOR_1_ADDR},
      {FLASH_SECTOR_2,  FLASH_SECTOR_2_SIZE,   FLASH_SECTOR_2_ADDR},
      {FLASH_SECTOR_3,  FLASH_SECTOR_3_SIZE,   FLASH_SECTOR_3_ADDR},
      {FLASH_SECTOR_4,  FLASH_SECTOR_4_SIZE,   FLASH_SECTOR_4_ADDR},
      {FLASH_SECTOR_5,  FLASH_SECTOR_5_SIZE,   FLASH_SECTOR_5_ADDR},
      {FLASH_SECTOR_6,  FLASH_SECTOR_6_SIZE,   FLASH_SECTOR_6_ADDR},
      {FLASH_SECTOR_7,  FLASH_SECTOR_7_SIZE,   FLASH_SECTOR_7_ADDR},
      {FLASH_SECTOR_0,  FLASH_SECTOR_8_SIZE,   FLASH_SECTOR_8_ADDR},
      {FLASH_SECTOR_1,  FLASH_SECTOR_9_SIZE,   FLASH_SECTOR_9_ADDR},
      {FLASH_SECTOR_2,  FLASH_SECTOR_10_SIZE,  FLASH_SECTOR_10_ADDR},
      {FLASH_SECTOR_3,  FLASH_SECTOR_11_SIZE,  FLASH_SECTOR_11_ADDR},
      {FLASH_SECTOR_4,  FLASH_SECTOR_12_SIZE,  FLASH_SECTOR_12_ADDR},
      {FLASH_SECTOR_5,  FLASH_SECTOR_13_SIZE,  FLASH_SECTOR_13_ADDR},
      {FLASH_SECTOR_6,  FLASH_SECTOR_14_SIZE,  FLASH_SECTOR_14_ADDR},
      {FLASH_SECTOR_7,  FLASH_SECTOR_15_SIZE,  FLASH_SECTOR_15_ADDR}
   }
};


/**
* @brief Initialize Flash.
* @return Error code
**/

error_t flashInit(void)
{
   error_t error;
   uint8_t fCurrentBankID;
   
   //Debug message
   TRACE_INFO("Initializing STM32H7xx flash driver...\r\n");
   
   //Initialize FLASH flags 
   //(Patch to fix stm32 hal library wrong initial flash flags issue)
   FLASH_WaitForLastOperation((uint32_t)50000U, FLASH_BANK_1_ID);
   FLASH_WaitForLastOperation((uint32_t)50000U, FLASH_BANK_2_ID);
   
   //Get current used flash bank
   error = flashGetCurrentBank(&fCurrentBankID);
   //Is any error?
   if (error)
	  return error;
  
   //Debug message
   if(fCurrentBankID == FLASH_BANK_1_ID)
      TRACE_INFO("Running on flash bank 1...\r\n");
   else
      TRACE_INFO("Running on flash bank 2...\r\n");
   
   //Successfull process
   return NO_ERROR;
}


/**
* @brief Get Flash start address.
* @return Error code
**/

error_t flashGetStartAddr(uint32_t *startAddr)
{  
   //Check parameter validity
   if(startAddr == NULL)
      return ERROR_INVALID_PARAMETER;
   
   //Set flash start address (always second bank)
   *startAddr = FLASH_BANK_2_ADDR;
   
   //Successful process
   return NO_ERROR;
}


/**
* @brief Get Flash bank size.
* @return Size of the flash bank
**/

size_t flashGetBankSize(void)
{     
   //Return flash bank size
   return flashDriver.banks[getFB_IDX(FLASH_BANK_1_ID)].size;
}


/**
* @brief Write a byte into Flash memory at the given address.
* @param[in] address Flash address
* @param[in] byte Byte to be written
* @return Error code
**/

error_t flashWriteByte(uint32_t address, uint8_t byte)
{
   FlashBanks *banks;
   
   //Point to flash driver banks
   banks = (FlashBanks*)&flashDriver.banks;
   
   //Check address validty
   if(address < banks[getFB_IDX(FLASH_BANK_2_ID)].addr || 
      address > (banks[getFB_IDX(FLASH_BANK_2_ID)].addr + 
      banks[getFB_IDX(FLASH_BANK_2_ID)].size - sizeof(uint32_t)))
      return ERROR_INVALID_PARAMETER;
   
   //TODO write byte in flash here
   
   return ERROR_NOT_IMPLEMENTED;
}


/**
* @brief Write a half word into Flash memory at the given address.
* @param[in] address Flash address
* @param[in] halfWord 16-bit Half Word to be written
* @return Error code
**/

error_t flashWriteHalfWord(uint32_t address, uint16_t halfWord)
{  
   FlashBanks *banks;
   
   //Point to flash driver banks
   banks = (FlashBanks*)&flashDriver.banks;
   
   //Check address validty
   if(address < banks[getFB_IDX(FLASH_BANK_2_ID)].addr || 
      address > (banks[getFB_IDX(FLASH_BANK_2_ID)].addr + 
      banks[getFB_IDX(FLASH_BANK_2_ID)].size - sizeof(uint32_t)))
      return ERROR_INVALID_PARAMETER;
   
   //TODO write half word in flash here
   
   return ERROR_NOT_IMPLEMENTED;
}


/**
* @brief Write a word into Flash memory at the given address.
* @param[in] address Flash address
* @param[in] word 32-bit Word to be written
* @return Error code
**/

error_t flashWriteWord(uint32_t address, uint32_t word)
{
   error_t error;
   int32_t sector;
   uint32_t sectorError;
   HAL_StatusTypeDef status;
   FLASH_EraseInitTypeDef EraseInitStruct;
   uint8_t fCurrentBankID;
   uint8_t bankToBeUsed;
   FlashBanks *banks;
   
   //Point to flash driver banks
   banks = (FlashBanks*)&flashDriver.banks;
   
   //Check address validty
   if(address < banks[getFB_IDX(FLASH_BANK_2_ID)].addr || 
      address > (banks[getFB_IDX(FLASH_BANK_2_ID)].addr + 
      banks[getFB_IDX(FLASH_BANK_2_ID)].size - sizeof(uint32_t)))
      return ERROR_INVALID_PARAMETER;
   
   //Retrieve current Flash sector
   sector = flashGetSector(address);

   //Check whether the address match the beginning of a Flash sector.
   //If this is the case then the flash sector must be erased before any write operation
   if(sector >= 0)
   {
      //Get current used flash bank
      error = flashGetCurrentBank(&fCurrentBankID);
      //Is any error?
      if (error)
         return error;
      
      //Set bank to be used for programming
      bankToBeUsed = (fCurrentBankID == FLASH_BANK_1_ID)? FLASH_BANK_2_ID : FLASH_BANK_1_ID;

      ////Running in flash bank2?
      //if(fCurrentBankID == FLASH_BANK_2_ID)   
      //{
      //   if(sector < FLASH_SECTOR_12)
      //      sector += 12;
      //   else
      //      sector -= 12;
      //}

      //Debug message
      TRACE_DEBUG("Erasing Flash sector %" PRIu32 "...\r\n", sector);
      
      //Start of exception handling block
      do
      {
         //Allow access to Flash control registers and user False
         status = HAL_FLASH_Unlock();
         //Is any error?
         if (status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Control Register unlock failed!\r\n");
            break;
         }
         
         //Initialize FLASH flags
         status = FLASH_WaitForLastOperation((uint32_t)50000U, bankToBeUsed);
         //Is any error?
         if(status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Initialize FLASH flags...\r\n");
         }
         
         //Set flash erase parameters
         EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
         EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
         EraseInitStruct.Banks = bankToBeUsed;
         EraseInitStruct.Sector = sector;
         EraseInitStruct.NbSectors = 1;
         
         //Erase the specified Flash sector
         status = HAL_FLASHEx_Erase(&EraseInitStruct, &sectorError);
         //Is any error?
         if(status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Failed to erase flash sector %d, error = 0x%08X!\r\n", sector, sectorError);
         }
         
         //Disable the Flash option control register access (recommended to protect 
         //the option Bytes against possible unwanted operations)
         if(HAL_FLASH_Lock() != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Control Register lock failed!\r\n");
         }
      }while(0);
   }
   else
   {
      //Erase is not needed
      status = HAL_OK;
   }

   //Check status code
   if(status == HAL_OK)
   {
      //Start of exception handling block
      do
      {
         //Allow access to Flash control registers and user False
         status = HAL_FLASH_Unlock();
         //Is any error?
         if (status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Control Register unlock failed!\r\n");
            break;
         }
         
         //Program the 32-bit word to Flash memory
         status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, address, word);
         //Is any error?
         if(status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Failed to write word \"0x%08X\" at 0x%08X!\r\n", word, address);
         }
         
         //Disable the Flash option control register access (recommended to protect 
         //the option Bytes against possible unwanted operations)
         if(HAL_FLASH_Lock() != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Control Register lock failed!\r\n");
         }
      }while(0);
   }

   //Return status code
   return (status == HAL_OK) ? NO_ERROR : ERROR_WRITE_FAILED;
}


/**
 * @brief Performs a flash bank swap according to the current flash bank ID.
 * If current flash bank ID match the 1st bank then it will swap on the 2nd flash bank.
 * If current flash bank ID match the 2nd bank then it will swap on the 1st flash bank.
 * @return Error code
 **/

error_t flashSwapBank(void)
{
   error_t error;
   FLASH_OBProgramInitTypeDef    OBInit;
   HAL_StatusTypeDef status;
   uint8_t fCurrentBankID;

   //Debug message
   TRACE_INFO("Swaping device flash bank...\r\n");
   
   //Get current flash bank ID
   error = flashGetCurrentBank(&fCurrentBankID);
   //Is any error?
   if(error)
      return error;
   
   //Start of exception handling block
   do
   {
      //Allow access to Flash control registers and user False
      status = HAL_FLASH_Unlock();
      //Is any error?
      if (status != HAL_OK)
      {
         //Debug message
         TRACE_ERROR("Flash Control Register unlock failed!\r\n");
         break;
      }

      //Start of exception handling block
      do
      {
         //Allow Access to option bytes sector
         status = HAL_FLASH_OB_Unlock();
         //Is any error?
         if (status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Option Control Registers unlock failed!\r\n");
            break;
         }

         //Get the Dual boot configuration status
         HAL_FLASHEx_OBGetConfig(&OBInit);

         //Swap in flash bank 2
         if(fCurrentBankID == FLASH_BANK_1_ID)
         {
            //Debug message
            TRACE_DEBUG("Swaping from flask bank 1 to flash bank 2...\r\n");
            
            //Configure option bytes to swap on flash bank 2
            OBInit.OptionType = OPTIONBYTE_USER;
            OBInit.USERType   = OB_USER_SWAP_BANK;
            OBInit.USERConfig = OB_SWAP_BANK_ENABLE;
         }
         //Swap in flash bank 1
         else
         {
            //Debug message
            TRACE_DEBUG("Swaping from flask bank 2 to flash bank 1...\r\n");
            
            //Configure option bytes to swap on flash bank 1
            OBInit.OptionType = OPTIONBYTE_USER;
            OBInit.USERType   = OB_USER_SWAP_BANK;
            OBInit.USERConfig = OB_SWAP_BANK_DISABLE;
         }

         //Start of exception handling block
         do
         {  
            //Start the Option Bytes programming process
            status = HAL_FLASHEx_OBProgram(&OBInit);
            //Is any error?
            if (status != HAL_OK)
            {
               //Debug message
               TRACE_ERROR("Option Bytes programming process failed!\r\n");
               break;
            }
            
            //Launch the option byte loading
            status = HAL_FLASH_OB_Launch();
            //Is any error?
            if (status != HAL_OK)
            {
               //Debug message
               TRACE_ERROR("Option byte loading failed!\r\n");
            }
            
            //invalidate snstruction cache
            SCB_InvalidateICache();
         }while(0);

         //Prevent Access to option bytes sector
         if(HAL_FLASH_OB_Lock() != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Option Control Register lock failed!\r\n");
         }
      }while(0);

      //Disable the Flash option control register access (recommended to protect 
      //the option Bytes against possible unwanted operations)
      if(HAL_FLASH_Lock() != HAL_OK)
      {
         //Debug message
         TRACE_ERROR("Flash Control Register lock failed!\r\n");
      }
   }while(0);
       
   
   //Return status code
   return (status == HAL_OK)? NO_ERROR : ERROR_FAILURE;
}


/**
 * @brief Initiates a system reset request to reset the MCU.
 **/

void systemReset(void)
{
   //Initiates a system reset request to reset the MCU
   HAL_NVIC_SystemReset();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief Retrieve the Flash sector that matches the specified address
 * @param[in] address 32-bit address
 * @return Flash sector
 **/

int32_t flashGetSector(uint32_t address)
{
   int32_t sectorID;
   uint_t i;
   FlashSectors *sectors;
   
   //Initialize sector ID
   sectorID = -1;
   
   //Point to flash driver sectors
   sectors = (FlashSectors*)&flashDriver.sectors;
   
   //Loop through flash driver sectors list
   for(i = 0; i < FLASH_SECTORS_NUMBER; i++)
   {
      //Is address match the start address of a sector?
      if(address == sectors[i].addr)
      {
         //Save sector ID
         sectorID = sectors[i].id;
         break;
      }
   }
      
   //Return sector ID
   return sectorID;
}


/**
 * @brief Get current used flash bank (FLASH_BANK_1_ID or FLASH_BANK_2_ID).
 * @param[out] fBankID Pointeur to the flash bank ID to be retrieved
 * @return Status code
 **/

error_t flashGetCurrentBank(uint8_t *fBankID)
{   
   FLASH_OBProgramInitTypeDef OBInit;
   HAL_StatusTypeDef status;

   //Check parameters validity
   if(fBankID == NULL)
      return ERROR_INVALID_PARAMETER;
   
   //Start of exception handling block
   do
   {
      //Allow access to Flash control registers and user False
      status = HAL_FLASH_Unlock();
      //Is any error?
      if (status != HAL_OK)
      {
         //Debug message
         TRACE_ERROR("Flash Control Register unlock failed!\r\n");
         break;
      }

      //Start of exception handling block
      do
      {
         //Allow Access to option bytes sector
         status = HAL_FLASH_OB_Unlock();
         //Is any error?
         if (status != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Option Control Registers unlock failed!\r\n");
            break;
         }

         //Get the Dual boot configuration status
         HAL_FLASHEx_OBGetConfig(&OBInit);

         //Running in flash bank 1
         if ((OBInit.USERConfig & OB_SWAP_BANK_ENABLE) == OB_SWAP_BANK_DISABLE)
         {
            *fBankID = FLASH_BANK_1_ID;
         }
         //Running in flash bank 2
         else
         {
            *fBankID = FLASH_BANK_2_ID;
         }

         //Prevent Access to option bytes sector
         if(HAL_FLASH_OB_Lock() != HAL_OK)
         {
            //Debug message
            TRACE_ERROR("Flash Option Control Register lock failed!\r\n");
         }
      }while(0);

      //Disable the Flash option control register access (recommended to protect 
      //the option Bytes against possible unwanted operations)
      if(HAL_FLASH_Lock() != HAL_OK)
      {
         //Debug message
         TRACE_ERROR("Flash Control Register lock failed!\r\n");
      }
   }while(0);
   
   //Return status code
   return (status == HAL_OK)? NO_ERROR: ERROR_FAILURE;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief  FLASH operation error interrupt callback
 * @param  ReturnValue The value saved in this parameter depends on the ongoing procedure
 *                 - Sectors Erase: Sector which has been erased (if 0xFFFFFFFF, it means that 
 *                                  all the selected sectors have been erased)
 *                 - Program      : Address which was selected for data program
 *                 - Mass Erase   : No return value expected
 * @retval None
 **/
void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
{
   static uint_t cnt = 0;
   cnt++;
   TRACE_INFO("flash error callback counter: %d\r\n", cnt);
}
