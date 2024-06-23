/**
 * @file m460_crypto_hash.h
 * @brief M460 hash hardware accelerator
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
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
 * @version 3.0.4
 **/

#ifndef _M460_CRYPTO_HASH_H
#define _M460_CRYPTO_HASH_H

//Dependencies
#include "core/crypto.h"

//Hash hardware accelerator
#ifndef M460_CRYPTO_HASH_SUPPORT
   #define M460_CRYPTO_HASH_SUPPORT DISABLED
#elif (M460_CRYPTO_HASH_SUPPORT != ENABLED && M460_CRYPTO_HASH_SUPPORT != DISABLED)
   #error M460_CRYPTO_HASH_SUPPORT parameter is not valid
#endif

//OPMODE bitfield
#define CRPT_HMAC_CTL_OPMODE_SHA1     0x00000000
#define CRPT_HMAC_CTL_OPMODE_SHA256   0x00000400
#define CRPT_HMAC_CTL_OPMODE_SHA224   0x00000500
#define CRPT_HMAC_CTL_OPMODE_SHA512   0x00000600
#define CRPT_HMAC_CTL_OPMODE_SHA384   0x00000700
#define CRPT_HMAC_CTL_OPMODE_SHA3_256 0x00000400
#define CRPT_HMAC_CTL_OPMODE_SHA3_224 0x00000500
#define CRPT_HMAC_CTL_OPMODE_SHA3_512 0x00000600
#define CRPT_HMAC_CTL_OPMODE_SHA3_384 0x00000700
#define CRPT_HMAC_CTL_OPMODE_SHAKE128 0x00000000
#define CRPT_HMAC_CTL_OPMODE_SHAKE256 0x00000100

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
