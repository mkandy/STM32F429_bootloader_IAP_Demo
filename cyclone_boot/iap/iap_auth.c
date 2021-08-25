/**
 * @file iap_auth.c
 * @brief CycloneBOOT IAP image authentication handling
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
#include "iap/iap_auth.h"
#include "debug.h"


/**
 * @brief Initialize authentification material for further authentification tag
 * computation on received firmware data and verification against 
 * received image authentification tag.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapAuthInit(IapContext *context)
{
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   error_t error;
   IapSettings* settings;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = &context->settings;
   //Initialize status code
   error = NO_ERROR;

   //Check user settings
   if (settings->authHashAlgo == NULL || settings->authAlgo == IAP_AUTH_NONE)
      return ERROR_INVALID_VALUE;

   //Is hmac authentification algo?
   if (settings->authAlgo == IAP_AUTH_HMAC)
   {
      //Initialize authentification hmac context
      hmacInit((HmacContext*)context->checkContext, settings->authHashAlgo, 
         settings->authKey, settings->authKeyLen);

      //Set digest length
      context->digestLen = ((HmacContext*)context->checkContext)->hash->digestSize;

      //Set check data (authentification tag) size
      context->checkDataSize = ((HmacContext*)context->checkContext)->hash->digestSize;
   }
   else
   {
      //Debug message
      TRACE_ERROR("Authentification algorithm not supported!\r\n");
      error = ERROR_NOT_IMPLEMENTED;
   }

   //Return status code
   return error;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Update authentification tag computation on received firmware data bloc.
 * @param[in,out] context Point to the IAP context.
 * @param[in] data Firmware data bloc to process.
 * @param[in] length Length of the firmware data bloc to process.
 * @return Error code
 **/

error_t iapAuthUpdateTag(IapContext *context, const uint8_t *data, size_t length)
{
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   error_t error;
   IapSettings* settings;

   //Check parameter validity
   if (context == NULL || data == NULL || length == 0)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = &context->settings;
   //Initialize status code
   error = NO_ERROR;

   //Check authentification hash algo
   if (settings->authAlgo == IAP_AUTH_NONE)
      return ERROR_INVALID_VALUE;
   
   //Is hmac authentification algo?
   if (settings->authAlgo == IAP_AUTH_HMAC)
   {
      //Update uahtentification hmac tag
      hmacUpdate((HmacContext*)context->checkContext, data, length);
   }
   else
   {
      //Debug message
      TRACE_ERROR("Authentification algorithm not supported!\r\n");
      error = ERROR_NOT_IMPLEMENTED;
   }

   //Return status code
   return error;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Finalize authentification tag computation on received firmware data.
 * @param[in,out] context Point to the IAP context.
 * @return Error code
 **/

error_t iapAuthFinalizeTag(IapContext *context)
{
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   error_t error;
   IapSettings* settings;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = &context->settings;
   //Initialize status code
   error = NO_ERROR;

   //Check authentification hash algo
   if (settings->authAlgo == IAP_AUTH_NONE)
      return ERROR_INVALID_VALUE;

   //Is hmac authentification algo?
   if (settings->authAlgo == IAP_AUTH_HMAC)
   {
      hmacFinal((HmacContext*)context->checkContext, context->digest);
   }
   else
   {
      //Debug message
      TRACE_ERROR("Authentification algorithm not supported!\r\n");
      error = ERROR_NOT_IMPLEMENTED;
   }

   //Return status code
   return error;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}
