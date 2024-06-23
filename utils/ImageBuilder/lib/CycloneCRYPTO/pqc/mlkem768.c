/**
 * @file mlkem768.c
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "pqc/mlkem768.h"

//Check crypto library configuration
#if (MLKEM768_SUPPORT == ENABLED)

//Dependencies (liboqs)
#include <oqs/oqs.h>

//Common interface for key encapsulation mechanisms (KEM)
const KemAlgo mlkem768KemAlgo =
{
   "ML-KEM-768",
   MLKEM768_PUBLIC_KEY_LEN,
   MLKEM768_SECRET_KEY_LEN,
   MLKEM768_CIPHERTEXT_LEN,
   MLKEM768_SHARED_SECRET_LEN,
   (KemAlgoGenerateKeyPair) mlkem768GenerateKeyPair,
   (KemAlgoEncapsulate) mlkem768Encapsulate,
   (KemAlgoDecapsulate) mlkem768Decapsulate
};


/**
 * @brief Key pair generation
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[out] pk Public key
 * @param[out] sk Secret key
 * @return Error code
 **/

error_t mlkem768GenerateKeyPair(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *pk, uint8_t *sk)
{
   OQS_STATUS status;

   //Key pair generation
   status = OQS_KEM_kyber_768_keypair(pk, sk);

   //Return status code
   return (status == OQS_SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}


/**
 * @brief Encapsulation algorithm
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[out] ct Ciphertext
 * @param[out] ss Shared secret
 * @param[in] pk Public key
 * @return Error code
 **/

error_t mlkem768Encapsulate(const PrngAlgo *prngAlgo, void *prngContext,
   uint8_t *ct, uint8_t *ss, const uint8_t *pk)
{
   OQS_STATUS status;

   //Encapsulation algorithm
   status = OQS_KEM_kyber_768_encaps(ct, ss, pk);

   //Return status code
   return (status == OQS_SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}


/**
 * @brief Decapsulation algorithm
 * @param[out] ss Shared secret
 * @param[in] ct Ciphertext
 * @param[in] sk Secret key
 * @return Error code
 **/

error_t mlkem768Decapsulate(uint8_t *ss, const uint8_t *ct, const uint8_t *sk)
{
   OQS_STATUS status;

   //Decapsulation algorithm
   status = OQS_KEM_kyber_768_decaps(ss, ct, sk);

   //Return status code
   return (status == OQS_SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}

#endif
