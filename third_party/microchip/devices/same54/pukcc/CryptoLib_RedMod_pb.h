/**
 * \file
 *
 * \brief Structure definition for PUKCL 'RedMod' service
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

#ifndef _CRYPTOLIB_REDMOD_PB_INCLUDED
#define _CRYPTOLIB_REDMOD_PB_INCLUDED

// Structure definition
typedef struct _PUKCL_redmod {
	nu1 nu1ModBase;
	nu1 nu1CnsBase;
	u2  u2ModLength;

	nu1 nu1RBase;
	u2  __Padding0;
	u2  __Padding1;
	nu1 nu1XBase;
} _PUKCL_REDMOD, *_PPKCL_REDMOD;

#define PUKCL_REDMOD_SETUP 0x0100
#define PUKCL_REDMOD_REDUCTION 0x0200
#define PUKCL_REDMOD_NORMALIZE 0x0400
#define PUKCL_REDMOD_OPERATIONMASK 0x0F00

#define PUKCL_REDMOD_USING_DIVISION 0x1000
#define PUKCL_REDMOD_USING_FASTRED 0x2000
#define PUKCL_REDMOD_RANDOMIZE 0x4000
#define PUKCL_REDMOD_MODEMASK 0xF000

#endif // _CRYPTOLIB_REDMOD_PB_INCLUDED
