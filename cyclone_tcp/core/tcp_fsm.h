/**
 * @file tcp_fsm.h
 * @brief TCP finite state machine
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

#ifndef _TCP_FSM_H
#define _TCP_FSM_H

//Dependencies
#include "core/tcp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//TCP FSM related functions
void tcpProcessSegment(NetInterface *interface,
   const IpPseudoHeader *pseudoHeader, const NetBuffer *buffer, size_t offset,
   const NetRxAncillary *ancillary);

void tcpStateClosed(NetInterface *interface, const IpPseudoHeader *pseudoHeader,
   const TcpHeader *segment, size_t length);

void tcpStateListen(Socket *socket, NetInterface *interface,
   const IpPseudoHeader *pseudoHeader, const TcpHeader *segment, size_t length);

void tcpStateSynSent(Socket *socket, const TcpHeader *segment, size_t length);

void tcpStateSynReceived(Socket *socket, const TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateEstablished(Socket *socket, const TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateCloseWait(Socket *socket, const TcpHeader *segment, size_t length);

void tcpStateLastAck(Socket *socket, const TcpHeader *segment, size_t length);

void tcpStateFinWait1(Socket *socket, const TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateFinWait2(Socket *socket, const TcpHeader *segment,
   const NetBuffer *buffer, size_t offset, size_t length);

void tcpStateClosing(Socket *socket, const TcpHeader *segment, size_t length);

void tcpStateTimeWait(Socket *socket, const TcpHeader *segment, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
