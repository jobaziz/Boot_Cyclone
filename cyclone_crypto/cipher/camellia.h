/**
 * @file camellia.h
 * @brief Camellia encryption algorithm
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

#ifndef _CAMELLIA_H
#define _CAMELLIA_H

//Dependencies
#include "core/crypto.h"

//Camellia block size
#define CAMELLIA_BLOCK_SIZE 16
//Common interface for encryption algorithms
#define CAMELLIA_CIPHER_ALGO (&camelliaCipherAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Structure describing subkey generation
 **/

typedef struct
{
   uint8_t index;
   uint8_t key;
   uint8_t shift;
   uint8_t position;
} CamelliaSubkey;


/**
 * @brief Camellia algorithm context
 **/

typedef struct
{
   uint_t nr;
   uint32_t k[16];
   uint32_t ks[68];
} CamelliaContext;


//Camellia related constants
extern const uint8_t CAMELLIA128_CBC_OID[11];
extern const uint8_t CAMELLIA192_CBC_OID[11];
extern const uint8_t CAMELLIA256_CBC_OID[11];
extern const CipherAlgo camelliaCipherAlgo;

//Camellia related functions
error_t camelliaInit(CamelliaContext *context, const uint8_t *key,
   size_t keyLen);

void camelliaEncryptBlock(CamelliaContext *context, const uint8_t *input,
   uint8_t *output);

void camelliaDecryptBlock(CamelliaContext *context, const uint8_t *input,
   uint8_t *output);

void camelliaDeinit(CamelliaContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
