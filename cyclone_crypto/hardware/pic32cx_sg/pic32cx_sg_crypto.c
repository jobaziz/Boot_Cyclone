/**
 * @file pic32cx_sg_crypto.c
 * @brief PIC32CX SG41/SG60/SG61 hardware cryptographic accelerator
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
#include "pic32c.h"
#include "core/crypto.h"
#include "hardware/pic32cx_sg/pic32cx_sg_crypto.h"
#include "hardware/pic32cx_sg/pic32cx_sg_crypto_trng.h"
#include "hardware/pic32cx_sg/pic32cx_sg_crypto_hash.h"
#include "hardware/pic32cx_sg/pic32cx_sg_crypto_cipher.h"
#include "hardware/pic32cx_sg/pic32cx_sg_crypto_pkc.h"
#include "debug.h"

//Global variables
OsMutex pic32cxsgCryptoMutex;


/**
 * @brief Initialize hardware cryptographic accelerator
 * @return Error code
 **/

error_t pic32cxsgCryptoInit(void)
{
   error_t error;

   //Initialize status code
   error = NO_ERROR;

   //Create a mutex to prevent simultaneous access to the hardware
   //cryptographic accelerator
   if(!osCreateMutex(&pic32cxsgCryptoMutex))
   {
      //Failed to create mutex
      error = ERROR_OUT_OF_RESOURCES;
   }

#if (PIC32CX_SG_CRYPTO_TRNG_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize TRNG module
      error = trngInit();
   }
#endif

#if (PIC32CX_SG_CRYPTO_HASH_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable ICM bus clocks (CLK_ICM_APB and CLK_ICM_AHB)
      MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_ICM_Msk;
      MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_ICM_Msk;
   }
#endif

#if (PIC32CX_SG_CRYPTO_CIPHER_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Enable AES bus clock (CLK_AES_APB)
      MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_AES_Msk;
   }
#endif

#if (PIC32CX_SG_CRYPTO_PKC_SUPPORT == ENABLED)
   //Check status code
   if(!error)
   {
      //Initialize public key accelerator
      error = pukccInit();
   }
#endif

   //Return status code
   return error;
}
