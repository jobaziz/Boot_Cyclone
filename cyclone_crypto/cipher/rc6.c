/**
 * @file rc6.c
 * @brief RC6-32/20 block cipher
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
#include "cipher/rc6.h"

//Check crypto library configuration
#if (RC6_SUPPORT == ENABLED)

//RC6 magic constants
#define P32 0xB7E15163
#define Q32 0x9E3779B9

//Common interface for encryption algorithms
const CipherAlgo rc6CipherAlgo =
{
   "RC6",
   sizeof(Rc6Context),
   CIPHER_ALGO_TYPE_BLOCK,
   RC6_BLOCK_SIZE,
   (CipherAlgoInit) rc6Init,
   NULL,
   NULL,
   (CipherAlgoEncryptBlock) rc6EncryptBlock,
   (CipherAlgoDecryptBlock) rc6DecryptBlock,
   (CipherAlgoDeinit) rc6Deinit
};


/**
 * @brief Initialize a RC6 context using the supplied key
 * @param[in] context Pointer to the RC6 context to initialize
 * @param[in] key Pointer to the key
 * @param[in] keyLen Length of the key
 * @return Error code
 **/

error_t rc6Init(Rc6Context *context, const uint8_t *key, size_t keyLen)
{
   uint_t c;
   uint_t i;
   uint_t j;
   uint_t s;
   uint_t v;
   uint32_t a;
   uint32_t b;

   //Check parameters
   if(context == NULL || key == NULL)
      return ERROR_INVALID_PARAMETER;

   //Invalid key length?
   if(keyLen > RC6_MAX_KEY_SIZE)
      return ERROR_INVALID_KEY_LENGTH;

   //Convert the secret key from bytes to words
   osMemset(context->l, 0, RC6_MAX_KEY_SIZE);
   osMemcpy(context->l, key, keyLen);

   //Calculate the length of the key in words
   c = (keyLen > 0) ? (keyLen + 3) / 4 : 1;

   //Initialize the first element of S
   context->s[0] = P32;

   //Initialize array S to a particular fixed pseudo random bit pattern
   for(i = 1; i < (2 * RC6_NB_ROUNDS + 4); i++)
   {
      context->s[i] = context->s[i - 1] + Q32;
   }

   //Initialize variables
   i = 0;
   j = 0;
   a = 0;
   b = 0;

   //Number of iterations
   v = 3 * MAX(c, 2 * RC6_NB_ROUNDS + 4);

   //Key expansion
   for(s = 0; s < v; s++)
   {
      context->s[i] += a + b;
      context->s[i] = ROL32(context->s[i], 3);
      a = context->s[i];

      context->l[j] += a + b;
      context->l[j] = ROL32(context->l[j], (a + b) % 32);
      b = context->l[j];

      if(++i >= (2 * RC6_NB_ROUNDS + 4))
      {
         i = 0;
      }

      if(++j >= c)
      {
         j = 0;
      }
   }

   //No error to report
   return NO_ERROR;
}


/**
 * @brief Encrypt a 16-byte block using RC6 algorithm
 * @param[in] context Pointer to the RC6 context
 * @param[in] input Plaintext block to encrypt
 * @param[out] output Ciphertext block resulting from encryption
 **/

void rc6EncryptBlock(Rc6Context *context, const uint8_t *input,
   uint8_t *output)
{
   uint_t i;
   uint32_t t;
   uint32_t u;

   //Load the 4 working registers with the plaintext
   uint32_t a = LOAD32LE(input + 0);
   uint32_t b = LOAD32LE(input + 4);
   uint32_t c = LOAD32LE(input + 8);
   uint32_t d = LOAD32LE(input + 12);

   //First, update B and D
   b += context->s[0];
   d += context->s[1];

   //Apply 20 rounds
   for(i = 1; i <= RC6_NB_ROUNDS; i++)
   {
      t = (b * (2 * b + 1));
      t = ROL32(t, 5);

      u = (d * (2 * d + 1));
      u = ROL32(u, 5);

      a ^= t;
      a = ROL32(a, u % 32) + context->s[2 * i];

      c ^= u;
      c = ROL32(c, t % 32) + context->s[2 * i + 1];

      t = a;
      a = b;
      b = c;
      c = d;
      d = t;
   }

   //Update A and C
   a += context->s[2 * RC6_NB_ROUNDS + 2];
   c += context->s[2 * RC6_NB_ROUNDS + 3];

   //The resulting value is the ciphertext
   STORE32LE(a, output + 0);
   STORE32LE(b, output + 4);
   STORE32LE(c, output + 8);
   STORE32LE(d, output + 12);
}


/**
 * @brief Decrypt a 16-byte block using RC6 algorithm
 * @param[in] context Pointer to the RC6 context
 * @param[in] input Ciphertext block to decrypt
 * @param[out] output Plaintext block resulting from decryption
 **/

void rc6DecryptBlock(Rc6Context *context, const uint8_t *input,
   uint8_t *output)
{
   uint_t i;
   uint32_t t;
   uint32_t u;

   //Load the 4 working registers with the ciphertext
   uint32_t a = LOAD32LE(input + 0);
   uint32_t b = LOAD32LE(input + 4);
   uint32_t c = LOAD32LE(input + 8);
   uint32_t d = LOAD32LE(input + 12);

   //First, update C and A
   c -= context->s[2 * RC6_NB_ROUNDS + 3];
   a -= context->s[2 * RC6_NB_ROUNDS + 2];

   //Apply 20 rounds
   for(i = RC6_NB_ROUNDS; i > 0; i--)
   {
      t = d;
      d = c;
      c = b;
      b = a;
      a = t;

      u = (d * (2 * d + 1));
      u = ROL32(u, 5);

      t = (b * (2 * b + 1));
      t = ROL32(t, 5);

      c -= context->s[2 * i + 1];
      c = ROR32(c, t % 32) ^ u;

      a -= context->s[2 * i];
      a = ROR32(a, u % 32) ^ t;
   }

   //Update D and B
   d -= context->s[1];
   b -= context->s[0];

   //The resulting value is the plaintext
   STORE32LE(a, output + 0);
   STORE32LE(b, output + 4);
   STORE32LE(c, output + 8);
   STORE32LE(d, output + 12);
}


/**
 * @brief Release RC6 context
 * @param[in] context Pointer to the RC6 context
 **/

void rc6Deinit(Rc6Context *context)
{
   //Clear RC6 context
   osMemset(context, 0, sizeof(Rc6Context));
}

#endif
