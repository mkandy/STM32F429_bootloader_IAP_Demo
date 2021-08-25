/**
 * @file iap_check.c
 * @brief CycloneBOOT IAP image verification handling
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
#include "iap/iap_check.h"
#include "iap/iap_auth.h"
#include "iap/iap_sign.h"
#include "debug.h"


/**
 * @brief Initialize all crypto materials that will be used to compute 
 * check data tag and verify it against the one received in the image.
 * Depending the user settings the check data tag could be an integrity tag or 
 * an authentification tag or a signature hash tag.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapCheckInit(IapContext *context)
{
   error_t error;
   IapSettings *settings;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Pont to the IAP user settings
   settings = (IapSettings*)&context->settings;
   //Initialize status code
   error = NO_ERROR;

#if IAP_INTEGRITY_SUPPORT == ENABLED
   //Is user require integrity check?
   if (settings->integrityAlgo != NULL)
   {
      //Initialize integrity algo context
      settings->integrityAlgo->init(context->checkContext);

      //Set digest length
      context->digestLen = settings->integrityAlgo->digestSize;

      //Set check data (integrity tag) size
      context->checkDataSize = settings->integrityAlgo->digestSize;
   }
   else
#endif
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   //Is user require authentification check?
   if (settings->authAlgo != IAP_AUTH_NONE)
   {
      //Initialize authentification according to user settings
      error = iapAuthInit(context);
   }
   else
#endif
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //Is user require signature check?
   if (settings->signAlgo != IAP_SIGN_NONE)
   {
      //Initialize signature according to user settings 
      error = iapSignInit(context);
   }
   else
#endif
   {
      //Debug message
      TRACE_ERROR("Missing image check method (integrity or authentification or signature!\r\n");
      error = ERROR_ABORTED;
   }

   //Return status code
   return error;
}


/**
 * @brief Update check comptutation tag that will be used to verify firmware 
 * image valdity. Depending of the user settings the check computation tag 
 * can be an integrity tag or authentification tag or signature hash tag.
 * @param[in,out] context Point to the IAP context
 * @param[in] data Firmware data bloc to be processed
 * @param[in] length Length of the firmware data bloc to be processed
 * @return Error code
 **/

error_t iapCheckUpdateTag(IapContext *context, const uint8_t *data, size_t length)
{
   error_t error;
   IapSettings *settings;

   //Check parameters validity
   if (context == NULL || data == NULL || length == 0)
      return ERROR_INVALID_PARAMETER;

   //Point to user settings
   settings = (IapSettings*)&context->settings;
   //Initialize status code
   error = NO_ERROR;

#if IAP_INTEGRITY_SUPPORT == ENABLED
   //Is user require integrity check?
   if (settings->integrityAlgo != NULL)
   {
      //Update integrity hash tag
      settings->integrityAlgo->update(context->checkContext, data, length);
   }
   else
#endif
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   //Is user require authentification check?
   if (settings->authAlgo != IAP_AUTH_NONE)
   {
      //Update authentification hash tag
      error = iapAuthUpdateTag(context, data, length);
   }
   else
#endif
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //Is user require signature check?
   if (settings->signAlgo != IAP_SIGN_NONE)
   {
      //Update signature hash tag
      settings->signHashAlgo->update(context->checkContext, data, length);
   }
   else
#endif
   {
      //Debug message
      TRACE_ERROR("Missing image check method (integrity or authentification or signature!\r\n");
      error = ERROR_ABORTED;
   }

   //Return status code
   return error;
}


/**
 * @brief Finalize check comptutation tag on received firmware data.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapCheckFinalizeTag(IapContext *context)
{
   error_t error;
   IapSettings *settings;
   
   //Check parameters validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to user settings
   settings = (IapSettings*)&context->settings;
   //Initialize status code
   error = NO_ERROR;

#if IAP_INTEGRITY_SUPPORT == ENABLED
   //Is user require integrity check?
   if (settings->integrityAlgo != NULL)
   {
      //Debug message
      TRACE_DEBUG("Finalizing firmware integrity tag computation...\r\n");
      
      //Update integrity hash tag
      settings->integrityAlgo->final(context->checkContext, context->digest);
   }
   else
#endif
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   //Is user require authentification check?
   if (settings->authAlgo != IAP_AUTH_NONE)
   {
      //Debug message
      TRACE_DEBUG("Finalizing firmware authentification tag computation...\r\n");
      
      //Update authentification hash tag
      error = iapAuthFinalizeTag(context);
   }
   else
#endif
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //Is user require signature check?
   if (settings->signAlgo != IAP_SIGN_NONE)
   {
      //Debug message
      TRACE_DEBUG("Finalizing firmware signature hash tag computation...\r\n");
      
      //Update signature hash tag
      settings->signHashAlgo->final(context->checkContext, context->digest);
   }
   else
#endif
   {
      //Debug message
      TRACE_ERROR("Missing image check method (integrity or authentification or signature!\r\n");
      error = ERROR_ABORTED;
   }

   //Return status code
   return error;
}


/**
 * @brief Verify computed firmware check data tag against the one received in the image.
 * Depending the user settings it could be a fimware integrity or authentification
 * or signature verification.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapCheckVerify(IapContext *context)
{
   error_t error;
   int ret;
   IapSettings* settings;
   
   //Debug message
   TRACE_INFO("Verifying firmware image validity...\r\n");

   //Check parameters validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = (IapSettings*)&context->settings;
   //Initialize status code
   error = NO_ERROR;

#if IAP_INTEGRITY_SUPPORT == ENABLED
   //Is user require integrity check?
   if (settings->integrityAlgo != NULL)
   {
      //Debug message
      TRACE_DEBUG("\r\n");
      TRACE_DEBUG("Checking firmware integrity...\r\n");
      TRACE_DEBUG("Received integrity tag (%d bytes):\r\n", context->checkDataSize);
      TRACE_DEBUG_ARRAY("  ", context->checkData, context->checkDataSize);
      TRACE_DEBUG("Computed integrity tag (%d bytes)\r\n", context->digestLen);
      TRACE_DEBUG_ARRAY("  ", context->digest, context->digestLen);
      TRACE_DEBUG("\r\n");

      //Compare computed integrity digest with the one received in the image 
      ret = osMemcmp(context->checkData, context->digest, context->digestLen);
      //Is firmware integrity not valid?
      if (ret != 0)
      {
         //Debug message
         TRACE_INFO("Firmware integrity tag is not valid!\r\n");
         error = ERROR_ABORTED;
      }
   }
   else
#endif
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   //Is user require authentification check?
   if(settings->authAlgo != IAP_AUTH_NONE)
   {
      //Debug message
      TRACE_DEBUG("\r\n");
      TRACE_DEBUG("Checking firmware authentification...\r\n");
      TRACE_DEBUG("Received authentification tag (%d bytes):\r\n", context->checkDataSize);
      TRACE_DEBUG_ARRAY("  ", context->checkData, context->checkDataSize);
      TRACE_DEBUG("Computed authentification tag (%d bytes)\r\n", context->digestLen);
      TRACE_DEBUG_ARRAY("  ", context->digest, context->digestLen);
      TRACE_DEBUG("\r\n");

      //Compare computed integrity digest with the one received in the image 
      ret = osMemcmp(context->checkData, context->digest, context->digestLen);
      //Is firmware integrity not valid?
      if (ret != 0)
      {
         //Debug message
         TRACE_INFO("Firmware authentification tag is not valid!\r\n");
         error = ERROR_ABORTED;
      }
   }
   else
#endif
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //Is user require signature check?
   if (settings->signAlgo != IAP_SIGN_NONE)
   {
      //Debug message
      TRACE_DEBUG("\r\n");
      TRACE_DEBUG("Checking firmware signature...\r\n");
      TRACE_DEBUG("Received signature (%d bytes):\r\n", context->checkDataSize);
      TRACE_DEBUG_ARRAY("  ", context->checkData, context->checkDataSize);
      TRACE_DEBUG("Computed signature hash tag (%d bytes)\r\n", context->digestLen);
      TRACE_DEBUG_ARRAY("  ", context->digest, context->digestLen);
      TRACE_DEBUG("\r\n");

      //Verify receive signature
      error = iapSignVerify(context);
      //Is any error?
      if (error)
      {
         //Debug message
         TRACE_INFO("Firmware signature is not valid!\r\n");
      }
   }
   else
#endif
   {
      //Debug message
      TRACE_ERROR("Missing image check method (integrity or authentification or signature!\r\n");
      error = ERROR_ABORTED;
   }

   //Return status
   return error;
}
