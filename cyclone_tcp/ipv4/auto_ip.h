/**
 * @file auto_ip.h
 * @brief Auto-IP (Dynamic Configuration of IPv4 Link-Local Addresses)
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

#ifndef _AUTO_IP_H
#define _AUTO_IP_H

//Dependencies
#include "core/net.h"

//Auto-IP support
#ifndef AUTO_IP_SUPPORT
   #define AUTO_IP_SUPPORT DISABLED
#elif (AUTO_IP_SUPPORT != ENABLED && AUTO_IP_SUPPORT != DISABLED)
   #error AUTO_IP_SUPPORT parameter is not valid
#endif

//Bonjour Conformance Test support
#ifndef AUTO_IP_BCT_SUPPORT
   #define AUTO_IP_BCT_SUPPORT DISABLED
#elif (AUTO_IP_BCT_SUPPORT != ENABLED && AUTO_IP_BCT_SUPPORT != DISABLED)
   #error AUTO_IP_BCT_SUPPORT parameter is not valid
#endif

//Auto-IP tick interval
#ifndef AUTO_IP_TICK_INTERVAL
   #define AUTO_IP_TICK_INTERVAL 200
#elif (AUTO_IP_TICK_INTERVAL < 10)
   #error AUTO_IP_TICK_INTERVAL parameter is not valid
#endif

//Initial random delay
#ifndef AUTO_IP_PROBE_WAIT
   #define AUTO_IP_PROBE_WAIT 1000
#elif (AUTO_IP_PROBE_WAIT < 0)
   #error AUTO_IP_PROBE_WAIT parameter is not valid
#endif

//Number of probe packets
#ifndef AUTO_IP_PROBE_NUM
   #define AUTO_IP_PROBE_NUM 3
#elif (AUTO_IP_PROBE_NUM < 1)
   #error AUTO_IP_PROBE_NUM parameter is not valid
#endif

//Minimum delay till repeated probe
#ifndef AUTO_IP_PROBE_MIN
   #define AUTO_IP_PROBE_MIN 1000
#elif (AUTO_IP_PROBE_MIN < 100)
   #error AUTO_IP_PROBE_MIN parameter is not valid
#endif

//Maximum delay till repeated probe
#ifndef AUTO_IP_PROBE_MAX
   #define AUTO_IP_PROBE_MAX 2000
#elif (AUTO_IP_PROBE_MAX < AUTO_IP_PROBE_MIN)
   #error AUTO_IP_PROBE_MAX parameter is not valid
#endif

//Delay before announcing
#ifndef AUTO_IP_ANNOUNCE_WAIT
   #define AUTO_IP_ANNOUNCE_WAIT 2000
#elif (AUTO_IP_ANNOUNCE_WAIT < 100)
   #error AUTO_IP_ANNOUNCE_WAIT parameter is not valid
#endif

//Number of announcement packets
#ifndef AUTO_IP_ANNOUNCE_NUM
   #define AUTO_IP_ANNOUNCE_NUM 2
#elif (AUTO_IP_ANNOUNCE_NUM < 1)
   #error AUTO_IP_ANNOUNCE_NUM parameter is not valid
#endif

//Time between announcement packets
#ifndef AUTO_IP_ANNOUNCE_INTERVAL
   #define AUTO_IP_ANNOUNCE_INTERVAL 2000
#elif (AUTO_IP_ANNOUNCE_INTERVAL < 100)
   #error AUTO_IP_ANNOUNCE_INTERVAL parameter is not valid
#endif

//Max conflicts before rate limiting
#ifndef AUTO_IP_MAX_CONFLICTS
   #define AUTO_IP_MAX_CONFLICTS 10
#elif (AUTO_IP_MAX_CONFLICTS < 1)
   #error AUTO_IP_MAX_CONFLICTS parameter is not valid
#endif

//Delay between successive attempts
#ifndef AUTO_IP_RATE_LIMIT_INTERVAL
   #define AUTO_IP_RATE_LIMIT_INTERVAL 60000
#elif (AUTO_IP_RATE_LIMIT_INTERVAL < 1000)
   #error AUTO_IP_RATE_LIMIT_INTERVAL parameter is not valid
#endif

//Minimum interval between defensive
#ifndef AUTO_IP_DEFEND_INTERVAL
   #define AUTO_IP_DEFEND_INTERVAL 10000
#elif (AUTO_IP_DEFEND_INTERVAL < 1000)
   #error AUTO_IP_DEFEND_INTERVAL parameter is not valid
#endif

//Auto-IP address prefix
#define AUTO_IP_PREFIX IPV4_ADDR(169, 254, 0, 0)
//Auto-IP subnet mask
#define AUTO_IP_MASK IPV4_ADDR(255, 255, 0, 0)

//Auto-IP address range
#define AUTO_IP_ADDR_MIN IPV4_ADDR(169, 254, 1, 0)
#define AUTO_IP_ADDR_MAX IPV4_ADDR(169, 254, 254, 255)

//Forward declaration of AutoIpContext structure
struct _AutoIpContext;
#define AutoIpContext struct _AutoIpContext

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Auto-IP FSM states
 **/

typedef enum
{
   AUTO_IP_STATE_INIT       = 0,
   AUTO_IP_STATE_PROBING    = 1,
   AUTO_IP_STATE_ANNOUNCING = 2,
   AUTO_IP_STATE_CONFIGURED = 3,
   AUTO_IP_STATE_DEFENDING  = 4
} AutoIpState;


/**
 * @brief Link state change callback
 **/

typedef void (*AutoIpLinkChangeCallback)(AutoIpContext *context,
   NetInterface *interface, bool_t linkState);


/**
 * @brief FSM state change callback
 **/

typedef void (*AutoIpStateChangeCallback)(AutoIpContext *context,
   NetInterface *interface, AutoIpState state);


/**
 * @brief Auto-IP settings
 **/

typedef struct
{
   NetInterface *interface;                    ///<Network interface to configure
   uint_t ipAddrIndex;                         ///<Index of the IP address to be configured
   Ipv4Addr linkLocalAddr;                     ///<Initial link-local address to be used
   AutoIpLinkChangeCallback linkChangeEvent;   ///<Link state change event
   AutoIpStateChangeCallback stateChangeEvent; ///<FSM state change event
} AutoIpSettings;


/**
 * @brief Auto-IP context
 **/

struct _AutoIpContext
{
   AutoIpSettings settings; ///<Auto-IP settings
   bool_t running;          ///<Auto-IP is currently running
   AutoIpState state;       ///<Current state of the FSM
   Ipv4Addr linkLocalAddr;  ///<Link-local address
   systime_t timestamp;     ///<Timestamp to manage retransmissions
   systime_t timeout;       ///<Timeout value
   uint_t retransmitCount;  ///<Retransmission counter
   uint_t conflictCount;    ///<Number of conflicts
};


//Auto-IP related functions
void autoIpGetDefaultSettings(AutoIpSettings *settings);
error_t autoIpInit(AutoIpContext *context, const AutoIpSettings *settings);

error_t autoIpStart(AutoIpContext *context);
error_t autoIpStop(AutoIpContext *context);

AutoIpState autoIpGetState(AutoIpContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
