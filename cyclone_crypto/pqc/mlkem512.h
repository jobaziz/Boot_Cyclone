/**
 * @file mlkem512.h
 * @brief ML-KEM-512 key encapsulation mechanism
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

#ifndef _MLKEM512_H
#define _MLKEM512_H

//Dependencies
#include "core/crypto.h"

//Public key length
#define MLKEM512_PUBLIC_KEY_LEN 800
//Secret key length
#define MLKEM512_SECRET_KEY_LEN 1632
//Ciphertext length
#define MLKEM512_CIPHERTEXT_LEN 768
//Shared secret length
#define MLKEM512_SHARED_SECRET_LEN 32

//Common interface for key encapsulation mechanisms (KEM)
#define MLKEM512_KEM_ALGO (&mlkem512KemAlgo)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//ML-KEM-512 related constants
extern const KemAlgo mlkem512KemAlgo;

//ML-KEM-512 related functions
error_t mlkem512GenerateKeyPair(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *pk, uint8_t *sk);

error_t mlkem512Encapsulate(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *ct, uint8_t *ss, const uint8_t *pk);

error_t mlkem512Decapsulate(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
