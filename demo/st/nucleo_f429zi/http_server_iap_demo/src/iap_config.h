/**
 * @file iap_config.h
 * @brief CycloneBOOT IAP configuration file
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

#ifndef _IAP_CONFIG_H
#define _IAP_CONFIG_H

//Trace level for CycloneBOOT IAP stack debugging
#define IAP_TRACE_LEVEL          4


#if defined(IAP_SCENARIO_2)

//IAP encryption support
#define IAP_ENCRYPTION_SUPPORT ENABLED
//IAP integrity support
#define IAP_INTEGRITY_SUPPORT DISABLED
//IAP authentication support
#define IAP_AUTHENTICATION_SUPPORT ENABLED

#elif defined(IAP_SCENARIO_3)

//IAP integrity support
#define IAP_INTEGRITY_SUPPORT DISABLED
//IAP signature support
#define IAP_ENCRYPTION_SUPPORT ENABLED
//IAP authentication support
#define IAP_SIGNATURE_SUPPORT ENABLED
//IAP RSA signature support
#define IAP_RSA_SUPPORT ENABLED

#else

//IAP integrity support
#define IAP_INTEGRITY_SUPPORT ENABLED

#endif



#endif //!_IAP_CONFIG_H
