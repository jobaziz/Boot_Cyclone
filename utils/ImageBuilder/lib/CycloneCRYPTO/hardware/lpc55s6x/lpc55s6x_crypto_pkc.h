/**
 * @file lpc55s6x_crypto_pkc.h
 * @brief LPC55S6x public-key hardware accelerator
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

#ifndef _LPC55S6X_CRYPTO_PKC_H
#define _LPC55S6X_CRYPTO_PKC_H

//Dependencies
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef LPC55S6X_CRYPTO_PKC_SUPPORT
   #define LPC55S6X_CRYPTO_PKC_SUPPORT DISABLED
#elif (LPC55S6X_CRYPTO_PKC_SUPPORT != ENABLED && LPC55S6X_CRYPTO_PKC_SUPPORT != DISABLED)
   #error LPC55S6X_CRYPTO_PKC_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief EC primitive arguments
 **/

typedef struct
{
   uint32_t rx[18];
   uint32_t ry[18];
   uint32_t sx[18];
   uint32_t sy[18];
   uint32_t d[18];
} Lpc55s6xEcArgs;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
