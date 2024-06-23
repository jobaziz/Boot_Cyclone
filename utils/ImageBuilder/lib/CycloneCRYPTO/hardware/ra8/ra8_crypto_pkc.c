/**
 * @file ra8_crypto_pkc.c
 * @brief RA8 public-key hardware accelerator
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
#include "hw_sce_private.h"
#include "hw_sce_ra_private.h"
#include "hw_sce_rsa_private.h"
#include "hw_sce_ecc_private.h"
#include "core/crypto.h"
#include "hardware/ra8/ra8_crypto.h"
#include "hardware/ra8/ra8_crypto_pkc.h"
#include "pkc/rsa.h"
#include "ecc/ec.h"
#include "ecc/ecdsa.h"
#include "debug.h"

//Check crypto library configuration
#if (RA8_CRYPTO_PKC_SUPPORT == ENABLED)

//Global variables
extern const uint32_t sce_oem_key_size[SCE_OEM_CMD_NUM];
static Ra8RsaArgs rsaArgs;
static Ra8EcArgs ecArgs;


/**
 * @brief Modular exponentiation (fast calculation)
 * @param[out] r Resulting integer R = A ^ E mod P
 * @param[in] a Pointer to a multiple precision integer
 * @param[in] e Exponent
 * @param[in] p Modulus
 * @return Error code
 **/

error_t mpiExpModFast(Mpi *r, const Mpi *a, const Mpi *e, const Mpi *p)
{
   error_t error;
   fsp_err_t status;
   size_t aLen;
   size_t eLen;
   size_t pLen;
   sce_oem_cmd_t command;

   //Retrieve the length of the integer, in bytes
   aLen = mpiGetByteLength(a);
   //Retrieve the length of the exponent, in bytes
   eLen = mpiGetByteLength(e);
   //Retrieve the length of the modulus, in bytes
   pLen = mpiGetByteLength(p);

   //The accelerator supports operand lengths up to 4096 bits
   if((aLen <= 128 && eLen <= 4 && pLen == 128) ||
      (aLen <= 256 && eLen <= 4 && pLen == 256) ||
      (aLen <= 384 && eLen <= 4 && pLen == 384) ||
      (aLen <= 512 && eLen <= 4 && pLen == 512))
   {
      //Select appropriate scalar length
      if(pLen == 128)
      {
         command = SCE_OEM_CMD_RSA1024_PUBLIC;
      }
      else if(pLen == 256)
      {
         command = SCE_OEM_CMD_RSA2048_PUBLIC;
      }
      else if(pLen == 384)
      {
         command = SCE_OEM_CMD_RSA3072_PUBLIC;
      }
      else
      {
         command = SCE_OEM_CMD_RSA4096_PUBLIC;
      }

      //Acquire exclusive access to the RSIP7 module
      osAcquireMutex(&ra8CryptoMutex);

      //Format message representative
      mpiWriteRaw(a, (uint8_t *) rsaArgs.m, pLen);

      //Format public key
      mpiWriteRaw(p, (uint8_t *) rsaArgs.key, pLen);
      mpiWriteRaw(e, (uint8_t *) rsaArgs.key + pLen, 4);

      //Install the plaintext public key and get the wrapped key
      status = HW_SCE_GenerateOemKeyIndexPrivate(SCE_OEM_KEY_TYPE_PLAIN,
         command, NULL, NULL, (uint8_t *) rsaArgs.key, rsaArgs.wrappedKey);

      //Check status code
      if(status == FSP_SUCCESS)
      {
         //Perform RSA encryption
         if(pLen == 128)
         {
            status = HW_SCE_Rsa1024ModularExponentEncryptSub(rsaArgs.wrappedKey,
               rsaArgs.m, rsaArgs.c);
         }
         else if(pLen == 256)
         {
            status = HW_SCE_Rsa2048ModularExponentEncryptSub(rsaArgs.wrappedKey,
               rsaArgs.m, rsaArgs.c);
         }
         else if(pLen == 384)
         {
            status = HW_SCE_Rsa3072ModularExponentEncryptSub(rsaArgs.wrappedKey,
               rsaArgs.m, rsaArgs.c);
         }
         else if(pLen == 512)
         {
            status = HW_SCE_Rsa4096ModularExponentEncryptSub(rsaArgs.wrappedKey,
               rsaArgs.m, rsaArgs.c);
         }
         else
         {
            status = FSP_ERR_CRYPTO_NOT_IMPLEMENTED;
         }
      }

      //Check status code
      if(status == FSP_SUCCESS)
      {
         //Copy the ciphertext representative
         error = mpiReadRaw(r, (uint8_t *) rsaArgs.c, pLen);
      }
      else
      {
         //Report an error
         error = ERROR_FAILURE;
      }

      //Release exclusive access to the RSIP7 module
      osReleaseMutex(&ra8CryptoMutex);
   }
   else
   {
      //Perform modular exponentiation (r = a ^ e mod p)
      error = mpiExpModRegular(r, a, e, p);
   }

   //Return status code
   return error;
}


/**
 * @brief Modular exponentiation (regular calculation)
 * @param[out] r Resulting integer R = A ^ E mod P
 * @param[in] a Pointer to a multiple precision integer
 * @param[in] e Exponent
 * @param[in] p Modulus
 * @return Error code
 **/

error_t mpiExpModRegular(Mpi *r, const Mpi *a, const Mpi *e, const Mpi *p)
{
   error_t error;
   fsp_err_t status;
   size_t aLen;
   size_t eLen;
   size_t pLen;
   sce_oem_cmd_t command;

   //Retrieve the length of the integer, in bytes
   aLen = mpiGetByteLength(a);
   //Retrieve the length of the exponent, in bytes
   eLen = mpiGetByteLength(e);
   //Retrieve the length of the modulus, in bytes
   pLen = mpiGetByteLength(p);

   //The accelerator supports operand lengths up to 4096 bits
   if((aLen <= 128 && eLen <= 128 && pLen == 128) ||
      (aLen <= 256 && eLen <= 256 && pLen == 256) ||
      (aLen <= 384 && eLen <= 384 && pLen == 384) ||
      (aLen <= 512 && eLen <= 512 && pLen == 512))
   {
      //Select appropriate scalar length
      if(pLen == 128)
      {
         command = SCE_OEM_CMD_RSA1024_PRIVATE;
      }
      else if(pLen == 256)
      {
         command = SCE_OEM_CMD_RSA2048_PRIVATE;
      }
      else if(pLen == 384)
      {
         command = SCE_OEM_CMD_RSA3072_PRIVATE;
      }
      else
      {
         command = SCE_OEM_CMD_RSA4096_PRIVATE;
      }

      //Acquire exclusive access to the RSIP7 module
      osAcquireMutex(&ra8CryptoMutex);

      //Format ciphertext representative
      mpiWriteRaw(a, (uint8_t *) rsaArgs.c, pLen);

      //Format private key
      mpiWriteRaw(p, (uint8_t *) rsaArgs.key, pLen);
      mpiWriteRaw(e, (uint8_t *) rsaArgs.key + pLen, pLen);

      //Install the plaintext private key and get the wrapped key
      status = HW_SCE_GenerateOemKeyIndexPrivate(SCE_OEM_KEY_TYPE_PLAIN,
         command, NULL, NULL, (uint8_t *) rsaArgs.key, rsaArgs.wrappedKey);

      //Check status code
      if(status == FSP_SUCCESS)
      {
         //Perform RSA decryption
         if(pLen == 128)
         {
            status = HW_SCE_Rsa1024ModularExponentDecryptSub(rsaArgs.wrappedKey,
               rsaArgs.c, rsaArgs.m);
         }
         else if(pLen == 256)
         {
            status = HW_SCE_Rsa2048ModularExponentDecryptSub(rsaArgs.wrappedKey,
               rsaArgs.c, rsaArgs.m);
         }
         else if(pLen == 384)
         {
            status = HW_SCE_Rsa3072ModularExponentDecryptSub(rsaArgs.wrappedKey,
               rsaArgs.c, rsaArgs.m);
         }
         else if(pLen == 512)
         {
            status = HW_SCE_Rsa4096ModularExponentDecryptSub(rsaArgs.wrappedKey,
               rsaArgs.c, rsaArgs.m);
         }
         else
         {
            status = FSP_ERR_CRYPTO_NOT_IMPLEMENTED;
         }
      }

      //Check status code
      if(status == FSP_SUCCESS)
      {
         //Copy the message representative
         error = mpiReadRaw(r, (uint8_t *) rsaArgs.m, pLen);
      }
      else
      {
         //Report an error
         error = ERROR_FAILURE;
      }

      //Release exclusive access to the RSIP7 module
      osReleaseMutex(&ra8CryptoMutex);
   }
   else
   {
      //Perform modular exponentiation (r = a ^ e mod p)
      error = mpiExpMod(r, a, e, p);
   }

   //Return status code
   return error;
}


/**
 * @brief RSA decryption primitive
 *
 * The RSA decryption primitive recovers the message representative from
 * the ciphertext representative under the control of a private key
 *
 * @param[in] key RSA private key
 * @param[in] c Ciphertext representative
 * @param[out] m Message representative
 * @return Error code
 **/

error_t rsadp(const RsaPrivateKey *key, const Mpi *c, Mpi *m)
{
   error_t error;
   size_t nLen;
   size_t dLen;
   size_t pLen;
   size_t qLen;
   size_t dpLen;
   size_t dqLen;
   size_t qinvLen;

   //Retrieve the length of the private key
   nLen = mpiGetByteLength(&key->n);
   dLen = mpiGetByteLength(&key->d);
   pLen = mpiGetByteLength(&key->p);
   qLen = mpiGetByteLength(&key->q);
   dpLen = mpiGetByteLength(&key->dp);
   dqLen = mpiGetByteLength(&key->dq);
   qinvLen = mpiGetByteLength(&key->qinv);

   //Sanity check
   if(nLen == 0)
      return ERROR_INVALID_PARAMETER;

   //The ciphertext representative c shall be between 0 and n - 1
   if(mpiCompInt(c, 0) < 0 || mpiComp(c, &key->n) >= 0)
      return ERROR_OUT_OF_RANGE;

   //Check the length of the private key
   if((nLen == 128 && dLen <= 128) || (nLen == 384 && dLen <= 384))
   {
      //Let m = c ^ d mod n
      error = mpiExpModRegular(m, c, &key->d, &key->n);
   }
   else if(nLen > 0 && pLen > 0 && qLen > 0 && dpLen > 0 && dqLen > 0 &&
      qinvLen > 0)
   {
      Mpi m1;
      Mpi m2;
      Mpi h;

      //Initialize multiple-precision integers
      mpiInit(&m1);
      mpiInit(&m2);
      mpiInit(&h);

      //Compute m1 = c ^ dP mod p
      error = mpiMod(&m1, c, &key->p);

      if(!error)
      {
         error = mpiExpModRegular(&m1, &m1, &key->dp, &key->p);
      }

      //Compute m2 = c ^ dQ mod q
      if(!error)
      {
         error = mpiMod(&m2, c, &key->q);
      }

      if(!error)
      {
         error = mpiExpModRegular(&m2, &m2, &key->dq, &key->q);
      }

      //Let h = (m1 - m2) * qInv mod p
      if(!error)
      {
         error = mpiSub(&h, &m1, &m2);
      }

      if(!error)
      {
         error = mpiMulMod(&h, &h, &key->qinv, &key->p);
      }

      //Let m = m2 + q * h
      if(!error)
      {
         error = mpiMul(m, &key->q, &h);
      }

      if(!error)
      {
         error = mpiAdd(m, m, &m2);
      }

      //Free previously allocated memory
      mpiFree(&m1);
      mpiFree(&m2);
      mpiFree(&h);
   }
   else if(nLen > 0 && dLen > 0)
   {
      //Let m = c ^ d mod n
      error = mpiExpModRegular(m, c, &key->d, &key->n);
   }
   else
   {
      //Report an error
      error = ERROR_INVALID_PARAMETER;
   }

   //Return status code
   return error;
}


/**
 * @brief Scalar multiplication
 * @param[in] params EC domain parameters
 * @param[out] r Resulting point R = d.S
 * @param[in] d An integer d such as 0 <= d < p
 * @param[in] s EC point
 * @return Error code
 **/

error_t ecMult(const EcDomainParameters *params, EcPoint *r, const Mpi *d,
   const EcPoint *s)
{
   error_t error;
   fsp_err_t status;
   size_t n;
   uint32_t curveType;
   uint32_t command;
   sce_oem_cmd_t oemCommand;
   const uint32_t *domainParams;

   //Check elliptic curve parameters
   if(!osStrcmp(params->name, "secp256k1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_KOBLITZ;
      oemCommand = SCE_OEM_CMD_ECC_SECP256K1_PRIVATE;
      domainParams = DomainParam_Koblitz_secp256k1;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "secp256r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P256_PRIVATE;
      domainParams = DomainParam_NIST_P256;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "secp384r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P384_PRIVATE;
      domainParams = DomainParam_NIST_P384;
      command = 0;
      n = 48;
   }
   else if(!osStrcmp(params->name, "secp521r1") &&
      sce_oem_key_size[SCE_OEM_CMD_ECC_P521_PRIVATE] != 0)
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P521_PRIVATE;
      domainParams = DomainParam_NIST_P521;
      command = 0;
      n = 80;
   }
   else if(!osStrcmp(params->name, "brainpoolP256r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P256R1_PRIVATE;
      domainParams = DomainParam_Brainpool_256r1;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "brainpoolP384r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P384R1_PRIVATE;
      domainParams = DomainParam_Brainpool_384r1;
      command = 0;
      n = 48;
   }
   else if(!osStrcmp(params->name, "brainpoolP512r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P512R1_PRIVATE;
      domainParams = DomainParam_Brainpool_512r1;
      command = 0;
      n = 64;
   }
   else
   {
      return ERROR_FAILURE;
   }

   //Acquire exclusive access to the RSIP7 module
   osAcquireMutex(&ra8CryptoMutex);

   //Set scalar value
   mpiWriteRaw(d, (uint8_t *) ecArgs.d, n);

   //Set input point
   mpiWriteRaw(&s->x, (uint8_t *) ecArgs.g, n);
   mpiWriteRaw(&s->y, (uint8_t *) ecArgs.g + n, n);

   //Install the plaintext private key and get the wrapped key
   status = HW_SCE_GenerateOemKeyIndexPrivate(SCE_OEM_KEY_TYPE_PLAIN,
      oemCommand, NULL, NULL, (uint8_t *) ecArgs.d, ecArgs.wrappedKey);

   //Check status code
   if(status == FSP_SUCCESS)
   {
      //Perform scalar multiplication
      if(n == 32)
      {
         status = HW_SCE_Ecc256ScalarMultiplicationSub(&curveType,
            &command, ecArgs.wrappedKey, ecArgs.g, domainParams, ecArgs.q);
      }
      else if(n == 48)
      {
         status = HW_SCE_Ecc384ScalarMultiplicationSub(&curveType,
            ecArgs.wrappedKey, ecArgs.g, domainParams, ecArgs.q);
      }
      else if(n == 64)
      {
         status = HW_SCE_Ecc512ScalarMultiplicationSub(ecArgs.wrappedKey,
            ecArgs.g, domainParams, ecArgs.q);
      }
      else if(n == 80)
      {
         status = HW_SCE_Ecc521ScalarMultiplicationSub(ecArgs.wrappedKey,
            ecArgs.g, domainParams, ecArgs.q);
      }
      else
      {
         status = FSP_ERR_CRYPTO_NOT_IMPLEMENTED;
      }
   }

   //Check status code
   if(status == FSP_SUCCESS)
   {
      //Copy the x-coordinate of the result
      error = mpiReadRaw(&r->x, (uint8_t *) ecArgs.q, n);

      //Check status code
      if(!error)
      {
         //Copy the y-coordinate of the result
         error = mpiReadRaw(&r->y, (uint8_t *) ecArgs.q + n, n);
      }

      //Check status code
      if(!error)
      {
         //Set the z-coordinate of the result
         error = mpiSetValue(&r->z, 1);
      }
   }
   else
   {
      //Report an error
      error = ERROR_FAILURE;
   }

   //Release exclusive access to the RSIP7 module
   osReleaseMutex(&ra8CryptoMutex);

   //Return status code
   return error;
}


/**
 * @brief ECDSA signature generation
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[in] params EC domain parameters
 * @param[in] privateKey Signer's EC private key
 * @param[in] digest Digest of the message to be signed
 * @param[in] digestLen Length in octets of the digest
 * @param[out] signature (R, S) integer pair
 * @return Error code
 **/

error_t ecdsaGenerateSignature(const PrngAlgo *prngAlgo, void *prngContext,
   const EcDomainParameters *params, const EcPrivateKey *privateKey,
   const uint8_t *digest, size_t digestLen, EcdsaSignature *signature)
{
   error_t error;
   fsp_err_t status;
   size_t n;
   size_t orderLen;
   uint32_t curveType;
   uint32_t command;
   sce_oem_cmd_t oemCommand;
   const uint32_t *domainParams;

   //Check parameters
   if(params == NULL || privateKey == NULL || digest == NULL || signature == NULL)
      return ERROR_INVALID_PARAMETER;

   //Retrieve the length of the base point order, in bytes
   orderLen = mpiGetByteLength(&params->q);

   //Check elliptic curve parameters
   if(!osStrcmp(params->name, "secp256k1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_KOBLITZ;
      oemCommand = SCE_OEM_CMD_ECC_SECP256K1_PRIVATE;
      domainParams = DomainParam_Koblitz_secp256k1;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "secp256r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P256_PRIVATE;
      domainParams = DomainParam_NIST_P256;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "secp384r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P384_PRIVATE;
      domainParams = DomainParam_NIST_P384;
      command = 0;
      n = 48;
   }
   else if(!osStrcmp(params->name, "secp521r1") &&
      sce_oem_key_size[SCE_OEM_CMD_ECC_P521_PRIVATE] != 0)
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P521_PRIVATE;
      domainParams = DomainParam_NIST_P521;
      command = 0;
      n = 80;
   }
   else if(!osStrcmp(params->name, "brainpoolP256r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P256R1_PRIVATE;
      domainParams = DomainParam_Brainpool_256r1;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "brainpoolP384r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P384R1_PRIVATE;
      domainParams = DomainParam_Brainpool_384r1;
      command = 0;
      n = 48;
   }
   else if(!osStrcmp(params->name, "brainpoolP512r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P512R1_PRIVATE;
      domainParams = DomainParam_Brainpool_512r1;
      command = 0;
      n = 64;
   }
   else
   {
      return ERROR_FAILURE;
   }

   //Keep the leftmost bits of the hash value
   digestLen = MIN(digestLen, orderLen);

   //Acquire exclusive access to the RSIP7 module
   osAcquireMutex(&ra8CryptoMutex);

   //Pad the digest with leading zeroes if necessary
   osMemset(ecArgs.digest, 0, n);
   osMemcpy((uint8_t *) ecArgs.digest + n - digestLen, digest, digestLen);

   //Set private key
   mpiWriteRaw(&privateKey->d, (uint8_t *) ecArgs.d, n);

   //Install the plaintext private key and get the wrapped key
   status = HW_SCE_GenerateOemKeyIndexPrivate(SCE_OEM_KEY_TYPE_PLAIN,
      oemCommand, NULL, NULL, (uint8_t *) ecArgs.d, ecArgs.wrappedKey);

   //Check status code
   if(status == FSP_SUCCESS)
   {
      //Verify ECDSA signature
      if(n == 32)
      {
         status = HW_SCE_EcdsaSignatureGenerateSub(&curveType, &command,
            ecArgs.wrappedKey, ecArgs.digest, domainParams, ecArgs.signature);
      }
      else if(n == 48)
      {
         status = HW_SCE_EcdsaP384SignatureGenerateSub(&curveType,
            ecArgs.wrappedKey, ecArgs.digest, domainParams, ecArgs.signature);
      }
      else if(n == 64)
      {
         status = HW_SCE_EcdsaP512SignatureGenerateSub(ecArgs.wrappedKey,
            ecArgs.digest, domainParams, ecArgs.signature);
      }
      else if(n == 80)
      {
         status = HW_SCE_EcdsaP521SignatureGenerateSub(ecArgs.wrappedKey,
            ecArgs.digest, domainParams, ecArgs.signature);
      }
      else
      {
         status = FSP_ERR_CRYPTO_NOT_IMPLEMENTED;
      }
   }

   //Check status code
   if(status == FSP_SUCCESS)
   {
      //Copy integer R
      error = mpiReadRaw(&signature->r, (uint8_t *) ecArgs.signature, n);

      //Check status code
      if(!error)
      {
         //Copy integer S
         error = mpiReadRaw(&signature->s, (uint8_t *) ecArgs.signature + n, n);
      }
   }
   else
   {
      //Report an error
      error = ERROR_FAILURE;
   }

   //Release exclusive access to the RSIP7 module
   osReleaseMutex(&ra8CryptoMutex);

   //Return status code
   return error;
}


/**
 * @brief ECDSA signature verification
 * @param[in] params EC domain parameters
 * @param[in] publicKey Signer's EC public key
 * @param[in] digest Digest of the message whose signature is to be verified
 * @param[in] digestLen Length in octets of the digest
 * @param[in] signature (R, S) integer pair
 * @return Error code
 **/

error_t ecdsaVerifySignature(const EcDomainParameters *params,
   const EcPublicKey *publicKey, const uint8_t *digest, size_t digestLen,
   const EcdsaSignature *signature)
{
   fsp_err_t status;
   size_t n;
   size_t orderLen;
   uint32_t curveType;
   uint32_t command;
   sce_oem_cmd_t oemCommand;
   const uint32_t *domainParams;

   //Check parameters
   if(params == NULL || publicKey == NULL || digest == NULL || signature == NULL)
      return ERROR_INVALID_PARAMETER;

   //The verifier shall check that 0 < r < q
   if(mpiCompInt(&signature->r, 0) <= 0 ||
      mpiComp(&signature->r, &params->q) >= 0)
   {
      //If the condition is violated, the signature shall be rejected as invalid
      return ERROR_INVALID_SIGNATURE;
   }

   //The verifier shall check that 0 < s < q
   if(mpiCompInt(&signature->s, 0) <= 0 ||
      mpiComp(&signature->s, &params->q) >= 0)
   {
      //If the condition is violated, the signature shall be rejected as invalid
      return ERROR_INVALID_SIGNATURE;
   }

   //Retrieve the length of the base point order, in bytes
   orderLen = mpiGetByteLength(&params->q);

   //Check elliptic curve parameters
   if(!osStrcmp(params->name, "secp256k1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_KOBLITZ;
      oemCommand = SCE_OEM_CMD_ECC_SECP256K1_PUBLIC;
      domainParams = DomainParam_Koblitz_secp256k1;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "secp256r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P256_PUBLIC;
      domainParams = DomainParam_NIST_P256;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "secp384r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P384_PUBLIC;
      domainParams = DomainParam_NIST_P384;
      command = 0;
      n = 48;
   }
   else if(!osStrcmp(params->name, "secp521r1") &&
      sce_oem_key_size[SCE_OEM_CMD_ECC_P521_PUBLIC] != 0)
   {
      curveType = SCE_ECC_CURVE_TYPE_NIST;
      oemCommand = SCE_OEM_CMD_ECC_P521_PUBLIC;
      domainParams = DomainParam_NIST_P521;
      command = 0;
      n = 80;
   }
   else if(!osStrcmp(params->name, "brainpoolP256r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P256R1_PUBLIC;
      domainParams = DomainParam_Brainpool_256r1;
      command = 0;
      n = 32;
   }
   else if(!osStrcmp(params->name, "brainpoolP384r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P384R1_PUBLIC;
      domainParams = DomainParam_Brainpool_384r1;
      command = 0;
      n = 48;
   }
   else if(!osStrcmp(params->name, "brainpoolP512r1"))
   {
      curveType = SCE_ECC_CURVE_TYPE_BRAINPOOL;
      oemCommand = SCE_OEM_CMD_ECC_P512R1_PUBLIC;
      domainParams = DomainParam_Brainpool_512r1;
      command = 0;
      n = 64;
   }
   else
   {
      return ERROR_FAILURE;
   }

   //Keep the leftmost bits of the hash value
   digestLen = MIN(digestLen, orderLen);

   //Acquire exclusive access to the RSIP7 module
   osAcquireMutex(&ra8CryptoMutex);

   //Pad the digest with leading zeroes if necessary
   osMemset(ecArgs.digest, 0, n);
   osMemcpy((uint8_t *) ecArgs.digest + n - digestLen, digest, digestLen);

   //Set public key
   mpiWriteRaw(&publicKey->q.x, (uint8_t *) ecArgs.q, n);
   mpiWriteRaw(&publicKey->q.y, (uint8_t *) ecArgs.q + n, n);

   //Set signature
   mpiWriteRaw(&signature->r, (uint8_t *) ecArgs.signature, n);
   mpiWriteRaw(&signature->s, (uint8_t *) ecArgs.signature + n, n);

   //Install the plaintext public key and get the wrapped key
   status = HW_SCE_GenerateOemKeyIndexPrivate(SCE_OEM_KEY_TYPE_PLAIN,
      oemCommand, NULL, NULL, (uint8_t *) ecArgs.q, ecArgs.wrappedKey);

   //Check status code
   if(status == FSP_SUCCESS)
   {
      //Verify ECDSA signature
      if(n == 32)
      {
         status = HW_SCE_EcdsaSignatureVerificationSub(&curveType, &command,
            ecArgs.wrappedKey, ecArgs.digest, ecArgs.signature, domainParams);
      }
      else if(n == 48)
      {
         status = HW_SCE_EcdsaP384SignatureVerificationSub(&curveType,
            ecArgs.wrappedKey, ecArgs.digest, ecArgs.signature, domainParams);
      }
      else if(n == 64)
      {
         status = HW_SCE_EcdsaP512SignatureVerificationSub(ecArgs.wrappedKey,
            ecArgs.digest, ecArgs.signature, domainParams);
      }
      else if(n == 80)
      {
         status = HW_SCE_EcdsaP521SignatureVerificationSub(ecArgs.wrappedKey,
            ecArgs.digest, ecArgs.signature, domainParams);
      }
      else
      {
         status = FSP_ERR_CRYPTO_NOT_IMPLEMENTED;
      }
   }

   //Release exclusive access to the RSIP7 module
   osReleaseMutex(&ra8CryptoMutex);

   //Return status code
   return (status == FSP_SUCCESS) ? NO_ERROR : ERROR_INVALID_SIGNATURE;
}

#endif
