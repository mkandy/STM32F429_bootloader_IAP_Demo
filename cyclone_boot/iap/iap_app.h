/**
 * @file iap_app.h
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

#ifndef _IAP_APP_H
#define _IAP_APP_H

//Dependencies
#include <stdint.h>
#include "iap_config.h"
#include "iap/iap_driver.h"
#include "core/crypto.h"
#include "mac/hmac.h"
#include "cipher/aes.h"
#include "iap/crc32.h"
#include "error.h"


//Version string
#define CYCLONE_BOOT_IAP_VERSION_STRING "1.0.0"
//Major version
#define CYCLONE_BOOT_IAP_MAJOR_VERSION 1
//Minor version
#define CYCLONE_BOOT_IAP_MINOR_VERSION 0
//Revision number
#define CYCLONE_BOOT_IAP_REV_NUMBER 0


//Integrity check support
#ifndef IAP_INTEGRITY_SUPPORT
   #define IAP_INTEGRITY_SUPPORT ENABLED
#elif (IAP_INTEGRITY_SUPPORT != ENABLED && IAP_INTEGRITY_SUPPORT != DISABLED)
   #error IAP_INTEGRITY_SUPPORT parameter is not valid
#endif

//Authentication check support
#ifndef IAP_AUTHENTICATION_SUPPORT
   #define IAP_AUTHENTICATION_SUPPORT DISABLED
#elif (IAP_AUTHENTICATION_SUPPORT != ENABLED && IAP_AUTHENTICATION_SUPPORT != DISABLED)
   #error IAP_AUTHENTICATION_SUPPORT parameter is not valid
#endif

//Signature check support
#ifndef IAP_SIGNATURE_SUPPORT
   #define IAP_SIGNATURE_SUPPORT DISABLED
#elif (IAP_SIGNATURE_SUPPORT != ENABLED && IAP_SIGNATURE_SUPPORT != DISABLED)
   #error IAP_SIGNATURE_SUPPORT parameter is not valid
#endif

//Acceptabled check support
#if !((IAP_INTEGRITY_SUPPORT == ENABLED && !(IAP_AUTHENTICATION_SUPPORT == ENABLED || \
   IAP_SIGNATURE_SUPPORT == ENABLED)) || \
   (IAP_AUTHENTICATION_SUPPORT == ENABLED && !(IAP_INTEGRITY_SUPPORT == ENABLED || \
   IAP_SIGNATURE_SUPPORT == ENABLED)) ||\
   (IAP_SIGNATURE_SUPPORT == ENABLED && !(IAP_AUTHENTICATION_SUPPORT == ENABLED || \
   IAP_INTEGRITY_SUPPORT == ENABLED)))
   #error Exactly one of the following parameters MUST be enabled : \
IAP_INTEGRITY_SUPPORT - \
IAP_AUTHENTICATION_SUPPORT - \
IAP_SIGNATURE_SUPPORT
#endif

//Cipher support
#ifndef IAP_ENCRYPTION_SUPPORT
   #define IAP_ENCRYPTION_SUPPORT DISABLED
#elif (IAP_ENCRYPTION_SUPPORT != ENABLED && IAP_ENCRYPTION_SUPPORT != DISABLED)
   #error IAP_ENCRYPTION_SUPPORT parameter is not valid
#endif

//RSA key support
#ifndef IAP_RSA_SUPPORT
   #define IAP_RSA_SUPPORT DISABLED
#elif (IAP_RSA_SUPPORT != ENABLED && IAP_RSA_SUPPORT != DISABLED)
   #error IAP_RSA_SUPPORT parameter is not valid
#endif

//ECDSA key support
#ifndef IAP_ECDSA_SUPPORT
   #define IAP_ECDSA_SUPPORT DISABLED
#elif (IAP_ECDSA_SUPPORT != ENABLED && IAP_ECDSA_SUPPORT != DISABLED)
   #error IAP_ECDSA_SUPPORT parameter is not valid
#endif

//Cipher context size
#define IAP_MAX_CIPHER_CONTEXT_SIZE sizeof(AesContext)
//Cipher IV size
#define IAP_MAX_CIPHER_IV_SIZE 32
//Check data context size
#define IAP_MAX_CHECK_CONTEXT_SIZE sizeof(HmacContext)
//Signature size
#define IAP_MAX_CHECK_DATA_SIZE 512


//Forward declaration of IapContext structure
struct _IapContext;
#define IapContext struct _IapContext

/**
 *@brief Supported authentication algorithms
 **/

typedef enum
{
   IAP_AUTH_NONE,
   IAP_AUTH_HMAC
} IapAuthAlgo;


/**
 *@brief Supported signature algorithms
 **/

typedef enum
{
   IAP_SIGN_NONE,
   IAP_SIGN_RSA
} IapSignAlgo;


/**
 * @brief IAP application image header
 **/

typedef struct
{
   uint32_t appSize;
} IapAppHeader;


/**
 * @brief IAP user settings
 **/

typedef struct
{
#if IAP_ENCRYPTION_SUPPORT == ENABLED
   const CipherAlgo *cipherAlgo;
   CipherMode cipherMode;
   const char_t *cipherKey;
   size_t cipherKeyLen;
#endif

#if IAP_INTEGRITY_SUPPORT == ENABLED   
   const HashAlgo *integrityAlgo;
#endif
   
#if IAP_AUTHENTICATION_SUPPORT == ENABLED
   IapAuthAlgo authAlgo;
   const HashAlgo *authHashAlgo;
   const char_t *authKey;
   size_t authKeyLen;
#endif

#if IAP_SIGNATURE_SUPPORT == ENABLED
   IapSignAlgo signAlgo;
   const HashAlgo *signHashAlgo;
   const char_t *signKey;
   size_t signKeyLen;
#endif
}IapSettings;


/**
 * @brief IAP states
 **/

typedef enum
{
   IAP_STATE_IDLE,
   IAP_STATE_RECV_APP_HEADER,
   IAP_STATE_RECV_APP_DATA,
   IAP_STATE_RECV_APP_CHECK,
   IAP_STATE_VALIDATE_APP,
   IAP_STATE_APP_REBOOT
} IapState;


/**
 * @brief IAP context
 **/

struct _IapContext
{
   IapSettings settings;
   
   IapState state;
   
   uint32_t firmwareAddr;
   size_t firmwareLength;
   uint32_t pos;
   size_t written;
   
   uint8_t buffer[128];
   uint8_t *bufferPos;
   size_t bufferLen;

#if IAP_ENCRYPTION_SUPPORT == ENABLED
   uint8_t cipherContext[IAP_MAX_CIPHER_CONTEXT_SIZE];
   uint8_t cipherIv[IAP_MAX_CIPHER_IV_SIZE];
   size_t cipherIvLen;
#endif
   
   uint8_t checkContext[IAP_MAX_CHECK_CONTEXT_SIZE];
   uint8_t checkData[IAP_MAX_CHECK_DATA_SIZE];
   uint8_t *checkDataPos;
   size_t checkDataLen;
   size_t checkDataSize;

   uint8_t digest[MAX_HASH_DIGEST_SIZE];
   size_t digestLen;
};

//IAP application related functions
void iapGetDefaultSettings(IapSettings *settings);
error_t iapInit(IapContext *context, const IapSettings *settings);
error_t iapUpdate(IapContext *context, const void *data, size_t length);
error_t iapFinalize(IapContext *context);
error_t iapReboot(IapContext *context);

#endif // !_IAP_APP_H
