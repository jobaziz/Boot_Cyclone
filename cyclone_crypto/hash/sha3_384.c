/**
 * @file sha3_384.c
 * @brief SHA3-384 hash function (SHA-3 with 384-bit output)
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CRYPTO_TRACE_LEVEL

//Dependencies
#include "core/crypto.h"
#include "hash/sha3_384.h"

//Check crypto library configuration
#if (SHA3_384_SUPPORT == ENABLED)

//SHA3-384 object identifier (2.16.840.1.101.3.4.2.9)
const uint8_t SHA3_384_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x09};

//Common interface for hash algorithms
const HashAlgo sha3_384HashAlgo =
{
   "SHA3-384",
   SHA3_384_OID,
   sizeof(SHA3_384_OID),
   sizeof(Sha3_384Context),
   SHA3_384_BLOCK_SIZE,
   SHA3_384_DIGEST_SIZE,
   SHA3_384_MIN_PAD_SIZE,
   FALSE,
   (HashAlgoCompute) sha3_384Compute,
   (HashAlgoInit) sha3_384Init,
   (HashAlgoUpdate) sha3_384Update,
   (HashAlgoFinal) sha3_384Final,
   NULL
};


/**
 * @brief Digest a message using SHA3-384
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t sha3_384Compute(const void *data, size_t length, uint8_t *digest)
{
#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   Sha3_384Context *context;
#else
   Sha3_384Context context[1];
#endif

   //Check parameters
   if(data == NULL && length != 0)
      return ERROR_INVALID_PARAMETER;

   if(digest == NULL)
      return ERROR_INVALID_PARAMETER;

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Allocate a memory buffer to hold the SHA3-384 context
   context = cryptoAllocMem(sizeof(Sha3_384Context));
   //Failed to allocate memory?
   if(context == NULL)
      return ERROR_OUT_OF_MEMORY;
#endif

   //Initialize the SHA3-384 context
   sha3_384Init(context);
   //Digest the message
   sha3_384Update(context, data, length);
   //Finalize the SHA3-384 message digest
   sha3_384Final(context, digest);

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Free previously allocated memory
   cryptoFreeMem(context);
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Initialize SHA3-384 message digest context
 * @param[in] context Pointer to the SHA3-384 context to initialize
 **/

void sha3_384Init(Sha3_384Context *context)
{
   //The capacity of the sponge is twice the digest length
   keccakInit(context, 2 * 384);
}


/**
 * @brief Update the SHA3-384 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA3-384 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void sha3_384Update(Sha3_384Context *context, const void *data, size_t length)
{
   //Absorb the input data
   keccakAbsorb(context, data, length);
}


/**
 * @brief Finish the SHA3-384 message digest
 * @param[in] context Pointer to the SHA3-384 context
 * @param[out] digest Calculated digest (optional parameter)
 **/

void sha3_384Final(Sha3_384Context *context, uint8_t *digest)
{
   //Finish absorbing phase (padding byte is 0x06 for SHA-3)
   keccakFinal(context, KECCAK_SHA3_PAD);
   //Extract data from the squeezing phase
   keccakSqueeze(context, digest, SHA3_384_DIGEST_SIZE);
}

#endif
