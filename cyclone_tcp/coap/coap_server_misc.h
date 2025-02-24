/**
 * @file coap_server_misc.h
 * @brief Helper functions for CoAP server
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

#ifndef _COAP_SERVER_MISC_H
#define _COAP_SERVER_MISC_H

//Dependencies
#include "core/net.h"
#include "coap/coap_server.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CoAP server related functions
void coapServerTick(CoapServerContext *context);

error_t coapServerProcessRequest(CoapServerContext *context,
   const uint8_t *data, size_t length);

error_t coapServerRejectRequest(CoapServerContext *context);

error_t coapServerInitResponse(CoapServerContext *context);

error_t coapServerSendResponse(CoapServerContext *context,
   const void *data, size_t length);

error_t coapServerFormatReset(CoapServerContext *context, uint16_t mid);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
