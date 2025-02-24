/**
 * @file ethernet.h
 * @brief Ethernet
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

#ifndef _ETHERNET_H
#define _ETHERNET_H

//Dependencies
#include "core/net.h"

//Ethernet support
#ifndef ETH_SUPPORT
   #define ETH_SUPPORT ENABLED
#elif (ETH_SUPPORT != ENABLED && ETH_SUPPORT != DISABLED)
   #error ETH_SUPPORT parameter is not valid
#endif

//Virtual interface support
#ifndef ETH_VIRTUAL_IF_SUPPORT
   #define ETH_VIRTUAL_IF_SUPPORT DISABLED
#elif (ETH_VIRTUAL_IF_SUPPORT != ENABLED && ETH_VIRTUAL_IF_SUPPORT != DISABLED)
   #error ETH_VIRTUAL_IF_SUPPORT parameter is not valid
#endif

//VLAN support (IEEE 802.1Q)
#ifndef ETH_VLAN_SUPPORT
   #define ETH_VLAN_SUPPORT DISABLED
#elif (ETH_VLAN_SUPPORT != ENABLED && ETH_VLAN_SUPPORT != DISABLED)
   #error ETH_VLAN_SUPPORT parameter is not valid
#endif

//VMAN support (IEEE 802.1ad)
#ifndef ETH_VMAN_SUPPORT
   #define ETH_VMAN_SUPPORT DISABLED
#elif (ETH_VMAN_SUPPORT != ENABLED && ETH_VMAN_SUPPORT != DISABLED)
   #error ETH_VMAN_SUPPORT parameter is not valid
#endif

//LLC support (IEEE 802.2)
#ifndef ETH_LLC_SUPPORT
   #define ETH_LLC_SUPPORT DISABLED
#elif (ETH_LLC_SUPPORT != ENABLED && ETH_LLC_SUPPORT != DISABLED)
   #error ETH_LLC_SUPPORT parameter is not valid
#endif

//Switch port tagging
#ifndef ETH_PORT_TAGGING_SUPPORT
   #define ETH_PORT_TAGGING_SUPPORT DISABLED
#elif (ETH_PORT_TAGGING_SUPPORT != ENABLED && ETH_PORT_TAGGING_SUPPORT != DISABLED)
   #error ETH_PORT_TAGGING_SUPPORT parameter is not valid
#endif

//Size of tags used for switch port tagging
#ifndef ETH_PORT_TAG_SIZE
   #define ETH_PORT_TAG_SIZE 4
#elif (ETH_PORT_TAG_SIZE < 0)
   #error ETH_PORT_TAG_SIZE parameter is not valid
#endif

//Hardware time stamping support
#ifndef ETH_TIMESTAMP_SUPPORT
   #define ETH_TIMESTAMP_SUPPORT DISABLED
#elif (ETH_TIMESTAMP_SUPPORT != ENABLED && ETH_TIMESTAMP_SUPPORT != DISABLED)
   #error ETH_TIMESTAMP_SUPPORT parameter is not valid
#endif

//Size of the MAC address filter
#ifndef MAC_ADDR_FILTER_SIZE
   #define MAC_ADDR_FILTER_SIZE 12
#elif (MAC_ADDR_FILTER_SIZE < 1)
   #error MAC_ADDR_FILTER_SIZE parameter is not valid
#endif

//CRC32 calculation using a pre-calculated lookup table
#ifndef ETH_FAST_CRC_SUPPORT
   #define ETH_FAST_CRC_SUPPORT DISABLED
#elif (ETH_FAST_CRC_SUPPORT != ENABLED && ETH_FAST_CRC_SUPPORT != DISABLED)
   #error ETH_FAST_CRC_SUPPORT parameter is not valid
#endif

//Minimum Ethernet frame size
#define ETH_MIN_FRAME_SIZE 64
//Maximum Ethernet frame size
#define ETH_MAX_FRAME_SIZE 1518
//Ethernet header size
#define ETH_HEADER_SIZE 14
//Ethernet CRC field size
#define ETH_CRC_SIZE 4
//Ethernet maximum transmission unit
#define ETH_MTU 1500

//VLAN tag control information
#define VLAN_PCP_POS 13
#define VLAN_DEI_POS 12
#define VLAN_VID_POS 0
#define VLAN_PCP_MASK 0xE000
#define VLAN_DEI_MASK 0x1000
#define VLAN_VID_MASK 0x0FFF

//Copy MAC address
#define macCopyAddr(destMacAddr, srcMacAddr) osMemcpy(destMacAddr, srcMacAddr, sizeof(MacAddr))

//Compare MAC addresses
#define macCompAddr(macAddr1, macAddr2) (!osMemcmp(macAddr1, macAddr2, sizeof(MacAddr)))

//Determine whether a MAC address is a group address
#define macIsMulticastAddr(macAddr) (((macAddr)->b[0] & 0x01) != 0)

//Copy EUI-64 address
#define eui64CopyAddr(destEui64Addr, srcEui64Addr) osMemcpy(destEui64Addr, srcEui64Addr, sizeof(Eui64))

//Compare EUI-64 addresses
#define eui64CompAddr(eui64Addr1, eui64Addr2) (!osMemcmp(eui64Addr1, eui64Addr2, sizeof(Eui64)))

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief MAC address flags
 **/

typedef enum
{
   MAC_ADDR_FLAG_MULTICAST = 0x01,
   MAC_ADDR_FLAG_LOCAL     = 0x02
} MacAddrFlags;


/**
 * @brief Ethernet Type field
 **/

typedef enum
{
   ETH_TYPE_IPV4  = 0x0800,
   ETH_TYPE_ARP   = 0x0806,
   ETH_TYPE_RARP  = 0x8035,
   ETH_TYPE_VLAN  = 0x8100,
   ETH_TYPE_IPV6  = 0x86DD,
   ETH_TYPE_EAPOL = 0x888E,
   ETH_TYPE_VMAN  = 0x88A8,
   ETH_TYPE_LLDP  = 0x88CC,
   ETH_TYPE_PTP   = 0x88F7
} EthType;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma pack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief MAC address
 **/

typedef __packed_struct
{
   __packed_union
   {
      uint8_t b[6];
      uint16_t w[3];
   };
} MacAddr;


/**
 * @brief EUI-64 identifier
 **/

typedef __packed_struct
{
   __packed_union
   {
      uint8_t b[8];
      uint16_t w[4];
      uint32_t dw[2];
   };
} Eui64;


/**
 * @brief Ethernet frame header
 **/

typedef __packed_struct
{
   MacAddr destAddr; //0-5
   MacAddr srcAddr;  //6-11
   uint16_t type;    //12-13
   uint8_t data[];   //14
} EthHeader;


/**
 * @brief LLC header
 **/

typedef __packed_struct
{
   uint8_t dsap;    //0
   uint8_t ssap;    //1
   uint8_t control; //2
} LlcHeader;


/**
 * @brief VLAN tag
 **/

typedef __packed_struct
{
   uint16_t tci;  //0-1
   uint16_t type; //2-3
} VlanTag;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma unpack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif


/**
 * @brief MAC filter table entry
 **/

typedef struct
{
   MacAddr addr;    ///<MAC address
   uint_t refCount; ///<Reference count for the current entry
   bool_t addFlag;
   bool_t deleteFlag;
} MacFilterEntry;


/**
 * @brief LLC frame received callback
 **/

typedef void (*LlcRxCallback)(NetInterface *interface, EthHeader *header,
   const uint8_t *data, size_t length, NetRxAncillary *ancillary, void *param);


//Ethernet related constants
extern const MacAddr MAC_UNSPECIFIED_ADDR;
extern const MacAddr MAC_BROADCAST_ADDR;
extern const Eui64 EUI64_UNSPECIFIED_ADDR;

//Ethernet related functions
error_t ethInit(NetInterface *interface);

void ethProcessFrame(NetInterface *interface, uint8_t *frame, size_t length,
   NetRxAncillary *ancillary);

error_t ethSendFrame(NetInterface *interface, const MacAddr *destAddr,
   uint16_t type, NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t ethAcceptMacAddr(NetInterface *interface, const MacAddr *macAddr);
error_t ethDropMacAddr(NetInterface *interface, const MacAddr *macAddr);

error_t ethAttachLlcRxCalback(NetInterface *interface, LlcRxCallback callback,
   void *param);

error_t ethDetachLlcRxCalback(NetInterface *interface);

NetBuffer *ethAllocBuffer(size_t length, size_t *offset);

error_t macStringToAddr(const char_t *str, MacAddr *macAddr);
char_t *macAddrToString(const MacAddr *macAddr, char_t *str);
void macAddrToEui64(const MacAddr *macAddr, Eui64 *interfaceId);

void ethDumpHeader(const EthHeader *ethHeader);

error_t eui64StringToAddr(const char_t *str, Eui64 *eui64);
char_t *eui64AddrToString(const Eui64 *eui64, char_t *str);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
