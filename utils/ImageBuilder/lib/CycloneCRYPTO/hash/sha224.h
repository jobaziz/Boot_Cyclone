/**
 * @file sha224.h
 * @brief SHA-224 (Secure Hash Algorithm 224)
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

#ifndef _SHA224_H
#define _SHA224_H

//Dependencies
#include "core/crypto.h"
#include "hash/sha256.h"

//SHA-224 block size
#define SHA224_BLOCK_SIZE 64
//SHA-224 digest size
#define SHA224_DIGEST_SIZE 28
//Minimum length of the padding string
#define SHA224_MIN_PAD_SIZE 9
//Common interface for hash algorithms
#define SHA224_HASH_ALGO (&sha224HashAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief SHA-224 algorithm context
 **/

typedef Sha256Context Sha224Context;


//SHA-224 related constants
extern const uint8_t SHA224_OID[9];
extern const HashAlgo sha224HashAlgo;

//SHA-224 related functions
error_t sha224Compute(const void *data, size_t length, uint8_t *digest);
void sha224Init(Sha224Context *context);
void sha224Update(Sha224Context *context, const void *data, size_t length);
void sha224Final(Sha224Context *context, uint8_t *digest);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
