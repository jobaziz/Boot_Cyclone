/**
 * @file m460_crypto_cipher.h
 * @brief M460 cipher hardware accelerator
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

#ifndef _M460_CRYPTO_CIPHER_H
#define _M460_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef M460_CRYPTO_CIPHER_SUPPORT
   #define M460_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (M460_CRYPTO_CIPHER_SUPPORT != ENABLED && M460_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error M460_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//OPMODE bitfield
#define CRPT_AES_CTL_OPMODE_ECB     0x00000000
#define CRPT_AES_CTL_OPMODE_CBC     0x00000100
#define CRPT_AES_CTL_OPMODE_CFB     0x00000200
#define CRPT_AES_CTL_OPMODE_OFB     0x00000300
#define CRPT_AES_CTL_OPMODE_CTR     0x00000400
#define CRPT_AES_CTL_OPMODE_CBC_CS1 0x00001000
#define CRPT_AES_CTL_OPMODE_CBC_CS2 0x00001100
#define CRPT_AES_CTL_OPMODE_CBC_CS3 0x00001200
#define CRPT_AES_CTL_OPMODE_GCM     0x00002000
#define CRPT_AES_CTL_OPMODE_GHASH   0x00002100
#define CRPT_AES_CTL_OPMODE_CCM     0x00002200

//KEYSZ bitfield
#define CRPT_AES_CTL_KEYSZ_128B     0x00000000
#define CRPT_AES_CTL_KEYSZ_192B     0x00000004
#define CRPT_AES_CTL_KEYSZ_256B     0x00000008

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
