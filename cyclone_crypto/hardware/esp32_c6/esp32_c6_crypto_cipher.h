/**
 * @file esp32_c6_crypto_cipher.h
 * @brief ESP32-C6 cipher hardware accelerator
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

#ifndef _ESP32_C6_CRYPTO_CIPHER_H
#define _ESP32_C6_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef ESP32_C6_CRYPTO_CIPHER_SUPPORT
   #define ESP32_C6_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (ESP32_C6_CRYPTO_CIPHER_SUPPORT != ENABLED && ESP32_C6_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error ESP32_C6_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//AES Operation Mode register
#define AES_MODE_ENC          0x00000000
#define AES_MODE_DEC          0x00000004
#define AES_MODE_128_BITS     0x00000000
#define AES_MODE_256_BITS     0x00000002

//Endianness Configuration register
#define AES_ENDIAN_DEFAULT    0x0000003F

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Cipher related functions
void esp32c6AesInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
