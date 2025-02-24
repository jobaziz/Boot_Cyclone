/**
 * @file ecdh.h
 * @brief ECDH (Elliptic Curve Diffie-Hellman) key exchange
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

#ifndef _ECDH_H
#define _ECDH_H

//Dependencies
#include "core/crypto.h"
#include "ecc/ec.h"

//X25519 supported?
#if (X25519_SUPPORT == ENABLED)
   #include "ecc/x25519.h"
#endif

//X448 supported?
#if (X448_SUPPORT == ENABLED)
   #include "ecc/x448.h"
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ECDH context
 **/

typedef struct
{
   EcDomainParameters params; ///<EC domain parameters
   EcPrivateKey da;           ///<One's own EC private key
   EcPublicKey qa;            ///<One's own EC public key
   EcPublicKey qb;            ///<Peer's EC public key
} EcdhContext;


//ECDH related functions
void ecdhInit(EcdhContext *context);
void ecdhFree(EcdhContext *context);

error_t ecdhGenerateKeyPair(EcdhContext *context, const PrngAlgo *prngAlgo,
   void *prngContext);

error_t ecdhCheckPublicKey(const EcDomainParameters *params, EcPoint *publicKey);

error_t ecdhComputeSharedSecret(EcdhContext *context,
   uint8_t *output, size_t outputSize, size_t *outputLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
