/**
 * @file gd32f2xx_crypto_cipher.h
 * @brief GD32F2 cipher hardware accelerator
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

#ifndef _GD32F2XX_CRYPTO_CIPHER_H
#define _GD32F2XX_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef GD32F2XX_CRYPTO_CIPHER_SUPPORT
   #define GD32F2XX_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (GD32F2XX_CRYPTO_CIPHER_SUPPORT != ENABLED && GD32F2XX_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error GD32F2XX_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Cipher related functions
error_t cauInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
