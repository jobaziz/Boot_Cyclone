/**
 * @file mimxrt1160_crypto_trng.h
 * @brief i.MX RT1160 true random number generator
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

#ifndef _MIMXRT1160_CRYPTO_TRNG_H
#define _MIMXRT1160_CRYPTO_TRNG_H

//Dependencies
#include "core/crypto.h"

//True random number generator
#ifndef MIMXRT1160_CRYPTO_TRNG_SUPPORT
   #define MIMXRT1160_CRYPTO_TRNG_SUPPORT ENABLED
#elif (MIMXRT1160_CRYPTO_TRNG_SUPPORT != ENABLED && MIMXRT1160_CRYPTO_TRNG_SUPPORT != DISABLED)
   #error MIMXRT1160_CRYPTO_TRNG_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TRNG related functions
error_t trngGetRandomData(uint8_t *data, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
