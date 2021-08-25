/**
 * @file iap_sign.c
 * @brief CycloneBOOT IAP image signature handling
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
#include "iap/iap_sign.h"
#include "pkc/rsa.h"
#include "pkix/pem_import.h"
#include "debug.h"


//IAP signature private related functions
error_t iapSignVerifyRsa(IapContext *context);
error_t iapSignVerifyEcdsa(IapContext *context);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief Initialize signature material for further signature hash computation
 * and signature verification.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapSignInit(IapContext *context)
{
#if IAP_SIGNATURE_SUPPORT == ENABLED
   error_t error;
   IapSettings* settings;
#if IAP_RSA_SUPPORT == ENABLED
   RsaPublicKey publicKey;
#endif
   
   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = &context->settings;
   //Initialize status code
   error = NO_ERROR;

   //Check user settings
   if (settings->signAlgo == IAP_SIGN_NONE || settings->signHashAlgo == NULL ||
      settings->signKey == NULL || settings->signKeyLen == 0)
      return ERROR_INVALID_VALUE;
   
#if IAP_RSA_SUPPORT == ENABLED
   //Is signature RSA algorithm?
   if (settings->signAlgo == IAP_SIGN_RSA)
   {
      //Initialize signature algo context
      settings->signHashAlgo->init(context->checkContext);

      //Set digest length
      context->digestLen = settings->signHashAlgo->digestSize;

      //Initialize RSA public key
      rsaInitPublicKey(&publicKey);

      //Decode pem key file into RSA public key
      error = pemImportRsaPublicKey(settings->signKey, settings->signKeyLen, &publicKey);
      
      //Check status code
      if (!error)
      {
         //Set check data (signature) size
         context->checkDataSize = publicKey.n.size * sizeof(publicKey.n.size);
   
         //Free RSA public key
         rsaFreePublicKey(&publicKey);
      }
   }
   else
#endif
#if IAP_ECDSA_SUPPORT == ENABLED
   //Is signature ECDSA algorithm?
   if (settings->signAlgo == IAP_SIGN_ECDSA)
   {
      error_t = ERROR_NOT_IMPLEMENTED;
   }
   else
#endif
   {
      //Debug message
      TRACE_ERROR("Signature algorithm not supported!\r\n");
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
 * @brief Verify received image firmware signature.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapSignVerify(IapContext *context)
{
#if IAP_SIGNATURE_SUPPORT == ENABLED 
   error_t error;
   IapSettings* settings;

   //Check parameters validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = (IapSettings*) &context->settings;
   //Initialize status code
   error = NO_ERROR;

   //Check authentification hash algo
   if (settings->signAlgo == IAP_SIGN_NONE)
      return ERROR_INVALID_VALUE;

#if IAP_RSA_SUPPORT == ENABLED
   //Is user require RSA signature?
   if (settings->signAlgo == IAP_SIGN_RSA)
   {
      //Verify RSA signature
      error = iapSignVerifyRsa(context);
   }
   else
#endif
#if IAP_ECDSA_SUPPORT == ENABLED
   //Is user require ECDSA signature?
   if (settings->signAlgo == IAP_SIGN_ECDSA)
   {
      //Verify ECDSA signature
      error = iapSignVerifyEcdsa(context);
   }
   else
#endif
   {
      //Debug message
      TRACE_ERROR("Signature algorithm not supported!\r\n");
      error = ERROR_NOT_IMPLEMENTED;
   }

   //Return status code
   return error;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief Verify RSA signature.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapSignVerifyRsa(IapContext* context)
{
#if IAP_SIGNATURE_SUPPORT == ENABLED
   error_t error;
   IapSettings* settings;
   RsaPublicKey publicKey;

   //Check parameter validity
   if (context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to IAP user settings
   settings = (IapSettings*)&context->settings;

   //Check signature user settings
   if (settings->signHashAlgo == NULL || settings->signKey == NULL ||
      settings->signKeyLen == 0)
      return ERROR_ABORTED;

   //Initialize RSA public key
   rsaInitPublicKey(&publicKey);

   //Import PEM RSA public key
   error = pemImportRsaPublicKey(settings->signKey, settings->signKeyLen, &publicKey);
   //Is any error?
   if (error)
   {
      //Debug message
      TRACE_ERROR("RSA public key import failed!\r\n");
      return error;
   }

   error = rsassaPkcs1v15Verify(&publicKey, settings->signHashAlgo, 
      context->digest, context->checkData, context->checkDataLen);
   //Is any error?
   if (error)
   {
      //Debuge message
      TRACE_ERROR("RSA signature verification failed!\r\n");
      return error;
   }

   //Successful process
   return NO_ERROR;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Verify ECDSA signature.
 * @param[in,out] context Point to the IAP context
 * @return Error code
 **/

error_t iapSignVerifyEcdsa(IapContext* context)
{
#if IAP_SIGNATURE_SUPPORT == ENABLED
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#else
   //Not implemented
   return ERROR_NOT_IMPLEMENTED;
#endif
}
