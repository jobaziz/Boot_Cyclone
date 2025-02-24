/**
 * @file dhcp_client.c
 * @brief DHCP client (Dynamic Host Configuration Protocol)
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
#define TRACE_LEVEL DHCP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "dhcp/dhcp_client.h"
#include "dhcp/dhcp_client_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (IPV4_SUPPORT == ENABLED && DHCP_CLIENT_SUPPORT == ENABLED)


/**
 * @brief Initialize settings with default values
 * @param[out] settings Structure that contains DHCP client settings
 **/

void dhcpClientGetDefaultSettings(DhcpClientSettings *settings)
{
   //Use default interface
   settings->interface = netGetDefaultInterface();
   //Index of the IP address to be configured
   settings->ipAddrIndex = 0;

   //Support for quick configuration using rapid commit
   settings->rapidCommit = FALSE;
   //Use the DNS servers provided by the DHCP server
   settings->manualDnsConfig = FALSE;
   //DHCP configuration timeout
   settings->timeout = 0;

   //DHCP configuration timeout event
   settings->timeoutEvent = NULL;
   //Link state change event
   settings->linkChangeEvent = NULL;
   //FSM state change event
   settings->stateChangeEvent = NULL;

   //Add DHCP options callback
   settings->addOptionsCallback = NULL;
   //Parse DHCP options callback
   settings->parseOptionsCallback = NULL;
}


/**
 * @brief DHCP client initialization
 * @param[in] context Pointer to the DHCP client context
 * @param[in] settings DHCP client specific settings
 * @return Error code
 **/

error_t dhcpClientInit(DhcpClientContext *context,
   const DhcpClientSettings *settings)
{
   NetInterface *interface;

   //Debug message
   TRACE_INFO("Initializing DHCP client...\r\n");

   //Ensure the parameters are valid
   if(context == NULL || settings == NULL)
      return ERROR_INVALID_PARAMETER;

   //The DHCP client must be bound to a valid interface
   if(settings->interface == NULL)
      return ERROR_INVALID_PARAMETER;

   //Point to the underlying network interface
   interface = settings->interface;

   //Clear the DHCP client context
   osMemset(context, 0, sizeof(DhcpClientContext));
   //Save user settings
   context->settings = *settings;

   //DHCP client is currently suspended
   context->running = FALSE;
   //Initialize state machine
   context->state = DHCP_STATE_INIT;

   //Attach the DHCP client context to the network interface
   interface->dhcpClientContext = context;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Start DHCP client
 * @param[in] context Pointer to the DHCP client context
 * @return Error code
 **/

error_t dhcpClientStart(DhcpClientContext *context)
{
   error_t error;
   NetInterface *interface;

   //Make sure the DHCP client context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Debug message
   TRACE_INFO("Starting DHCP client...\r\n");

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Point to the underlying network interface
   interface = context->settings.interface;

   //Check the operational state of the DHCP client
   if(!context->running)
   {
      //Reset DHCP configuration
      dhcpClientResetConfig(context);

      //Initialize state machine
      context->state = DHCP_STATE_INIT;

      //Register the callback function to be called whenever a UDP datagram
      //is received on port 68
      error = udpAttachRxCallback(interface, DHCP_CLIENT_PORT,
         dhcpClientProcessMessage, context);

      //Check status code
      if(!error)
      {
         //Start DHCP client
         context->running = TRUE;
      }
   }
   else
   {
      //The DHCP client is already running
      error = ERROR_ALREADY_RUNNING;
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Return status code
   return error;
}


/**
 * @brief Stop DHCP client
 * @param[in] context Pointer to the DHCP client context
 * @return Error code
 **/

error_t dhcpClientStop(DhcpClientContext *context)
{
   NetInterface *interface;

   //Make sure the DHCP client context is valid
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Debug message
   TRACE_INFO("Stopping DHCP client...\r\n");

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Point to the underlying network interface
   interface = context->settings.interface;

   //Check whether the DHCP client is running
   if(context->running)
   {
      //Unregister callback function
      udpDetachRxCallback(interface, DHCP_CLIENT_PORT);

      //Stop DHCP client
      context->running = FALSE;
      //Reinitialize state machine
      context->state = DHCP_STATE_INIT;
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Release DHCP lease
 * @param[in] context Pointer to the DHCP client context
 * @return Error code
 **/

error_t dhcpClientRelease(DhcpClientContext *context)
{
   NetInterface *interface;

   //Check parameter
   if(context == NULL)
      return ERROR_INVALID_PARAMETER;

   //Debug message
   TRACE_INFO("Releasing DHCP lease...\r\n");

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Point to the underlying network interface
   interface = context->settings.interface;

   //Check whether the DHCP client is running
   if(context->running)
   {
      //Check current state
      if(context->state == DHCP_STATE_BOUND ||
         context->state == DHCP_STATE_RENEWING ||
         context->state == DHCP_STATE_REBINDING)
      {
         //Select a new transaction identifier
         context->transactionId = netGenerateRand();

         //The client may choose to relinquish its lease on a network address
         //by sending a DHCPRELEASE message to the server
         dhcpClientSendRelease(context);

         //The host address is no longer valid
         dhcpClientResetConfig(context);
      }

      //Unregister callback function
      udpDetachRxCallback(interface, DHCP_CLIENT_PORT);

      //Stop DHCP client
      context->running = FALSE;
      //Reinitialize state machine
      context->state = DHCP_STATE_INIT;
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Retrieve current state
 * @param[in] context Pointer to the DHCP client context
 * @return Current DHCP client state
 **/

DhcpState dhcpClientGetState(DhcpClientContext *context)
{
   DhcpState state;

   //Get exclusive access
   osAcquireMutex(&netMutex);
   //Get current state
   state = context->state;
   //Release exclusive access
   osReleaseMutex(&netMutex);

   //Return current state
   return state;
}

#endif
