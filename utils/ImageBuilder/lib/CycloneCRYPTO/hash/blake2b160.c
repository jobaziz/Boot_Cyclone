/**
 * @file blake2b160.c
 * @brief BLAKE2b-160 hash function
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
#include "hash/blake2b160.h"

//Check crypto library configuration
#if (BLAKE2B160_SUPPORT == ENABLED)

//BLAKE2b-160 object identifier (1.3.6.1.4.1.1722.12.2.1.5)
const uint8_t BLAKE2B160_OID[11] = {0x43, 0x06, 0x01, 0x04, 0x01, 0x8D, 0x3A, 0x0C, 0x02, 0x01, 0x05};

//Common interface for hash algorithms
const HashAlgo blake2b160HashAlgo =
{
   "BLAKE2b-160",
   BLAKE2B160_OID,
   sizeof(BLAKE2B160_OID),
   sizeof(Blake2b160Context),
   BLAKE2B160_BLOCK_SIZE,
   BLAKE2B160_DIGEST_SIZE,
   BLAKE2B160_MIN_PAD_SIZE,
   FALSE,
   (HashAlgoCompute) blake2b160Compute,
   (HashAlgoInit) blake2b160Init,
   (HashAlgoUpdate) blake2b160Update,
   (HashAlgoFinal) blake2b160Final,
   NULL
};


/**
 * @brief Digest a message using BLAKE2b-160
 * @param[in] data Pointer to the message being hashed
 * @param[in] length Length of the message
 * @param[out] digest Pointer to the calculated digest
 * @return Error code
 **/

error_t blake2b160Compute(const void *data, size_t length, uint8_t *digest)
{
   //Compute the unkeyed hash with BLAKE2b-160
   return blake2bCompute(NULL, 0, data, length, digest, BLAKE2B160_DIGEST_SIZE);
}


/**
 * @brief Initialize BLAKE2b-160 hash computation
 * @param[in] context Pointer to the BLAKE2b context to initialize
 **/

void blake2b160Init(Blake2b160Context *context)
{
   //Initialize the hashing context
   blake2bInit(context, NULL, 0, BLAKE2B160_DIGEST_SIZE);
}


/**
 * @brief Update BLAKE2b-160 hash computation
 * @param[in] context Pointer to the BLAKE2b context
 * @param[in] data Pointer to the buffer being hashed
 * @param[in] length Length of the buffer
 **/

void blake2b160Update(Blake2b160Context *context, const void *data, size_t length)
{
   //Digest the data
   blake2bUpdate(context, data, length);
}


/**
 * @brief Finish BLAKE2b-160 hash computation
 * @param[in] context Pointer to the BLAKE2b context
 * @param[out] digest Calculated digest (optional parameter)
 **/

void blake2b160Final(Blake2b160Context *context, uint8_t *digest)
{
   //Generate the message digest
   blake2bFinal(context, digest);
}

#endif
