/**
 * @file ra4_crypto_pkc.h
 * @brief RA4 public-key hardware accelerator
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneCRYPTO Eval.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.4.2
 **/

#ifndef _RA4_CRYPTO_PKC_H
#define _RA4_CRYPTO_PKC_H

//Dependencies
#include "hw_sce_private.h"
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef RA4_CRYPTO_PKC_SUPPORT
   #define RA4_CRYPTO_PKC_SUPPORT DISABLED
#elif (RA4_CRYPTO_PKC_SUPPORT != ENABLED && RA4_CRYPTO_PKC_SUPPORT != DISABLED)
   #error RA4_CRYPTO_PKC_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief RSA primitive arguments
 **/

typedef struct
{
   uint32_t m[128];
   uint32_t c[128];
   uint32_t key[256];
   uint32_t wrappedKey[300];
} Ra4RsaArgs;


/**
 * @brief EC primitive arguments
 **/

typedef struct
{
   uint32_t g[24];
   uint32_t d[12];
   uint32_t q[24];
   uint32_t digest[12];
   uint32_t signature[24];
   uint32_t wrappedKey[100];
} Ra4EcArgs;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
