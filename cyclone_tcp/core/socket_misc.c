/**
 * @file socket_misc.c
 * @brief Helper functions for sockets
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

//Switch to the appropriate trace level
#define TRACE_LEVEL SOCKET_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "core/socket.h"
#include "core/socket_misc.h"
#include "core/raw_socket.h"
#include "core/udp.h"
#include "core/tcp.h"
#include "core/tcp_misc.h"
#include "debug.h"


/**
 * @brief Allocate a socket
 * @param[in] type Type specification for the new socket
 * @param[in] protocol Protocol to be used
 * @return Handle referencing the new socket
 **/

Socket *socketAllocate(uint_t type, uint_t protocol)
{
   error_t error;
   uint_t i;
   uint16_t port;
   Socket *socket;

   //Initialize socket handle
   socket = NULL;

#if (TCP_SUPPORT == ENABLED)
   //Connection-oriented socket?
   if(type == SOCKET_TYPE_STREAM)
   {
      //Always use TCP as underlying transport protocol
      protocol = SOCKET_IP_PROTO_TCP;
      //Get an ephemeral port number
      port = tcpGetDynamicPort();
      //Continue processing
      error = NO_ERROR;
   }
   else
#endif
#if (UDP_SUPPORT == ENABLED)
   //Connectionless socket?
   if(type == SOCKET_TYPE_DGRAM)
   {
      //Always use UDP as underlying transport protocol
      protocol = SOCKET_IP_PROTO_UDP;
      //Get an ephemeral port number
      port = udpGetDynamicPort();
      //Continue processing
      error = NO_ERROR;
   }
   else
#endif
#if (RAW_SOCKET_SUPPORT == ENABLED)
   //Raw socket?
   if(type == SOCKET_TYPE_RAW_IP || type == SOCKET_TYPE_RAW_ETH)
   {
      //Port numbers are not relevant for raw sockets
      port = 0;
      //Continue processing
      error = NO_ERROR;
   }
   else
#endif
   {
      //The socket type is not supported
      error = ERROR_INVALID_PARAMETER;
   }

   //Check status code
   if(!error)
   {
      //Loop through socket descriptors
      for(i = 0; i < SOCKET_MAX_COUNT; i++)
      {
         //Unused socket found?
         if(socketTable[i].type == SOCKET_TYPE_UNUSED)
         {
            //Save socket handle
            socket = &socketTable[i];
            //We are done
            break;
         }
      }

#if (TCP_SUPPORT == ENABLED)
      //No more sockets available?
      if(socket == NULL)
      {
         //Kill the oldest connection in the TIME-WAIT state whenever the
         //socket table runs out of space
         socket = tcpKillOldestConnection();
      }
#endif

      //Check whether the current entry is free
      if(socket != NULL)
      {
         //Save socket descriptor
         i = socket->descriptor;

         //Clear the structure keeping the event field untouched
         osMemset(socket, 0, offsetof(Socket, event));

         osMemset((uint8_t *) socket + offsetof(Socket, event) + sizeof(OsEvent),
            0, sizeof(Socket) - offsetof(Socket, event) - sizeof(OsEvent));

         //Save socket characteristics
         socket->descriptor = i;
         socket->type = type;
         socket->protocol = protocol;
         socket->localPort = port;
         socket->timeout = INFINITE_DELAY;

#if (ETH_VLAN_SUPPORT == ENABLED)
         //Default VLAN PCP and DEI fields
         socket->vlanPcp = -1;
         socket->vlanDei = -1;
#endif

#if (ETH_VMAN_SUPPORT == ENABLED)
         //Default VMAN PCP and DEI fields
         socket->vmanPcp = -1;
         socket->vmanDei = -1;
#endif

#if (TCP_SUPPORT == ENABLED && TCP_KEEP_ALIVE_SUPPORT == ENABLED)
         //TCP keep-alive mechanism must be disabled by default (refer to
         //RFC 1122, section 4.2.3.6)
         socket->keepAliveEnabled = FALSE;

         //Default TCP keep-alive parameters
         socket->keepAliveIdle = TCP_DEFAULT_KEEP_ALIVE_IDLE;
         socket->keepAliveInterval = TCP_DEFAULT_KEEP_ALIVE_INTERVAL;
         socket->keepAliveMaxProbes = TCP_DEFAULT_KEEP_ALIVE_PROBES;
#endif

#if (TCP_SUPPORT == ENABLED)
         //Default MSS value
         socket->mss = TCP_MAX_MSS;

         //Default TX and RX buffer size
         socket->txBufferSize = MIN(TCP_DEFAULT_TX_BUFFER_SIZE, TCP_MAX_TX_BUFFER_SIZE);
         socket->rxBufferSize = MIN(TCP_DEFAULT_RX_BUFFER_SIZE, TCP_MAX_RX_BUFFER_SIZE);
#endif
      }
   }

   //Return a handle to the freshly created socket
   return socket;
}


/**
 * @brief Subscribe to the specified socket events
 * @param[in] socket Handle that identifies a socket
 * @param[in] event Event object used to receive notifications
 * @param[in] eventMask Logic OR of the requested socket events
 **/

void socketRegisterEvents(Socket *socket, OsEvent *event, uint_t eventMask)
{
   //Valid socket handle?
   if(socket != NULL)
   {
      //Get exclusive access
      osAcquireMutex(&netMutex);

      //An user event may have been previously registered...
      if(socket->userEvent != NULL)
      {
         socket->eventMask |= eventMask;
      }
      else
      {
         socket->eventMask = eventMask;
      }

      //Suscribe to get notified of events
      socket->userEvent = event;

#if (TCP_SUPPORT == ENABLED)
      //Handle TCP specific events
      if(socket->type == SOCKET_TYPE_STREAM)
      {
         tcpUpdateEvents(socket);
      }
#endif
#if (UDP_SUPPORT == ENABLED)
      //Handle UDP specific events
      if(socket->type == SOCKET_TYPE_DGRAM)
      {
         udpUpdateEvents(socket);
      }
#endif
#if (RAW_SOCKET_SUPPORT == ENABLED)
      //Handle events that are specific to raw sockets
      if(socket->type == SOCKET_TYPE_RAW_IP ||
         socket->type == SOCKET_TYPE_RAW_ETH)
      {
         rawSocketUpdateEvents(socket);
      }
#endif

      //Release exclusive access
      osReleaseMutex(&netMutex);
   }
}


/**
 * @brief Unsubscribe previously registered events
 * @param[in] socket Handle that identifies a socket
 **/

void socketUnregisterEvents(Socket *socket)
{
   //Valid socket handle?
   if(socket != NULL)
   {
      //Get exclusive access
      osAcquireMutex(&netMutex);

      //Unsuscribe socket events
      socket->userEvent = NULL;

      //Release exclusive access
      osReleaseMutex(&netMutex);
   }
}


/**
 * @brief Retrieve event flags for a specified socket
 * @param[in] socket Handle that identifies a socket
 * @return Logic OR of events in the signaled state
 **/

uint_t socketGetEvents(Socket *socket)
{
   uint_t eventFlags;

   //Valid socket handle?
   if(socket != NULL)
   {
      //Get exclusive access
      osAcquireMutex(&netMutex);

      //Read event flags for the specified socket
      eventFlags = socket->eventFlags;

      //Release exclusive access
      osReleaseMutex(&netMutex);
   }
   else
   {
      //The socket handle is not valid
      eventFlags = 0;
   }

   //Return the events in the signaled state
   return eventFlags;
}
