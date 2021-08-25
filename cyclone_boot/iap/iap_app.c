/**
 * @file iap_app.c
 * @brief CycloneBOOT IAP User API
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
#include <stdlib.h>
#include "iap/iap_driver.h"
#include "iap/iap_app.h"
#include "iap/iap_check.h"
#include "iap/iap_cipher.h"
#include "iap/crc32.h"
#include "debug.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//IAP Application related private functions
error_t iapProcessAppHeader(IapContext *context);
error_t iapProcessAppData(IapContext *context);
error_t iapProcessAppCheck(IapContext* context);
error_t iapFlashWrite(IapContext *context, const void *data, size_t length);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
* @brief Initialize settings with default values
* @param[in,out] tructure that contains IAP settings
**/

void iapGetDefaultSettings(IapSettings *settings)
{
#if IAP_INTEGRITY_SUPPORT == ENABLED
   //IAP integrity setting
   settings->integrityAlgo = NULL;
#endif
 
#if IAP_AUTHENTICATION_SUPPORT == ENABLED   
   //IAP authentication settings
   settings->authAlgo = IAP_AUTH_NONE;
   settings->authHashAlgo = NULL;
   settings->authKey = NULL;
   settings->authKeyLen = 0;
#endif
   
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //IAP signature settings
   settings->signAlgo = IAP_SIGN_NONE;
   settings->signHashAlgo = NULL;
   settings->signKey = NULL;
   settings->signKeyLen = 0;
#endif

#if IAP_ENCRYPTION_SUPPORT == ENABLED
   //IAP cipher settings
   settings->cipherAlgo = NULL;
   settings->cipherMode = CIPHER_MODE_NULL;
   settings->cipherKey = NULL;
   settings->cipherKeyLen = 0;
#endif
}


/**
 * @brief Initialize IAP Application context
 * @param[in,out] context Pointer to the IAP Application context to be initialized
 * @return Status code
 **/

error_t iapInit(IapContext *context, const IapSettings *settings)
{
   error_t error;
   //FlashBankID fCurrentBankID;
   
   //Check Parameters validity
   if(context == NULL || settings == NULL)
      return ERROR_INVALID_PARAMETER;
   
   //Debug message
   TRACE_INFO("Initializing IAP...\r\n");
   
   //Check settings
#if IAP_INTEGRITY_SUPPORT == ENABLED
   //Check integrity settings
   if(settings->integrityAlgo == NULL)
      return ERROR_INVALID_PARAMETER;
#endif
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   //Check authentication settings
   if(settings->authAlgo == IAP_AUTH_NONE || settings->authHashAlgo == NULL || 
      settings->authKey == NULL || settings->authKeyLen == 0)
      return ERROR_INVALID_PARAMETER;
   //Force authentication algo to HMAC
   if(settings->authAlgo != IAP_AUTH_HMAC)
      return ERROR_UNSUPPORTED_ALGO;
#endif
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //Check signature settings
   if(settings->signAlgo == IAP_SIGN_NONE || settings->signHashAlgo == NULL ||
      settings->signKey == NULL || settings->signKeyLen == 0)
   //Force signature algo to RSA
   if(settings->signAlgo != IAP_SIGN_RSA)
      return ERROR_UNSUPPORTED_SIGNATURE_ALGO;
#endif
   
#if IAP_ENCRYPTION_SUPPORT == ENABLED
   //Check encryption settings
   if(settings->cipherAlgo == NULL || settings->cipherMode == CIPHER_MODE_NULL ||
       settings->cipherKey == NULL || settings->cipherKeyLen == 0)
   //Force cipher algo to AES
   if(settings->cipherAlgo != AES_CIPHER_ALGO)
      return ERROR_UNSUPPORTED_CIPHER_ALGO;
   //Force cipher mode to CBC
   if(settings->cipherMode != CIPHER_MODE_CBC)
      return ERROR_UNSUPPORTED_CIPHER_MODE;
#endif

   //Clear the IAP context
   memset(context, 0, sizeof(IapContext));

   //Save user settings
   context->settings = *settings;

   //Set default state
   context->state = IAP_STATE_IDLE;
   
   //Initialize device flash driver
   error = flashInit();
   //Is any error?
   if(error)
      return error;
   
   //Set flash bank start address
   error = flashGetStartAddr(&context->firmwareAddr);
   //Is any error?
   if(error)
      return error;
   
   //Initialize flash position to flash bank start addresse
   context->pos = context->firmwareAddr;
   
   //Initialize buffer position to buffer start addresse 
   context->bufferPos = context->buffer;
   //Initialize check data position to check data start addresse
   context->checkDataPos = context->checkData;

   //Return status code
   return NO_ERROR;
}


/**
 * @brief Write receive firmware in the unused flash bank.
 * @param[in,out] context Pointer to the IAP application context
 * @param[in] data Firmware chunck of data to be written in flash bank
 * @param[in] length Length of the firmware chunck of data to be written
 * @param[in] endOfData All data has been received
 * @return Status code
 **/

error_t iapUpdate(IapContext *context, const void *data, size_t length)
{
   error_t error;
   uint_t n;
   uint8_t *pData;

   //Check parameters validity
   if (context == NULL || data == NULL || length == 0)
      return ERROR_INVALID_PARAMETER;

   //Initialize variables
   n = 0;
   error = NO_ERROR;
   
   //Point to the beginning of the data
   pData = (uint8_t*)data;

   //IAP idle state?
   if(context->state == IAP_STATE_IDLE)
   {
      //Debug message
      TRACE_INFO("Processing firmware update...\r\n");
      
      //Reset flash position to flash bank start addresse
      context->pos = context->firmwareAddr;
      //Reset written data in flash counter
      context->written = 0;

      //Reset buffer position to buffer start addresse
      context->bufferPos = context->buffer;
      //Reset buffer length
      context->bufferLen = 0;

      //Reset check data position to check data start addresse
      context->checkDataPos = context->checkData;
      //Reset check data size and length
      context->checkDataLen = 0;
      context->checkDataSize = 0;
      //Reset check context
      memset(context->checkContext, 0, sizeof(context->checkContext));

      //Reset digest length
      context->digestLen = 0;
      //Reset digest
      memset(context->digest, 0, sizeof(context->digest));

#if IAP_ENCRYPTION_SUPPORT == ENABLED
      //Reset cipher iv length
      context->cipherIvLen = 0;
      //Reset cipher iv
      memset(context->cipherIv, 0, sizeof(context->cipherIv));
      //Reset cipher context
      memset(context->cipherContext, 0, sizeof(context->cipherContext));
#endif

      //Initialize IAP check crypto materials
      error = iapCheckInit(context);
      //Is any error ?
      if (error)
         return error;
      
#if IAP_ENCRYPTION_SUPPORT == ENABLED
      //Is application encrypted?
      if (context->settings.cipherAlgo != NULL)
      {
         //Initialize cipher engine
         error = iapCipherInit(context);
         //Is any error?
         if (error)
            return error;
      }
#endif
      
      //Change IAP state
      context->state = IAP_STATE_RECV_APP_HEADER;
   }

   //Process the incoming data
   while(length > 0)
   {
      //Still room in buffer?
      if(context->bufferLen < sizeof(context->buffer))
      {
         //Fill buffer with input data
         n = MIN(length, sizeof(context->buffer) - context->bufferLen);
         memcpy(context->bufferPos, pData, n);

         //Update buffer position and length
         context->bufferPos += n;
         context->bufferLen += n;
         
         //Update input data postion and length 
         pData += n;
         length -= n;
         
         //IAP receiving image header state?
         if(context->state == IAP_STATE_RECV_APP_HEADER)
         {
            //Process image header
            error = iapProcessAppHeader(context);
            //Is any error?
            if(error)
               break;
         }
         //IAP receiving image data state?
         else if(context->state == IAP_STATE_RECV_APP_DATA)
         {
            //Process image data
            error = iapProcessAppData(context);
            //Is any error?
            if(error)
               break;
         }
         //IAP receiving image check data state?
         else if(context->state == IAP_STATE_RECV_APP_CHECK)
         {
            //Process image check data
            error = iapProcessAppCheck(context);
            //Is any error?
            if(error)
               break;
         }
         else
         {
            //Wrong state
            error = ERROR_WRONG_STATE;
         }
      }
      else
      {
         //Debug message
         TRACE_ERROR("Buffer would overflow!\r\n");
         error = ERROR_BUFFER_OVERFLOW;
         break;
      }
   }

   //Is any error?
   if (error)
   {
      //Return to IAP idle state?
      if (context->state > IAP_STATE_IDLE && context->state < IAP_STATE_VALIDATE_APP)
      {
         //Change IAP state
         context->state = IAP_STATE_IDLE;
      }
   }

   //Return error code
   return error;
}


/**
 * @brief Finalize firmware update. It performs :
 *    - Firmware integrity or authentification or signature validation.
 *    - Flash bank memory swap setup
 * If validation is done successfully the device would be ready for reboot, 
 * if not an error will be raised.
 * The firmware validation can only be done if all the image 
 * (header, firmware data and check data) has been processed successfully, if not
 * an error will be raised.
 * @param[in,out] context Pointer to the IAP application context
 * @return Error code
 **/

error_t iapFinalize(IapContext* context)
{
   error_t error;

   //Check parameters validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;
   
   //Debug message
   TRACE_INFO("Finalizing firmware image update...\r\n");

   //Ready to verify firmware image validity?
   if (context->state == IAP_STATE_VALIDATE_APP)
   {
      //Verify firmware image validity (could integrity tag or 
      //authentification tag or signature)
      error = iapCheckVerify(context);
      //Is any error?
      if (error)
      {
         //Debug message
         TRACE_ERROR("Firmware image is invalid!\r\n");
         //Return to IAP idle state
         context->state = IAP_STATE_IDLE;
         //Return error code
         return error;
      }
      
      //Debug message
      TRACE_INFO("Firmware image is valid\r\n");
            
      //Swapping flash banks
      error = flashSwapBank();
      //Is any error?
      if (error)
      {
         //Debug message
         TRACE_ERROR("Flash bank swap failed!\r\n");
         return error;
      }

      //Change IAP state
      context->state = IAP_STATE_APP_REBOOT;
   }
   else
   {
      //Debug message
      TRACE_ERROR("Firmware image is not ready for verification!\r\n");
      return ERROR_WRONG_STATE;
   }

   //Valid firmware aaplication
   return NO_ERROR;
}


/**
 * @brief Reboot the device by triggering a system reset.
 * This reboot can only be performed if firmware validation has been done
 * succesfully. If not an error will be raised.
 * @param[in] context Pointer to the IAP context
 * @return Error code.
 **/

error_t iapReboot(IapContext* context)
{
   //Debug message
   TRACE_INFO("Rebooting device to finalize update...\r\n");
   
   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   if (context->state == IAP_STATE_APP_REBOOT)
   {
      //Reboot system to complete firmware update procedure
      systemReset();
   }
   else
   {
      //Wrong state
      return ERROR_WRONG_STATE;
   }

   //Successful process
   return NO_ERROR;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Process receiving of the image application header. 
 * Once fully received it will parse the image header and retrieve the length 
 * of the inside firmware.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code.
 **/

error_t iapProcessAppHeader(IapContext *context)
{
   IapAppHeader *header;
   uint_t n;
   
   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Initialize variable
   n = 0;

   //Is buffer full enough to contains IAP image header?
   if (context->bufferLen >= sizeof(IapAppHeader))
   {
      //Debug message
      TRACE_DEBUG("Processing firmware image header...\r\n");
      
      //Point to the IAP application header
      header = (IapAppHeader*)context->buffer;

      //Would firmware overcome the flash bank size?
      if (header->appSize > flashGetBankSize())
      {
         //Debug message
         TRACE_ERROR("The firmware would overcome the flash bank size!\r\n");
         return ERROR_BUFFER_OVERFLOW;
      }

      //Save application firmware length
      context->firmwareLength = header->appSize;

      //Remove header from buffer
      n = context->bufferLen - sizeof(IapAppHeader);
      memcpy(context->buffer, context->buffer + sizeof(IapAppHeader), n);
      context->bufferPos -= sizeof(IapAppHeader);
      context->bufferLen -= sizeof(IapAppHeader);

      //Change IAP state
      context->state = IAP_STATE_RECV_APP_DATA;
   }

   //Successful process
   return NO_ERROR;
}


/**
 * @brief Process receiving of the firmware cipher initialization vector (iv) in 
 * case the firmware has been encrypted. Once fully received it will be save for
 * later firmware dencryption.
 * of the inside firmware.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code.
 **/

error_t iapProcessAppCipherIv(IapContext *context)
{
#if IAP_ENCRYPTION_SUPPORT == ENABLED
   uint_t n;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Initialize variable
   n = 0;

   //Is buffer full enough to contains IAP image header?
   if (context->bufferLen >= context->settings.cipherKeyLen)
   {
      //Debug message
      TRACE_DEBUG("Processing firmware image cipher initialization vector...\r\n");
      
      //Set application cipher intialization vector length
      context->cipherIvLen = context->settings.cipherKeyLen;
      //Save application cipher intialization vector
      memcpy(context->cipherIv, context->buffer, context->cipherIvLen);

      //Remove processed data (cipher iv) from buffer
      n = context->bufferLen - context->cipherIvLen;
      memcpy(context->buffer, context->buffer + context->cipherIvLen, n);
      context->bufferPos -= context->cipherIvLen;
      context->bufferLen -= context->cipherIvLen;
   }

   //Successful process
   return NO_ERROR;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Process receiving of the firmware data bloc by bloc. 
 * If firmware is encrypted the data will first be deencrypted then depending 
 * of the user settings integrity or authentification or signature hash 
 * computation will be done one each data blocs. 
 * Finally each data bloc will write into the flash 
 * bank.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code.
 **/

error_t iapProcessAppData(IapContext *context)
{
   error_t error;
   uint_t dataLength;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

#if IAP_ENCRYPTION_SUPPORT == ENABLED
   //Receiving image application header?
   if (context->settings.cipherAlgo != NULL && context->cipherIvLen == 0)
   {
      //Process application cipher initialisation vector here
      error = iapProcessAppCipherIv(context);
      //Is any error?
      if (error)
         return error;
   }
   //Receiving image firmware data?
   else
   {
#else
   //Receiving image firmware data?
   if(1)
   {
#endif
      //Is buffer full or full enought to contain last application data?
      if ((context->bufferLen == sizeof(context->buffer)) ||
         (context->written + context->bufferLen >= context->firmwareLength))
      {
         //We must not process more data than the firmware length
         dataLength = MIN(context->bufferLen, context->firmwareLength - context->written);

         //Update application check computation tag (could be integrity tag or
         //authentification tag or hash signature tag) 
         error = iapCheckUpdateTag(context, context->buffer, dataLength);
         //Is any error?
         if (error)
            return error;
         
#if IAP_ENCRYPTION_SUPPORT == ENABLED
         //Is application is encrypted?
         if (context->settings.cipherAlgo != NULL)
         {
            //Decrypt application data
            error = iapCipherDecodeData(context, context->buffer, dataLength);
            //Is any error?
            if (error)
               return error;
         }
#endif
         
         //Write application data in flash bank
         error = iapFlashWrite(context, context->buffer, dataLength);
         //Is any error?
         if (error)
            return error;

         //Is buffer contains remaining data?
         if (context->bufferLen != dataLength)
         {
            //Discard already processed data
            memset(context->buffer, 0, dataLength);

            //Put remaining data at buffer start
            memcpy(context->buffer, context->buffer + dataLength, 
               context->bufferLen - dataLength);
            //Update buffer position and length
            context->bufferPos = context->buffer + (context->bufferLen - dataLength);
            context->bufferLen = context->bufferLen - dataLength;
         }
         else
         {
            //Reset buffer
            memset(context->buffer, 0, sizeof(context->buffer));
            context->bufferPos = context->buffer;
            context->bufferLen = 0;
         }

         //Is application data all received?
         if (context->written == context->firmwareLength)
         {
            //Finalize application check comptutation tag (could be integrity 
            //tag or authentification tag or hash signature tag)
            error = iapCheckFinalizeTag(context);
            //Is any error?
            if (error)
               return error;

            //Change IAP state
            context->state = IAP_STATE_RECV_APP_CHECK;

            //Still data to process?
            if (context->bufferLen > 0)
            {
               //Process IAP check app data
               error = iapProcessAppCheck(context);
               //Is any error?
               if (error)
                  return error;
            }
         }
      }
   }
      
   //Successfull process
   return NO_ERROR;
}


/**
 * @brief Process receiving of the image check data. Depending of the user 
 * settings it could be the integrity or the authentification tag or signature
 * of the image firmware data. Once fully received it will saved for further 
 * firmware validity verification.
 * @param[in,out] context Pointer to the IAP context
 * @return Error code.
 **/

error_t iapProcessAppCheck(IapContext *context)
{
   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Is buffer full enough to contains IAP image check data?
   //(could be integrity/authentification tag or signature)
   if (context->checkDataLen + context->bufferLen <= context->checkDataSize)
   {
      //Save image check data block
      memcpy(context->checkDataPos, context->buffer, context->bufferLen);
      context->checkDataPos += context->bufferLen;
      context->checkDataLen += context->bufferLen;

      //Reset buffer
      memset(context->buffer, 0, sizeof(context->buffer));
      context->bufferPos = context->buffer;
      context->bufferLen = 0;

      //Is image check data fully received?
      if (context->checkDataLen == context->checkDataSize)
      {
         //Change IAP state
         context->state = IAP_STATE_VALIDATE_APP;
      }
   }
   else
   {
      //Debug message
      TRACE_ERROR("Image check data is bigger than expected!\r\n");
      return ERROR_BUFFER_OVERFLOW;
   }

   //Successful process
   return NO_ERROR;
}


/**
 * @brief Write firmware data in flash bank starting at the beginning of the bank.
 * Each time this function is called the position in bank and the number of data 
 * written is updated. 
 * In case writting data would overcome the flash bank an error would be raised.
 * @param[in,out] context Pointer to the IAP context
 * @param[in] data Firmware data bloc to be written in flash
 * @param[in] length Length of the firmware data bloc to be written
 * @return Error code
 **/

error_t iapFlashWrite(IapContext *context, const void *data, size_t length)
{
   error_t error;
   size_t i;
   size_t nbWords;
   uint32_t *p;

   //Initialize variables
   error = NO_ERROR;
   
   //Check parameters validity
   if(context == NULL || data == NULL || length % sizeof(uint32_t) != 0)
      return ERROR_INVALID_PARAMETER;
   
   //Debug message
   TRACE_DEBUG("Writting data (%d bytes) in flash at 0x%08X...\r\n", length, context->pos);

   //Retrieve the number of words inside data buffer to be written
   nbWords = length / sizeof(uint32_t);
   //Point to the first word
   p = (uint32_t*) data;

   //Loop through data words by words
   for(i = 0; i < nbWords && error == NO_ERROR; i++)
   {
      //Is remaining bytes bigger than a word (4 bytes)?
      if((length - i*sizeof(uint32_t)) >= sizeof(uint32_t))
      {
         //Program the 32-bit word to Flash memory
         error = flashWriteWord((uint32_t) (context->pos + i*sizeof(uint32_t)), p[i]);
         //Is any error?
         if(error)
         {
            //Debug message
            TRACE_ERROR("Failed to write in flash at 0x%08X!\r\n", (uint32_t) (context->pos + i*sizeof(uint32_t)));
            return error;
         }
      }
   }
   
   //Update flash bank position
   context->pos += length;
   //Update written firmware data
   context->written += length;

   //Return status code
   return NO_ERROR;
}
