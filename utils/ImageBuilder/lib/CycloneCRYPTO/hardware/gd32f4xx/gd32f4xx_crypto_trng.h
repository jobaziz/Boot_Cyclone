/**
 * @file gd32f4xx_crypto_trng.h
 * @brief GD32F4 true random number generator
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

#ifndef _GD32F4XX_CRYPTO_TRNG_H
#define _GD32F4XX_CRYPTO_TRNG_H

//Dependencies
#include "core/crypto.h"

//True random number generator
#ifndef GD32F4XX_CRYPTO_TRNG_SUPPORT
   #define GD32F4XX_CRYPTO_TRNG_SUPPORT ENABLED
#elif (GD32F4XX_CRYPTO_TRNG_SUPPORT != ENABLED && GD32F4XX_CRYPTO_TRNG_SUPPORT != DISABLED)
   #error GD32F4XX_CRYPTO_TRNG_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TRNG related functions
error_t trngInit(void);
error_t trngGetRandomData(uint8_t *data, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
