/**
 * @file sha256.h
 * @brief SHA-256 (Secure Hash Algorithm 256)
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

#ifndef _SHA256_H
#define _SHA256_H

//Dependencies
#include "core/crypto.h"

//Application specific context
#ifndef SHA256_PRIVATE_CONTEXT
   #define SHA256_PRIVATE_CONTEXT
#endif

//SHA-256 block size
#define SHA256_BLOCK_SIZE 64
//SHA-256 digest size
#define SHA256_DIGEST_SIZE 32
//Minimum length of the padding string
#define SHA256_MIN_PAD_SIZE 9
//Common interface for hash algorithms
#define SHA256_HASH_ALGO (&sha256HashAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SHA-256 algorithm context
 **/

typedef struct
{
   union
   {
      uint32_t h[8];
      uint8_t digest[32];
   };
   union
   {
      uint32_t w[16];
      uint8_t buffer[64];
   };
   size_t size;
   uint64_t totalSize;
   SHA256_PRIVATE_CONTEXT
} Sha256Context;


//SHA-256 related constants
extern const uint8_t SHA256_OID[9];
extern const HashAlgo sha256HashAlgo;

//SHA-256 related functions
error_t sha256Compute(const void *data, size_t length, uint8_t *digest);
void sha256Init(Sha256Context *context);
void sha256Update(Sha256Context *context, const void *data, size_t length);
void sha256Final(Sha256Context *context, uint8_t *digest);
void sha256FinalRaw(Sha256Context *context, uint8_t *digest);
void sha256ProcessBlock(Sha256Context *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
