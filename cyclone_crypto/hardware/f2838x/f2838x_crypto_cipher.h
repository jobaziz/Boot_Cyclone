/**
 * @file f2838x_crypto_cipher.h
 * @brief TMS320F2838xD cipher hardware accelerator
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

#ifndef _F2838X_CRYPTO_CIPHER_H
#define _F2838X_CRYPTO_CIPHER_H

//Dependencies
#include "core/crypto.h"

//Cipher hardware accelerator
#ifndef F2838X_CRYPTO_CIPHER_SUPPORT
   #define F2838X_CRYPTO_CIPHER_SUPPORT DISABLED
#elif (F2838X_CRYPTO_CIPHER_SUPPORT != ENABLED && F2838X_CRYPTO_CIPHER_SUPPORT != DISABLED)
   #error F2838X_CRYPTO_CIPHER_SUPPORT parameter is not valid
#endif

//AES engine registers
#define AES_KEY2_6_R        HWREG(AES_BASE + AES_O_KEY2_6)
#define AES_KEY2_7_R        HWREG(AES_BASE + AES_O_KEY2_7)
#define AES_KEY2_4_R        HWREG(AES_BASE + AES_O_KEY2_4)
#define AES_KEY2_5_R        HWREG(AES_BASE + AES_O_KEY2_5)
#define AES_KEY2_2_R        HWREG(AES_BASE + AES_O_KEY2_2)
#define AES_KEY2_3_R        HWREG(AES_BASE + AES_O_KEY2_3)
#define AES_KEY2_0_R        HWREG(AES_BASE + AES_O_KEY2_0)
#define AES_KEY2_1_R        HWREG(AES_BASE + AES_O_KEY2_1)
#define AES_KEY1_6_R        HWREG(AES_BASE + AES_O_KEY1_6)
#define AES_KEY1_7_R        HWREG(AES_BASE + AES_O_KEY1_7)
#define AES_KEY1_4_R        HWREG(AES_BASE + AES_O_KEY1_4)
#define AES_KEY1_5_R        HWREG(AES_BASE + AES_O_KEY1_5)
#define AES_KEY1_2_R        HWREG(AES_BASE + AES_O_KEY1_2)
#define AES_KEY1_3_R        HWREG(AES_BASE + AES_O_KEY1_3)
#define AES_KEY1_0_R        HWREG(AES_BASE + AES_O_KEY1_0)
#define AES_KEY1_1_R        HWREG(AES_BASE + AES_O_KEY1_1)
#define AES_IV_IN_OUT_0_R   HWREG(AES_BASE + AES_O_IV_IN_OUT_0)
#define AES_IV_IN_OUT_1_R   HWREG(AES_BASE + AES_O_IV_IN_OUT_1)
#define AES_IV_IN_OUT_2_R   HWREG(AES_BASE + AES_O_IV_IN_OUT_2)
#define AES_IV_IN_OUT_3_R   HWREG(AES_BASE + AES_O_IV_IN_OUT_3)
#define AES_CTRL_R          HWREG(AES_BASE + AES_O_CTRL)
#define AES_C_LENGTH_0_R    HWREG(AES_BASE + AES_O_C_LENGTH_0)
#define AES_C_LENGTH_1_R    HWREG(AES_BASE + AES_O_C_LENGTH_1)
#define AES_AUTH_LENGTH_R   HWREG(AES_BASE + AES_O_AUTH_LENGTH)
#define AES_DATA_IN_OUT_0_R HWREG(AES_BASE + AES_O_DATA_IN_OUT_0)
#define AES_DATA_IN_OUT_1_R HWREG(AES_BASE + AES_O_DATA_IN_OUT_1)
#define AES_DATA_IN_OUT_2_R HWREG(AES_BASE + AES_O_DATA_IN_OUT_2)
#define AES_DATA_IN_OUT_3_R HWREG(AES_BASE + AES_O_DATA_IN_OUT_3)
#define AES_TAG_OUT_0_R     HWREG(AES_BASE + AES_O_TAG_OUT_0)
#define AES_TAG_OUT_1_R     HWREG(AES_BASE + AES_O_TAG_OUT_1)
#define AES_TAG_OUT_2_R     HWREG(AES_BASE + AES_O_TAG_OUT_2)
#define AES_TAG_OUT_3_R     HWREG(AES_BASE + AES_O_TAG_OUT_3)
#define AES_REVISION_R      HWREG(AES_BASE + AES_O_REVISION)
#define AES_SYSCONFIG_R     HWREG(AES_BASE + AES_O_SYSCONFIG)
#define AES_SYSSTATUS_R     HWREG(AES_BASE + AES_O_SYSSTATUS)
#define AES_IRQSTATUS_R     HWREG(AES_BASE + AES_O_IRQSTATUS)
#define AES_IRQENABLE_R     HWREG(AES_BASE + AES_O_IRQENABLE)
#define AES_DIRTYBITS_R     HWREG(AES_BASE + AES_O_DIRTYBITS)
#define AES_DMAIM_R         HWREG(AES_BASE + AES_O_DMAIM)
#define AES_DMARIS_R        HWREG(AES_BASE + AES_O_DMARIS)
#define AES_DMAMIS_R        HWREG(AES_BASE + AES_O_DMAMIS)
#define AES_DMAIC_R         HWREG(AES_BASE + AES_O_DMAIC)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
