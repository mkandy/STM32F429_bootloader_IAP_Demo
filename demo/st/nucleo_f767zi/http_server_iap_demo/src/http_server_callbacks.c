/**
 * @file http_server_callbacks.c
 * @brief HTTP server callbacks handling
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
#include "core/net.h"
#include "path.h"
#include "date_time.h"
#include "iap/iap_app.h"
#include "http_server_callbacks.h"
#include "iap/iap_app.h"
#include "iap_user_settings.h"
#include "version.h"
#include "debug.h"

//Gloabal variables
IapSettings iapSettings;
IapContext iapContext;

//Extern variables
extern OsMutex iapMutex;
extern const char_t* appVersion;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief HTTP request callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/

error_t httpServerRequestCallback(HttpConnection *connection,
   const char_t *uri)
{
   error_t error;
   uint8_t buffer[128];
   size_t received;
   size_t n;

   //Initialize variables
   received = 0;
   error = NO_ERROR;
   
   //Manage application data request
   if(!strcasecmp(uri, "/data.json"))
   {
      //Format json response
      n = sprintf((char_t*) buffer, 
         "{" \
            "\"data\": {" \
               "\"version\": \"%s\"," \
               "\"color\": \"%s\"," \
               "\"scenario\": \"%d\"" \
            "}" \
         "}", 
         APP_VERSION_STRING,
         APP_VERSION_COLOR,
#if defined(IAP_SCENARIO_2)
         2
#elif defined(IAP_SCENARIO_3)
         3
#else
         1
#endif
      );
      
      //Format HTTP response header
      connection->response.version = connection->request.version;
      connection->response.statusCode = 200;
      connection->response.keepAlive = connection->request.keepAlive;
      connection->response.noCache = TRUE;
      connection->response.contentLength = n;
      
      //Send the header to the client
      error = httpWriteHeader(connection);
      //Any error to report?
      if(error)
         return error;
      
      //Send the response data to the client
      error = httpWriteStream(connection, buffer, n);
      //Any error to report?
      if(error)
         return error;
      
      //Properly close output stream
      error = httpCloseStream(connection);
      //Any error to report?
      if(error)
         return error;
   }
   //Manage new firmware request
   else if(!strcasecmp(uri, "/upload/binary"))
   {
      //Get exclusive access
      osAcquireMutex(&iapMutex);
      
      //Debug message
      TRACE_INFO("Updating firmware...\r\n");
      
      //Start of exception handling block
      do
      {
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
            TRACE_INFO("Receiving firmware image...\r\n");
            
            //Process HTTP request body
            while(1)
            {
               //Read the HTTP request body until buffer is full or end of stream
               error = httpReadStream(connection, buffer, sizeof(buffer), 
                  &received, HTTP_FLAG_WAIT_ALL);
               //End of stream detected?
               if(error)
                  break;
              
               //Write received bytes in flash
               error = iapUpdate(&iapContext, (uint8_t*) buffer, received);
               //Is any error?
               if(error)
               {
                  //Debug message
                  TRACE_ERROR("Failed to update firmware!\r\n");
                  break;
               }
            }
            
            //Propagate exception if necessary
            if(error != ERROR_END_OF_STREAM)
            {
               //Debug message
               TRACE_ERROR("Firmware image reception failed!\r\n");
               break;
            }
            else
               error = NO_ERROR;
                 
            //Check application firmware validity
            error = iapFinalize(&iapContext);
            //Is application is invalid?
            if(error)
            {
               //Debug message
               TRACE_ERROR("Failed to finalize firmware image update!\r\n");
               break;
            }

            //Start of exception handling block
            do
            {
               //Format HTTP response header
               connection->response.version = connection->request.version;
               connection->response.statusCode = 200;
               connection->response.keepAlive = connection->request.keepAlive;
               connection->response.noCache = TRUE;
               connection->response.contentLength = 0;

               //Send the header to the client
               error = httpWriteHeader(connection);
               //Any error to report?
               if(error)
               {
                  //Debug message
                  TRACE_ERROR("Sending HTTP response response failed!\r\n");
                  break;
               }
               
               //Properly close output stream
               error = httpCloseStream(connection);
               //Any error to report?
               if(error)
               {
                  //Debug message
                  TRACE_ERROR("Closing HTTP output stream failed!\r\n");
                  break;
               }
               //End of exception handling block
            }while(0);
            
            //Perform flash swap bank then reboot
            error = iapReboot(&iapContext);
            //Is any error?
            if(error)
            {
               //Debug message
               TRACE_ERROR("Failed to reboot!\r\n");
               break;
            }
            
            //End of exception handling block
         }while(0);
      
         //Is any error?
         if(error)
         {
            //Format HTTP response header
            connection->response.version = connection->request.version;
            connection->response.statusCode = 400;
            connection->response.keepAlive = connection->request.keepAlive;
            connection->response.noCache = TRUE;
            connection->response.contentLength = 0;
            
            //Send the header to the client
            error = httpWriteHeader(connection);
            //Any error to report?
            if(error)
            {
               //Debug message
               TRACE_ERROR("Sending HTTP response response failed!\r\n");
               break;
            }
            
            //Properly close output stream
            error = httpCloseStream(connection);
            //Any error to report?
            if(error)
            {
               //Debug message
               TRACE_ERROR("Closing HTTP output stream failed!\r\n");
               break;
            }
         }
         
         //End of exception handling block
      }while(0);
      
      //Release exclusive access
      osReleaseMutex(&iapMutex);
      
      //Return status code
      return error;
   }
   else
   {
      //For sanity
   }
   
   //Not implemented
   return ERROR_NOT_FOUND;
}


/**
 * @brief URI not found callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/

error_t httpServerUriNotFoundCallback(HttpConnection *connection,
   const char_t *uri)
{
   //Not implemented
   return ERROR_NOT_FOUND;
}
