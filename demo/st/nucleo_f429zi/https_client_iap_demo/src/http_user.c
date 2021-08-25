/**
 * @file http_user.c
 * @brief HTTP user request handling
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

//Dependencies
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "http/http_client.h"
#include "http_user.h"
#include "tls.h"
#include "tls_cipher_suites.h"
#include "rng/yarrow.h"
#include "main.h"
#include "iap/iap_app.h"
#include "iap_user_settings.h"
#include "debug.h"

//Application configuration
#define APP_HTTP_SERVER_NAME "www.oryx-embedded.com"

#define APP_HTTP_SERVER_PORT 443
//#define APP_HTTP_SERVER_PORT 80

#if defined(IAP_SCENARIO_1) || (!(defined(IAP_SCENARIO_1) || defined(IAP_SCENARIO_2) || defined(IAP_SCENARIO_3)))
#define APP_IMAGE_VARIANT "_scenario_1"
#elif defined(IAP_SCENARIO_2)
#define APP_IMAGE_VARIANT "_scenario_2"
#elif defined(IAP_SCENARIO_3)
#define APP_IMAGE_VARIANT "_scenario_3"
#endif

#if defined (APP_DEMO_GREEN)
#define APP_IMAGE_FILE "nucleo_f429zi_https_client_iap_blue_image" APP_IMAGE_VARIANT ".img"
#elif defined(APP_DEMO_BLUE)
#define APP_IMAGE_FILE "nucleo_f429zi_https_client_iap_red_image" APP_IMAGE_VARIANT ".img"
#else
#define APP_IMAGE_FILE "nucleo_f429zi_https_client_iap_green_image" APP_IMAGE_VARIANT ".img"
#endif

#define APP_HTTP_URI "/demo/cycloneboot/iap/" APP_IMAGE_FILE

//Gloabal variables
IapSettings iapSettings;
IapContext iapContext;

//Extern variables
extern OsMutex iapMutex;
extern HttpClientContext httpClientContext;
extern YarrowContext yarrowContext;


/**
 * @brief TLS initialization callback
 * @param[in] context Pointer to the HTTP client context
 * @param[in] tlsContext Pointer to the TLS context
 * @return Error code
 **/

error_t httpUserTlsInitCallback(HttpClientContext *context,
   TlsContext *tlsContext)
{
   error_t error;

   //Debug message
   TRACE_INFO("HTTP Client: TLS initialization callback\r\n");

   //Set the PRNG algorithm to be used
   error = tlsSetPrng(tlsContext, YARROW_PRNG_ALGO, &yarrowContext);
   //Any error to report?
   if(error)
      return error;

#if (APP_SET_SERVER_NAME == ENABLED)
   //Set the fully qualified domain name of the server
   error = tlsSetServerName(tlsContext, APP_HTTP_SERVER_NAME);
   //Any error to report?
   if(error)
      return error;
#endif

#if (APP_SET_TRUSTED_CA_LIST == ENABLED)
   //Import the list of trusted CA certificates
   error = tlsSetTrustedCaList(tlsContext, trustedCaList, trustedCaListLength);
   //Any error to report?
   if(error)
      return error;
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief HTTP client test routine
 * @return Error code
 **/

error_t httpUserUpdate(void)
{
   error_t error;
   size_t length;
   uint_t status;
   IpAddr ipAddr;
   uint8_t buffer[128];
   
   //Initialize status code
   error = NO_ERROR;
   
   //Get exclusive access
   osAcquireMutex(&iapMutex);
   
   //Start of exception handling block
   do
   {
      //Get default IAP settings
      iapGetDefaultSettings(&iapSettings);
      
#if defined(IAP_SCENARIO_2)
            //User IAP settings configuration
            iapSettings.authAlgo          = IAP_SETTING_AUTH_ALGO;
            iapSettings.authHashAlgo      = IAP_SETTING_AUTH_HASH_ALGO;
            iapSettings.authKey           = IAP_SETTING_AUTH_KEY;
            iapSettings.authKeyLen        = IAP_SETTING_AUTH_KEY_LEN;
            iapSettings.cipherAlgo        = IAP_SETTING_CIPHER_ALGO;
            iapSettings.cipherMode        = IAP_SETTING_CIPHER_MODE;
            iapSettings.cipherKey         = IAP_SETTING_CIPHER_KEY;
            iapSettings.cipherKeyLen      = IAP_SETTING_CIHPER_KEY_LEN;
#elif defined(IAP_SCENARIO_3)
            //User IAP settings configuration
            iapSettings.signAlgo          = IAP_SETTING_SIGN_ALGO;
            iapSettings.signHashAlgo      = IAP_SETTING_SIGN_HASH_ALGO;
            iapSettings.signKey           = IAP_SETTING_SIGN_KEY;
            iapSettings.signKeyLen        = IAP_SETTING_SIGN_KEY_LEN;
            iapSettings.cipherAlgo        = IAP_SETTING_CIPHER_ALGO;
            iapSettings.cipherMode        = IAP_SETTING_CIPHER_MODE;
            iapSettings.cipherKey         = IAP_SETTING_CIPHER_KEY;
            iapSettings.cipherKeyLen      = IAP_SETTING_CIHPER_KEY_LEN;
#else
            //User IAP settings configuration
            iapSettings.integrityAlgo     = IAP_SETTING_INTEGRITY_ALGO;
#endif
      
      //Initialize IAP Application context
      error = iapInit(&iapContext, &iapSettings);
      //Is any error?
      if(error)
      {
         //Debug message
         TRACE_ERROR("Failed to initialize IAP!\r\n");
         break;
      }
      
      //Debug message
      TRACE_INFO("Downloading firmware image \"%s\"...\r\n", APP_IMAGE_FILE);

      //Initialize HTTP client context
      httpClientInit(&httpClientContext);

      //Start of exception handling block
      do
      {
         //Debug message
         TRACE_INFO("\r\n\r\nResolving server name...\r\n");

         //Resolve HTTP server name
         error = getHostByName(NULL, APP_HTTP_SERVER_NAME, &ipAddr, 0);
         //Any error to report?
         if(error)
         {
            //Debug message
            TRACE_INFO("Failed to resolve server name!\r\n");
            break;
         }
         
#if (APP_HTTP_SERVER_PORT == 443)
         //Register TLS initialization callback
         error = httpClientRegisterTlsInitCallback(&httpClientContext,
            httpUserTlsInitCallback);
         //Any error to report?
         if(error)
            break;
#endif

         //Select HTTP protocol version
         error = httpClientSetVersion(&httpClientContext, HTTP_VERSION_1_1);
         //Any error to report?
         if(error)
            break;

         //Set timeout value for blocking operations
         error = httpClientSetTimeout(&httpClientContext, 20000);
         //Any error to report?
         if(error)
            break;

         //Debug message
         TRACE_INFO("Connecting to HTTP server %s...\r\n",
            ipAddrToString(&ipAddr, NULL));

         //Connect to the HTTP server
         error = httpClientConnect(&httpClientContext, &ipAddr,
            APP_HTTP_SERVER_PORT);
         //Any error to report?
         if(error)
         {
            //Debug message
            TRACE_INFO("Failed to connect to HTTP server!\r\n");
            break;
         }

         //Create an HTTP request
         httpClientCreateRequest(&httpClientContext);
         httpClientSetMethod(&httpClientContext, "GET");
         httpClientSetUri(&httpClientContext, APP_HTTP_URI);

         //Add HTTP header fields
         httpClientAddHeaderField(&httpClientContext, "Host", APP_HTTP_SERVER_NAME);
         httpClientAddHeaderField(&httpClientContext, "User-Agent", "Mozilla/5.0");
         httpClientAddHeaderField(&httpClientContext, "Content-Type", "text/plain");

         //Send HTTP request header
         error = httpClientWriteHeader(&httpClientContext);
         //Any error to report?
         if(error)
         {
            //Debug message
            TRACE_INFO("Failed to write HTTP request header!\r\n");
            break;
         }

         //Receive HTTP response header
         error = httpClientReadHeader(&httpClientContext);
         //Any error to report?
         if(error)
         {
            //Debug message
            TRACE_INFO("Failed to read HTTP response header!\r\n");
            break;
         }

         //Retrieve HTTP status code
         status = httpClientGetStatus(&httpClientContext);
         //Debug message
         TRACE_INFO("HTTP status code: %u\r\n", status);
         
         //Is HTTP request successfullly completed?
         if(!HTTP_STATUS_CODE_2YZ(status))
         {
            //Debug message
            TRACE_INFO("HTTP request failed (invalid status)!\r\n");
            error = ERROR_ABORTED;
            break;
         }

         //Receive HTTP response body
         while(!error)
         {
            //Read data
            error = httpClientReadBody(&httpClientContext, buffer,
               sizeof(buffer), &length, 0);

            //Check status code
            if(!error)
            {
               //Process firmware image data
               error = iapUpdate(&iapContext, buffer, length);
               //Is any error?
               if(error)
               {
                  //Debug message
                  TRACE_ERROR("Failed to update firmware!\r\n");
                  break;
               }
            }
         }

         //Terminate the HTTP response body with a CRLF
         TRACE_INFO("\r\n");

         //Any error to report?
         if(error != ERROR_END_OF_STREAM)
         {
            //Debug message
            TRACE_ERROR("Firmware image reception failed!\r\n");
            break;
         }
         
         //Check image application firmware validity
         error = iapFinalize(&iapContext);
         //Is any error?
         if(error)
         {
            //Debug message
            TRACE_ERROR("Failed to finalize firmware image update!\r\n");
            break;
         }
         
         //Reboot device to finalize update procedure
         error = iapReboot(&iapContext);
         //Is any error?
         if(error)
         {
            //Debug message
            TRACE_ERROR("Failed to reboot!\r\n");
            break;
         }

         //Close HTTP response body
         error = httpClientCloseBody(&httpClientContext);
         //Any error to report?
         if(error)
         {
            //Debug message
            TRACE_INFO("Failed to read HTTP response trailer!\r\n");
            break;
         }

         //Gracefully disconnect from the HTTP server
         httpClientDisconnect(&httpClientContext);

         //Debug message
         TRACE_INFO("Connection closed\r\n");

         //End of exception handling block
      } while(0);

      //Release HTTP client context
      httpClientDeinit(&httpClientContext);
   
      //End of exception handling block
   }while(0);
   
   //Release exclusive access
   osReleaseMutex(&iapMutex);

   //Return status code
   return error;
}


/**
 * @brief User task
 * @param[in] param Unused parameter
 **/

void httpUserTask(void *param)
{
   //Endless loop
   while(1)
   {
      //User button pressed?
      if(BSP_PB_GetState(BUTTON_KEY))
      {
         //HTTP client test routine
         httpUserUpdate();

         //Wait for the user button to be released
         while(BSP_PB_GetState(BUTTON_KEY));
      }

      //Loop delay
      osDelayTask(100);
   }
}
