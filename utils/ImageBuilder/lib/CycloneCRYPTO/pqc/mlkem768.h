/**
 * @file mlkem768.h
 * @brief ML-KEM-768 key encapsulation mechanism
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

#ifndef _MLKEM768_H
#define _MLKEM768_H

//Dependencies
#include "core/crypto.h"

//Public key length
#define MLKEM768_PUBLIC_KEY_LEN 1184
//Secret key length
#define MLKEM768_SECRET_KEY_LEN 2400
//Ciphertext length
#define MLKEM768_CIPHERTEXT_LEN 1088
//Shared secret length
#define MLKEM768_SHARED_SECRET_LEN 32

//Common interface for key encapsulation mechanisms (KEM)
#define MLKEM768_KEM_ALGO (&mlkem768KemAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//ML-KEM-768 related constants
extern const KemAlgo mlkem768KemAlgo;

//ML-KEM-768 related functions
error_t mlkem768GenerateKeyPair(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *pk, uint8_t *sk);

error_t mlkem768Encapsulate(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *ct, uint8_t *ss, const uint8_t *pk);

error_t mlkem768Decapsulate(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
