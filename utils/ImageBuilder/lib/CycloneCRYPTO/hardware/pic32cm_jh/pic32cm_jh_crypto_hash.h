/**
 * @file pic32cm_jh_crypto_hash.h
 * @brief PIC32CM JH00/JH01 hash hardware accelerator
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

#ifndef _PIC32CM_JH_CRYPTO_HASH_H
#define _PIC32CM_JH_CRYPTO_HASH_H

//Dependencies
#include "core/crypto.h"

//Hash hardware accelerator
#ifndef PIC32CM_JH_CRYPTO_HASH_SUPPORT
   #define PIC32CM_JH_CRYPTO_HASH_SUPPORT DISABLED
#elif (PIC32CM_JH_CRYPTO_HASH_SUPPORT != ENABLED && PIC32CM_JH_CRYPTO_HASH_SUPPORT != DISABLED)
   #error PIC32CM_JH_CRYPTO_HASH_SUPPORT parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief ICM region descriptor
 **/

typedef struct
{
   uint32_t raddr; ///<ICM region start address
   uint32_t rcfg;  ///<ICM region configuration
   uint32_t rctrl; ///<ICM region control
   uint32_t rnext; ///<ICM region next address
} Pic32cmjhIcmDesc;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
