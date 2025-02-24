/**
 * @file s7g2_crypto.c
 * @brief Synergy S7G2 hardware cryptographic accelerator (SCE7)
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
#include "core/crypto.h"
#include "hardware/s7g2/s7g2_crypto.h"
#include "debug.h"

//Global variables
OsMutex s7g2CryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t s7g2CryptoInit(void)
{
   ssp_err_t status;

   //Initialize status code
   status = SSP_SUCCESS;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&s7g2CryptoMutex))
   {
      //Failed to create mutex
      status = SSP_ERR_CRYPTO_NOT_OPEN;
   }

   //Check status code
   if(status == SSP_SUCCESS)
   {
      //Initialize SCE7 module
      status = HW_SCE_McuSpecificInit();
   }

   //Return status code
   return (status == SSP_SUCCESS) ? NO_ERROR : ERROR_FAILURE;
}
