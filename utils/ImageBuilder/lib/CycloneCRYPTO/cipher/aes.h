/**
 * @file aes.h
 * @brief AES (Advanced Encryption Standard)
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

#ifndef _AES_H
#define _AES_H

//Dependencies
#include "core/crypto.h"

//Application specific context
#ifndef AES_PRIVATE_CONTEXT
   #define AES_PRIVATE_CONTEXT
#endif

//AES block size
#define AES_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define AES_CIPHER_ALGO (&aesCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief AES algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t ek[60];
   uint32_t dk[60];
   AES_PRIVATE_CONTEXT
} AesContext;


//AES related constants
extern const uint8_t AES128_ECB_OID[9];
extern const uint8_t AES128_CBC_OID[9];
extern const uint8_t AES128_OFB_OID[9];
extern const uint8_t AES128_CFB_OID[9];
extern const uint8_t AES128_GCM_OID[9];
extern const uint8_t AES128_CCM_OID[9];
extern const uint8_t AES192_ECB_OID[9];
extern const uint8_t AES192_CBC_OID[9];
extern const uint8_t AES192_OFB_OID[9];
extern const uint8_t AES192_CFB_OID[9];
extern const uint8_t AES192_GCM_OID[9];
extern const uint8_t AES192_CCM_OID[9];
extern const uint8_t AES256_ECB_OID[9];
extern const uint8_t AES256_CBC_OID[9];
extern const uint8_t AES256_OFB_OID[9];
extern const uint8_t AES256_CFB_OID[9];
extern const uint8_t AES256_GCM_OID[9];
extern const uint8_t AES256_CCM_OID[9];
extern const CipherAlgo aesCipherAlgo;

//AES related functions
error_t aesInit(AesContext *context, const uint8_t *key, size_t keyLen);

void aesEncryptBlock(AesContext *context, const uint8_t *input,
   uint8_t *output);

void aesDecryptBlock(AesContext *context, const uint8_t *input,
   uint8_t *output);

void aesDeinit(AesContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
