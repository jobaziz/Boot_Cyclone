/**
 * @file dns_debug.h
 * @brief Data logging functions for debugging purpose (DNS)
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

#ifndef _DNS_DEBUG_H
#define _DNS_DEBUG_H

//Dependencies
#include "core/net.h"
#include "dns/dns_common.h"
#include "debug.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Check current trace level
#if (DNS_TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
   void dnsDumpMessage(const DnsHeader *message, size_t length);
   size_t dnsDumpQuestion(const DnsHeader *message, size_t length, size_t pos, char_t *buffer);
   size_t dnsDumpResourceRecord(const DnsHeader *message, size_t length, size_t pos, char_t *buffer);
#else
   #define dnsDumpMessage(message, length)
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
