/**
 * @file pic32cx_mt_crypto_trng.c
 * @brief PIC32CX MTC/MTG/MTSH true random number generator
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
#include "hardware/pic32cx_mt/pic32cx_mt_crypto.h"
#include "hardware/pic32cx_mt/pic32cx_mt_crypto_trng.h"
#include "debug.h"

//Check crypto library configuration
#if (PIC32CX_MT_CRYPTO_TRNG_SUPPORT == ENABLED)


/**
 * @brief TRNG module initialization
 * @return Error code
 **/

error_t trngInit(void)
{
   uint32_t temp;

   //Enable TRNG peripheral clock
   PMC_REGS->PMC_PCR = PMC_PCR_PID(ID_TRNG);
   temp = PMC_REGS->PMC_PCR;
   PMC_REGS->PMC_PCR = temp | PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk;

   //Enable TRNG
   TRNG_REGS->TRNG_CR = TRNG_CR_WAKEY_PASSWD | TRNG_CR_ENABLE_Msk;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Get random data from the TRNG module
 * @param[out] data Buffer where to store random data
 * @param[in] length Number of random bytes to generate
 **/

error_t trngGetRandomData(uint8_t *data, size_t length)
{
   size_t i;
   uint32_t value;

   //Initialize variable
   value = 0;

   //Acquire exclusive access to the TRNG module
   osAcquireMutex(&pic32cxmtCryptoMutex);

   //Generate random data
   for(i = 0; i < length; i++)
   {
      //Generate a new 32-bit random value when necessary
      if((i % 4) == 0)
      {
         //Wait for the TRNG to contain a valid data
         while((TRNG_REGS->TRNG_ISR & TRNG_ISR_DATRDY_Msk) == 0)
         {
         }

         //Get the 32-bit random value
         value = TRNG_REGS->TRNG_ODATA;
      }

      //Copy random byte
      data[i] = value & 0xFF;
      //Shift the 32-bit random value
      value >>= 8;
   }

   //Release exclusive access to the TRNG module
   osReleaseMutex(&pic32cxmtCryptoMutex);

   //Successful processing
   return NO_ERROR;
}

#endif
