/**
 * @file stm32wbaxx_crypto_pkc.h
 * @brief STM32WBA public-key hardware accelerator (PKA)
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

#ifndef _STM32WBAXX_CRYPTO_PKC_H
#define _STM32WBAXX_CRYPTO_PKC_H

//Dependencies
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef STM32WBAXX_CRYPTO_PKC_SUPPORT
   #define STM32WBAXX_CRYPTO_PKC_SUPPORT DISABLED
#elif (STM32WBAXX_CRYPTO_PKC_SUPPORT != ENABLED && STM32WBAXX_CRYPTO_PKC_SUPPORT != DISABLED)
   #error STM32WBAXX_CRYPTO_PKC_SUPPORT parameter is not valid
#endif

//Maximum RSA operand size, in bits
#define PKA_MAX_ROS 4160
//Maximum ECC operand size, in bits
#define PKA_MAX_EOS 640

//PKA operation modes
#define PKA_CR_MODE_MODULAR_EXP           0x00
#define PKA_CR_MODE_MONTGOMERY_PARAM      0x01
#define PKA_CR_MODE_MODULAR_EXP_FAST      0x02
#define PKA_CR_MODE_MODULAR_EXP_PROTECTED 0x03
#define PKA_CR_MODE_RSA_CRT_EXP           0x07
#define PKA_CR_MODE_MODULAR_INV           0x08
#define PKA_CR_MODE_ARITHMETIC_ADD        0x09
#define PKA_CR_MODE_ARITHMETIC_SUB        0x0A
#define PKA_CR_MODE_ARITHMETIC_MUL        0x0B
#define PKA_CR_MODE_COMPARISON            0x0C
#define PKA_CR_MODE_MODULAR_RED           0x0D
#define PKA_CR_MODE_MODULAR_ADD           0x0E
#define PKA_CR_MODE_MODULAR_SUB           0x0F
#define PKA_CR_MODE_MONTGOMERY_MUL        0x10
#define PKA_CR_MODE_ECC_MUL               0x20
#define PKA_CR_MODE_ECC_COMPLETE_ADD      0x23
#define PKA_CR_MODE_ECDSA_SIGN            0x24
#define PKA_CR_MODE_ECDSA_VERIFY          0x26
#define PKA_CR_MODE_DOUBLE_BASE_LADDER    0x27
#define PKA_CR_MODE_POINT_CHECK           0x28
#define PKA_CR_MODE_ECC_PROJECTIVE_AFF    0x2F

//PKA status codes
#define PKA_STATUS_SUCCESS 0xD60D
#define PKA_STATUS_INVALID 0x0000

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//PKA related functions
error_t pkaInit(void);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
