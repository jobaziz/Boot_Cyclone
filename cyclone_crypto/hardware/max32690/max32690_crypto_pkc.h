/**
 * @file max32690_crypto_pkc.h
 * @brief MAX32690 public-key hardware accelerator
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

#ifndef _MAX32690_CRYPTO_PKC_H
#define _MAX32690_CRYPTO_PKC_H

//Dependencies
#include "core/crypto.h"

//Public-key hardware accelerator
#ifndef MAX32690_CRYPTO_PKC_SUPPORT
   #define MAX32690_CRYPTO_PKC_SUPPORT DISABLED
#elif (MAX32690_CRYPTO_PKC_SUPPORT != ENABLED && MAX32690_CRYPTO_PKC_SUPPORT != DISABLED)
   #error MAX32690_CRYPTO_PKC_SUPPORT parameter is not valid
#endif

//MAA memory instances
#define CTB_MAA_MEM_INSTANCE_0 0x0100
#define CTB_MAA_MEM_INSTANCE_1 0x0200
#define CTB_MAA_MEM_INSTANCE_2 0x0300
#define CTB_MAA_MEM_INSTANCE_3 0x0400
#define CTB_MAA_MEM_INSTANCE_4 0x0500
#define CTB_MAA_MEM_INSTANCE_5 0x0600

//Cryptographic Control register
#define CTB_CTRL_MAA_DONE_Pos             28
#define TB_CTRL_MAA_DONE_Msk              0x10000000
#define TB_CTRL_MAA_DONE(n)               (((n) << CTB_CTRL_MAA_DONE_Pos) & TB_CTRL_MAA_DONE_Msk)

//MAA Control register
#define CTB_MAA_CTRL_TMA_Pos              28
#define CTB_MAA_CTRL_TMA_Msk              0xF0000000
#define CTB_MAA_CTRL_TMA(n)               (((n) << CTB_MAA_CTRL_TMA_Pos) & CTB_MAA_CTRL_TMA_Msk)

#define CTB_MAA_CTRL_RMA_Pos              24
#define CTB_MAA_CTRL_RMA_Msk              0x0F000000
#define CTB_MAA_CTRL_RMA(n)               (((n) << CTB_MAA_CTRL_RMA_Pos) & CTB_MAA_CTRL_RMA_Msk)

#define CTB_MAA_CTRL_BMA_Pos              20
#define CTB_MAA_CTRL_BMA_Msk              0x00F00000
#define CTB_MAA_CTRL_BMA(n)               (((n) << CTB_MAA_CTRL_BMA_Pos) & CTB_MAA_CTRL_BMA_Msk)

#define CTB_MAA_CTRL_AMA_Pos              16
#define CTB_MAA_CTRL_AMA_Msk              0x000F0000
#define CTB_MAA_CTRL_AMA(n)               (((n) << CTB_MAA_CTRL_AMA_Pos) & CTB_MAA_CTRL_AMA_Msk)

#define CTB_MAA_CTRL_MMS_Pos              14
#define CTB_MAA_CTRL_MMS_Msk              0x0000C000
#define CTB_MAA_CTRL_MMS(n)               (((n) << CTB_MAA_CTRL_MMS_Pos) & CTB_MAA_CTRL_MMS_Msk)

#define CTB_MAA_CTRL_EMS_Pos              12
#define CTB_MAA_CTRL_EMS_Msk              0x00003000
#define CTB_MAA_CTRL_EMS(n)               (((n) << CTB_MAA_CTRL_EMS_Pos) & CTB_MAA_CTRL_EMS_Msk)

#define CTB_MAA_CTRL_BMS_Pos              10
#define CTB_MAA_CTRL_BMS_Msk              0x00000C00
#define CTB_MAA_CTRL_BMS(n)               (((n) << CTB_MAA_CTRL_BMS_Pos) & CTB_MAA_CTRL_BMS_Msk)

#define CTB_MAA_CTRL_AMS_Pos              8
#define CTB_MAA_CTRL_AMS_Msk              0x00000300
#define CTB_MAA_CTRL_AMS(n)               (((n) << CTB_MAA_CTRL_AMS_Pos) & CTB_MAA_CTRL_AMS_Msk)

#define CTB_MAA_CTRL_OPT_Pos              4
#define CTB_MAA_CTRL_OPT_Msk              0x00000010
#define CTB_MAA_CTRL_OPT(n)               (((n) << CTB_MAA_CTRL_OPT_Pos) & CTB_MAA_CTRL_OPT_Msk)

#define CTB_MAA_CTRL_CALC_Pos             1
#define CTB_MAA_CTRL_CALC_Msk             0x0000000E
#define CTB_MAA_CTRL_CALC_MOD_EXP_Val     0
#define CTB_MAA_CTRL_CALC_MOD_SQR_Val     1
#define CTB_MAA_CTRL_CALC_MOD_MUL_Val     2
#define CTB_MAA_CTRL_CALC_MOD_SQR_MUL_Val 3
#define CTB_MAA_CTRL_CALC_MOD_ADD_Val     4
#define CTB_MAA_CTRL_CALC_MOD_SUB_Val     5
#define CTB_MAA_CTRL_CALC(n)              (((n) << CTB_MAA_CTRL_CALC_Pos) & CTB_MAA_CTRL_CALC_Msk)

#define CTB_MAA_CTRL_START_Pos            0
#define CTB_MAA_CTRL_START_Msk            0x00000001
#define CTB_MAA_CTRL_START(n)             (((n) << CTB_MAA_CTRL_START_Pos) & CTB_MAA_CTRL_START_Msk)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
