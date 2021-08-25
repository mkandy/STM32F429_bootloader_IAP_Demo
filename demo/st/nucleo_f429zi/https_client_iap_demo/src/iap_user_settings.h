/**
 * @file iap_user_settings.h
 * @brief IAP user settings configuration
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

#ifndef _IAP_USER_SETTINGS
#define _IAP_USER_SETTINGS

//Dependencies
#include "iap/iap_app.h"

extern const uint8_t* pemIapSignPublicKey;
extern size_t pemIapSignPublicKeyLen;

//IAP signature public key resource path
#define IAP_PUBLIC_SIGN_KEY "keys/rsa_public_key.pem"

//Scenario 1 : Plaintext app + integrity
#if defined(IAP_SCENARIO_1) || (!(defined(IAP_SCENARIO_1) || defined(IAP_SCENARIO_2) || defined(IAP_SCENARIO_3)))
#define IAP_SETTING_INTEGRITY_ALGO  CRC32_HASH_ALGO
#define IAP_SETTING_AUTH_ALGO       IAP_AUTH_NONE
#define IAP_SETTING_AUTH_HASH_ALGO  NULL
#define IAP_SETTING_AUTH_KEY        NULL
#define IAP_SETTING_AUTH_KEY_LEN    0
#define IAP_SETTING_SIGN_ALGO       IAP_SIGN_NONE
#define IAP_SETTING_SIGN_HASH_ALGO  NULL
#define IAP_SETTING_SIGN_KEY        NULL
#define IAP_SETTING_SIGN_KEY_LEN    0
#define IAP_SETTING_CIPHER_ALGO     NULL
#define IAP_SETTING_CIPHER_MODE     CIPHER_MODE_NULL
#define IAP_SETTING_CIPHER_KEY      NULL
#define IAP_SETTING_CIHPER_KEY_LEN  0
#endif

//Scenario 2 : Cipher app + authentification
#if defined(IAP_SCENARIO_2)
#define IAP_SETTING_INTEGRITY_ALGO  NULL
#define IAP_SETTING_AUTH_ALGO       IAP_AUTH_HMAC
#define IAP_SETTING_AUTH_HASH_ALGO  MD5_HASH_ALGO
#define IAP_SETTING_AUTH_KEY        "107b8d4bf62d1c9e26968e8ff51a74df"
#define IAP_SETTING_AUTH_KEY_LEN    32
#define IAP_SETTING_SIGN_ALGO       IAP_SIGN_NONE
#define IAP_SETTING_SIGN_HASH_ALGO  NULL
#define IAP_SETTING_SIGN_KEY        NULL
#define IAP_SETTING_SIGN_KEY_LEN    0
#define IAP_SETTING_CIPHER_ALGO     AES_CIPHER_ALGO
#define IAP_SETTING_CIPHER_MODE     CIPHER_MODE_CBC
#define IAP_SETTING_CIPHER_KEY      "aa3ff7d43cc015682c7dfd00de9379e7"
#define IAP_SETTING_CIHPER_KEY_LEN  32
#endif

//Scenario 3 : Cipher app + signature
#if defined(IAP_SCENARIO_3)
#define IAP_SETTING_INTEGRITY_ALGO  NULL
#define IAP_SETTING_AUTH_ALGO       IAP_AUTH_NONE
#define IAP_SETTING_AUTH_HASH_ALGO  NULL
#define IAP_SETTING_AUTH_KEY        NULL
#define IAP_SETTING_AUTH_KEY_LEN    0
#define IAP_SETTING_SIGN_ALGO       IAP_SIGN_RSA
#define IAP_SETTING_SIGN_HASH_ALGO  SHA256_HASH_ALGO
#define IAP_SETTING_SIGN_KEY        (const char_t*) pemIapSignPublicKey
#define IAP_SETTING_SIGN_KEY_LEN    pemIapSignPublicKeyLen
#define IAP_SETTING_CIPHER_ALGO     AES_CIPHER_ALGO
#define IAP_SETTING_CIPHER_MODE     CIPHER_MODE_CBC
#define IAP_SETTING_CIPHER_KEY      "aa3ff7d43cc015682c7dfd00de9379e7"
#define IAP_SETTING_CIHPER_KEY_LEN  32
#endif

#endif //!_IAP_USER_SETTINGS
