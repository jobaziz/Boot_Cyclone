/**
 * @file md5_crypt.h
 * @brief Unix crypt using MD5
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

#ifndef _MD5_CRYPT_H
#define _MD5_CRYPT_H

//Dependencies
#include "core/crypto.h"

//Number of rounds
#define MD5_CRYPT_ROUNDS 1000

//Maximum length of the salt string
#define MD5_CRYPT_MAX_SALT_LEN 8

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MD5-crypt related functions
error_t md5Crypt(const char_t *password, const char_t *salt, char_t *output,
   size_t *outputLen);

size_t md5CryptEncodeBase64(const uint8_t *input, uint8_t *output);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
