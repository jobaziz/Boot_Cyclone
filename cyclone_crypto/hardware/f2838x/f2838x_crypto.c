/**
 * @file f2838x_crypto.c
 * @brief TMS320F2838xD hardware cryptographic accelerator
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
#include <stdint.h>
#include <stdbool.h>
#include "driverlib_cm/sysctl.h"
#include "core/crypto.h"
#include "hardware/f2838x/f2838x_crypto.h"
#include "debug.h"

//Global variables
OsMutex f2838xCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t f2838xCryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Enable AES peripheral clock
   SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_AESIP);
   //Reset AES peripheral
   SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_AESIP);

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&f2838xCryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

   //Return status code
   return error;
}
