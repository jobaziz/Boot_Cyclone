/**
 * @file lldp_debug.h
 * @brief Data logging functions for debugging purpose (LLDP)
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
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

#ifndef _LLDP_DEBUG_H
#define _LLDP_DEBUG_H

//Dependencies
#include "core/net.h"
#include "lldp/lldp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Parameter value/name binding
 **/

typedef struct
{
   uint_t value;
   const char_t *name;
} LldpParamName;


//LLDP agent related functions
void lldpDumpDataUnit(LldpDataUnit *lldpdu);
void lldpDumpTlv(const LldpTlv *tlv);

const char_t *lldpGetParamName(uint_t value, const LldpParamName *paramList,
   size_t paramListLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
