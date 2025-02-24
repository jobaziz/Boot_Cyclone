/**
 * @file mimxrt1160_crypto.c
 * @brief i.MX RT1160 hardware cryptographic accelerator (CAAM)
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
#include "fsl_device_registers.h"
#include "fsl_caam.h"
#include "core/crypto.h"
#include "hardware/mimxrt1160/mimxrt1160_crypto.h"
#include "debug.h"

//Global variables
OsMutex mimxrt1160CryptoMutex;

//CAAM job ring interfaces
static caam_job_ring_interface_t caamJobRingInterface[4];


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t mimxrt1160CryptoInit(void)
{
   status_t status;
   caam_config_t caamConfig;

   //Initialize status code
   status = kStatus_Success;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&mimxrt1160CryptoMutex))
   {
      //Failed to create mutex
      status = kStatus_Fail;
   }

   //Check status code
   if(status == kStatus_Success)
   {
      //Get default configuration
      CAAM_GetDefaultConfig(&caamConfig);

      //Set job ring interfaces
      caamConfig.jobRingInterface[0] = &caamJobRingInterface[0];
      caamConfig.jobRingInterface[1] = &caamJobRingInterface[1];
      caamConfig.jobRingInterface[2] = &caamJobRingInterface[2];
      caamConfig.jobRingInterface[3] = &caamJobRingInterface[3];

      //Initialize CAAM module
      status = CAAM_Init(CAAM, &caamConfig);
   }

   //Return status code
   return (status == kStatus_Success) ? NO_ERROR : ERROR_FAILURE;
}
