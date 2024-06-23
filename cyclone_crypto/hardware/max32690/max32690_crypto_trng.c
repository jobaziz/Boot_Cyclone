/**
 * @file max32690_crypto_trng.c
 * @brief MAX32690 true random number generator
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
#include "mxc_device.h"
#include "trng.h"
#include "core/crypto.h"
#include "hardware/max32690/max32690_crypto.h"
#include "hardware/max32690/max32690_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (MAX32690_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief TRNG module initialization
 * @return Error code
 **/

error_t trngInit(void)
{
   int_t status;

   //Initialize TRNG module
   status = MXC_TRNG_Init();

   //Return status code
   return (status == E_NO_ERROR) ? NO_ERROR : ERROR_FAILURE;
}


/**
 * @brief Get random data from the TRNG module
 * @param[out] data Buffer where to store random data
 * @param[in] length Number of random bytes to generate
 **/

error_t trngGetRandomData(uint8_t *data, size_t length)
{
   int_t status;

   //Generate random data
   status = MXC_TRNG_Random(data, length);

   //Return status code
   return (status == E_NO_ERROR) ? NO_ERROR : ERROR_FAILURE;
}

#endif
