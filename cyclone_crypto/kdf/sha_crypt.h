/**
 * @file sha_crypt.h
 * @brief Unix crypt using SHA-256 and SHA-512
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

#ifndef _SHA_CRYPT_H
#define _SHA_CRYPT_H

//Dependencies
#include "core/crypto.h"

//Minimum number of rounds
#define SHA_CRYPT_MIN_ROUNDS 1000
//Maximum number of rounds
#define SHA_CRYPT_MAX_ROUNDS 999999999
//Default number of rounds
#define SHA_CRYPT_DEFAULT_ROUNDS 5000

//Maximum length of the salt string
#define SHA_CRYPT_MAX_SALT_LEN 16

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//SHA-crypt related functions
error_t shaCrypt(const HashAlgo *hashAlgo, const char_t *password,
   const char_t *salt, char_t *output, size_t *outputLen);

size_t shaCryptEncodeBase64(const HashAlgo *hashAlgo, const uint8_t *input,
   uint8_t *output);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
