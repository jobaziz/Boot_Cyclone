/**
 * \file
 *
 * \brief Structure definition for PUKCL 'ZpEccQuickDualMul' service
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED FOR DEMONSTRATION PURPOSES AS AN EXAMPLE.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". THERE IS NO
 * SECURITY BUILT INTO THIS SOFTWARE. NO WARRANTIES,WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#ifndef __PUKCL_ZPECC_QUICKDUALMULT_PB_INCLUDED__
#define __PUKCL_ZPECC_QUICKDUALMULT_PB_INCLUDED__

#define AbsVal(a) ((a > 0) ? a : (-a))

// Structure definition
typedef struct _PUKCL_ZpEccQuickDualMul {
	pu1 pu1ModCnsBase;
	pu1 pu1PointABase;
	pu1 pu1PointBBase;
	pu1 pu1KABBase;
	pu1 pu1AWorkBase;
	// pu1       pu1ABase;
	u2 u2ModLength;
	u2 u2KLength;
} _PUKCL_ZPECCQUICKDUALMUL, *_PPUKCL_ZPECCQUICKDUALMUL;

#define PUKCL_ZPECCMUL_SCAL_IN_CLASSIC_RAM (0x01 << 8)
#define PUKCL_ZPECCMUL_WINSIZE_MASK 0x0F
#define PUKCL_ZPECCMUL_WINSIZE_B_VAL_TO_OPT(W) (((W)&PUKCL_ZPECCMUL_WINSIZE_MASK) << 4)
#define PUKCL_ZPECCMUL_WINSIZE_A_VAL_TO_OPT(W) ((W)&PUKCL_ZPECCMUL_WINSIZE_MASK)
#define PUKCL_ZPECCMUL_WINSIZE_B_OPT_TO_VAL(Option) (((Option) >> 4) & PUKCL_ZPECCMUL_WINSIZE_MASK)
#define PUKCL_ZPECCMUL_WINSIZE_A_OPT_TO_VAL(Option) ((Option)&PUKCL_ZPECCMUL_WINSIZE_MASK)

#endif //__PUKCL_ZPECC_QUICKDUALMULT_PB_INCLUDED__
