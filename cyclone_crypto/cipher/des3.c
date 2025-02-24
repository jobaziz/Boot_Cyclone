/**
 * @file des3.c
 * @brief Triple DES (Triple Data Encryption Algorithm)
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
#include "cipher/des3.h"
#include "cipher/des.h"

//Check crypto library configuration
#if (DES3_SUPPORT == ENABLED)

//DES-EDE3-CBC OID (1.2.840.113549.3.7)
const uint8_t DES_EDE3_CBC_OID[8] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x03, 0x07};

//Common interface for encryption algorithms
const CipherAlgo des3CipherAlgo =
{
   "3DES",
   sizeof(Des3Context),
   CIPHER_ALGO_TYPE_BLOCK,
   DES3_BLOCK_SIZE,
   (CipherAlgoInit) des3Init,
   NULL,
   NULL,
   (CipherAlgoEncryptBlock) des3EncryptBlock,
   (CipherAlgoDecryptBlock) des3DecryptBlock,
   (CipherAlgoDeinit) des3Deinit
};


/**
 * @brief Initialize a Triple DES context using the supplied key
 * @param[in] context Pointer to the Triple DES context to initialize
 * @param[in] key Pointer to the key
 * @param[in] keyLen Length of the key
 * @return Error code
 **/

__weak_func error_t des3Init(Des3Context *context, const uint8_t *key,
   size_t keyLen)
{
   //Check parameters
   if(context == NULL || key == NULL)
      return ERROR_INVALID_PARAMETER;

   //Check key length
   if(keyLen == 8)
   {
      //This option provides backward compatibility with DES, because the
      //first and second DES operations cancel out
      desInit(&context->k1, key, 8);
      desInit(&context->k2, key, 8);
      desInit(&context->k3, key, 8);
   }
   else if(keyLen == 16)
   {
      //If the key length is 128 bits including parity, the first 8 bytes of the
      //encoding represent the key used for the two outer DES operations, and
      //the second 8 bytes represent the key used for the inner DES operation
      desInit(&context->k1, key, 8);
      desInit(&context->k2, key + 8, 8);
      desInit(&context->k3, key, 8);
   }
   else if(keyLen == 24)
   {
      //If the key length is 192 bits including parity, then 3 independent DES
      //keys are represented, in the order in which they are used for encryption
      desInit(&context->k1, key, 8);
      desInit(&context->k2, key + 8, 8);
      desInit(&context->k3, key + 16, 8);
   }
   else
   {
      //The length of the key is not valid
      return ERROR_INVALID_KEY_LENGTH;
   }

   //No error to report
   return NO_ERROR;
}


/**
 * @brief Encrypt a 8-byte block using Triple DES algorithm
 * @param[in] context Pointer to the Triple DES context
 * @param[in] input Plaintext block to encrypt
 * @param[out] output Ciphertext block resulting from encryption
 **/

__weak_func void des3EncryptBlock(Des3Context *context, const uint8_t *input,
   uint8_t *output)
{
   //The first pass is a DES encryption
   desEncryptBlock(&context->k1, input, output);
   //The second pass is a DES decryption of the first ciphertext result
   desDecryptBlock(&context->k2, output, output);
   //The third pass is a DES encryption of the second pass result
   desEncryptBlock(&context->k3, output, output);
}


/**
 * @brief Decrypt a 8-byte block using Triple DES algorithm
 * @param[in] context Pointer to the Triple DES context
 * @param[in] input Ciphertext block to decrypt
 * @param[out] output Plaintext block resulting from decryption
 **/

__weak_func void des3DecryptBlock(Des3Context *context, const uint8_t *input,
   uint8_t *output)
{
   //The first pass is a DES decryption
   desDecryptBlock(&context->k3, input, output);
   //The second pass is a DES encryption of the first pass result
   desEncryptBlock(&context->k2, output, output);
   //The third pass is a DES decryption of the second ciphertext result
   desDecryptBlock(&context->k1, output, output);
}


/**
 * @brief Release Triple DES context
 * @param[in] context Pointer to the Triple DES context
 **/

__weak_func void des3Deinit(Des3Context *context)
{
   //Clear Triple DES context
   osMemset(context, 0, sizeof(Des3Context));
}

#endif
