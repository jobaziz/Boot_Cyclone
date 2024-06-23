/**
 * @file sha512_256.c
 * @brief SHA-512/256 (Secure Hash Algorithm)
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
#include "hash/sha512_256.h"

//Check crypto library configuration
#if (SHA512_256_SUPPORT == ENABLED)

//SHA-512/256 object identifier (2.16.840.1.101.3.4.2.6)
const uint8_t SHA512_256_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x06};

//Common interface for hash algorithms
const HashAlgo sha512_256HashAlgo =
{
   "SHA-512/256",
   SHA512_256_OID,
   sizeof(SHA512_256_OID),
   sizeof(Sha512_256Context),
   SHA512_256_BLOCK_SIZE,
   SHA512_256_DIGEST_SIZE,
   SHA512_256_MIN_PAD_SIZE,
   TRUE,
   (HashAlgoCompute) sha512_256Compute,
   (HashAlgoInit) sha512_256Init,
   (HashAlgoUpdate) sha512_256Update,
   (HashAlgoFinal) sha512_256Final,
   NULL
};


/**
 * @brief Digest a message using SHA-512/256
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

__weak_func error_t sha512_256Compute(const void *data, size_t length, uint8_t *digest)
{
#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   Sha512_256Context *context;
#else
   Sha512_256Context context[1];
#endif

   //Check parameters
   if(data == NULL && length != 0)
      return ERROR_INVALID_PARAMETER;

   if(digest == NULL)
      return ERROR_INVALID_PARAMETER;

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Allocate a memory buffer to hold the SHA-512/256 context
   context = cryptoAllocMem(sizeof(Sha512_256Context));
   //Failed to allocate memory?
   if(context == NULL)
      return ERROR_OUT_OF_MEMORY;
#endif

   //Initialize the SHA-512/256 context
   sha512_256Init(context);
   //Digest the message
   sha512_256Update(context, data, length);
   //Finalize the SHA-512/256 message digest
   sha512_256Final(context, digest);

#if (CRYPTO_STATIC_MEM_SUPPORT == DISABLED)
   //Free previously allocated memory
   cryptoFreeMem(context);
#endif

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Initialize SHA-512/256 message digest context
 * @param[in] context Pointer to the SHA-512/256 context to initialize
 **/

__weak_func void sha512_256Init(Sha512_256Context *context)
{
   //Set initial hash value
   context->h[0] = 0x22312194FC2BF72C;
   context->h[1] = 0x9F555FA3C84C64C2;
   context->h[2] = 0x2393B86B6F53B151;
   context->h[3] = 0x963877195940EABD;
   context->h[4] = 0x96283EE2A88EFFE3;
   context->h[5] = 0xBE5E1E2553863992;
   context->h[6] = 0x2B0199FC2C85B8AA;
   context->h[7] = 0x0EB72DDC81C52CA2;

   //Number of bytes in the buffer
   context->size = 0;
   //Total length of the message
   context->totalSize = 0;
}


/**
 * @brief Update the SHA-512/256 context with a portion of the message being hashed
 * @param[in] context Pointer to the SHA-512/256 context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

__weak_func void sha512_256Update(Sha512_256Context *context, const void *data, size_t length)
{
   //The function is defined in the exact same manner as SHA-512
   sha512Update(context, data, length);
}


/**
 * @brief Finish the SHA-512/256 message digest
 * @param[in] context Pointer to the SHA-512/256 context
 * @param[out] digest Calculated digest (optional parameter)
 **/

__weak_func void sha512_256Final(Sha512_256Context *context, uint8_t *digest)
{
   //The function is defined in the exact same manner as SHA-512
   sha512Final(context, NULL);

   //Copy the resulting digest
   if(digest != NULL)
   {
      osMemcpy(digest, context->digest, SHA512_256_DIGEST_SIZE);
   }
}

#endif
