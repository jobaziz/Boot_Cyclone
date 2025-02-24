/**
 * @file nbns_common.h
 * @brief Definitions common to NBNS client and NBNS responder
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

#ifndef _NBNS_COMMON_H
#define _NBNS_COMMON_H

//Dependencies
#include "core/net.h"
#include "dns/dns_common.h"

//Default resource record TTL (cache lifetime)
#ifndef NBNS_DEFAULT_RESOURCE_RECORD_TTL
   #define NBNS_DEFAULT_RESOURCE_RECORD_TTL 120
#elif (NBNS_DEFAULT_RESOURCE_RECORD_TTL < 1)
   #error NBNS_DEFAULT_RESOURCE_RECORD_TTL parameter is not valid
#endif

//NBNS port number
#define NBNS_PORT 137

//Macro definition
#define NBNS_ENCODE_H(c) ('A' + (((c) >> 4) & 0x0F))
#define NBNS_ENCODE_L(c) ('A' + ((c) & 0x0F))

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief NBNS flags
 **/

typedef enum
{
   NBNS_ONT_BNODE = 0x0000,
   NBNS_ONT_PNODE = 0x2000,
   NBNS_ONT_MNODE = 0x4000,
   NBNS_G_UNIQUE  = 0x0000,
   NBNS_G_GROUP   = 0x8000
} DnsFlags;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma pack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief NBNS message header
 **/

typedef __packed_struct
{
   uint16_t id;         //0-1
#if defined(_CPU_BIG_ENDIAN) && !defined(__ICCRX__)
   uint8_t qr : 1;      //2
   uint8_t opcode : 4;
   uint8_t aa : 1;
   uint8_t tc : 1;
   uint8_t rd : 1;
   uint8_t ra : 1;      //3
   uint8_t z : 2;
   uint8_t b : 1;
   uint8_t rcode : 4;
#else
   uint8_t rd : 1;      //2
   uint8_t tc : 1;
   uint8_t aa : 1;
   uint8_t opcode : 4;
   uint8_t qr : 1;
   uint8_t rcode : 4;   //3
   uint8_t b : 1;
   uint8_t z : 2;
   uint8_t ra : 1;
#endif
   uint16_t qdcount;    //4-5
   uint16_t ancount;    //6-7
   uint16_t nscount;    //8-9
   uint16_t arcount;    //10-11
   uint8_t questions[]; //12
} NbnsHeader;


/**
 * @brief NBNS address entry
 **/

typedef __packed_struct
{
   uint16_t flags;
   Ipv4Addr addr;
} NbnsAddrEntry;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma unpack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif

//NBNS related functions
error_t nbnsInit(NetInterface *interface);

void nbnsProcessMessage(NetInterface *interface,
   const IpPseudoHeader *pseudoHeader, const UdpHeader *udpHeader,
   const NetBuffer *buffer, size_t offset, const NetRxAncillary *ancillary,
   void *param);

size_t nbnsEncodeName(const char_t *src, uint8_t *dest);

size_t nbnsParseName(const NbnsHeader *message,
   size_t length, size_t pos, char_t *dest);

bool_t nbnsCompareName(const NbnsHeader *message,
   size_t length, size_t pos, const char_t *name);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
