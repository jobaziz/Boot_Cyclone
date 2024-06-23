/**
 * @file sha3_512.c
 * @brief SHA3-512 hash function (SHA-3 with 512-bit output)
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
#include "hash/sha3_512.h"

//Check crypto library configuration
#if (SHA3_512_SUPPORT == ENABLED)

//SHA3-512 object identifier (2.16.840.1.101.3.4.2.10)
const uint8_t SHA3_512_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x0A};

//Common interface for hash algorithms
const HashAlgo sha3_512HashAlgo =
{
   "SHA3-512",
   SHA3_512_OID,
   sizeof(SHA3_512_OID),
   sizeof(Sha3_512Context),
   SHA3_512_BLOCK_SIZE,
   SHA3_512_DIGEST_SIZE,
   SHA3_512_MIN_PAD_SIZE,
   FALSE,
   (HashAlgoCompute) sha3_512Compute,
   (HashAlgoInit) sha3_512Init,
   (HashAlgoUpdate) sha3_512Update,
   (HashAlgoFinal) sha3_512Final,
   NULL
};


/**
 * @brief Digest a message using SHA3-512
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t sha3_512Compute(const void *data, size_t length, uint8_t *digest)
{
#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   Sha3_512Context *context;
#else
   Sha3_512Context context[1];
#endif

   //Check parameters
   if(data == NULL && length != 0)
      return ERROR_INVALID_PARAMETER;

   if(digest == NULL)
      return ERROR_INVALID_PARAMETER;

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Allocate a memory buffer to hold the SHA3-512 context
   context = cryptoAllocMem(sizeof(Sha3_512Context));
   //Failed to allocate memory?
   if(context == NULL)
      return ERROR_OUT_OF_MEMORY;
#endif

   //Initialize the SHA3-512 context
   sha3_512Init(context);
   //Digest the message
   sha3_512Update(context, data, length);
   //Finalize the SHA3-512 message digest
   sha3_512Final(context, digest);

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Free previously allocated memory
   cryptoFreeMem(context);
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Initialize SHA3-512 message digest context
 * @param[in] context Pointer to the SHA3-512 context to initialize
 **/

void sha3_512Init(Sha3_512Context *context)
{
   //The capacity of the sponge is twice the digest length
   keccakInit(context, 2 * 512);
}


/**
 * @brief Update the SHA3-512 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA3-512 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void sha3_512Update(Sha3_512Context *context, const void *data, size_t length)
{
   //Absorb the input data
   keccakAbsorb(context, data, length);
}


/**
 * @brief Finish the SHA3-512 message digest
 * @param[in] context Pointer to the SHA3-512 context
 * @param[out] digest Calculated digest (optional parameter)
 **/

void sha3_512Final(Sha3_512Context *context, uint8_t *digest)
{
   //Finish absorbing phase (padding byte is 0x06 for SHA-3)
   keccakFinal(context, KECCAK_SHA3_PAD);
   //Extract data from the squeezing phase
   keccakSqueeze(context, digest, SHA3_512_DIGEST_SIZE);
}

#endif
