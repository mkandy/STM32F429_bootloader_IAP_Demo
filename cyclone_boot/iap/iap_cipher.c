/**
 * @file iap_cipher.c
 * @brief CycloneBOOT IAP image decoding handling
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
#include "iap_app.h"
#include "cipher_mode/cbc.h"
#include "debug.h"


/**
 * @brief Initialize cipher material for further firmware data decryption.
 * @param[in,out] context Point to the IAP context.
 * @return Error code
 **/

error_t iapCipherInit(IapContext *context)
{
#if IAP_ENCRYPTION_SUPPORT == ENABLED
   IapSettings* settings;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = &context->settings;

   //Check user cipher settings
   if (settings->cipherAlgo == NULL || settings->cipherKey == NULL ||
      settings->cipherKeyLen == 0)
      return ERROR_INVALID_VALUE;

   //Initialize cipher algo context
   settings->cipherAlgo->init(context->cipherContext, (const uint8_t*)settings->cipherKey,
      settings->cipherKeyLen);

   //Successful process
   return NO_ERROR;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Decode encrypted firmware data bloc. 
 * The input data encrypted bloc would also be the output decrypted data bloc.
 * @param[in,out] context Point to the IAP context.
 * @param[in,out] data Firmware data bloc to be decrypted
 * @return Error code
 **/

error_t iapCipherDecodeData(IapContext *context, uint8_t *data, size_t length)
{
#if IAP_ENCRYPTION_SUPPORT == ENABLED
   IapSettings* settings;

   //Check parameters validity
   if (context == NULL || data == NULL || length == 0)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = (IapSettings*)&context->settings;

   //Check cipher mode
   if (settings->cipherMode == CIPHER_MODE_NULL)
      return ERROR_INVALID_VALUE;


   //Is cbc cipher mode ?
   if (settings->cipherMode == CIPHER_MODE_CBC)
   {
      //Decipher data with CBC mode
      return cbcDecrypt(settings->cipherAlgo, context->cipherContext,
         context->cipherIv, data, data, length);
   }
   else
   {
      //Debug message
      TRACE_ERROR("Cipher mode not supported!\r\n");
      return ERROR_NOT_IMPLEMENTED;
   }
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}
