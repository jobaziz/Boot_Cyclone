/**
 * @file mkv5x_eth_driver.h
 * @brief NXP Kinetis KV5x Ethernet MAC driver
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

#ifndef _MKV5X_ETH_DRIVER_H
#define _MKV5X_ETH_DRIVER_H

//Number of TX buffers
#ifndef MKV5X_ETH_TX_BUFFER_COUNT
   #define MKV5X_ETH_TX_BUFFER_COUNT 2
#elif (MKV5X_ETH_TX_BUFFER_COUNT < 1)
   #error MKV5X_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef MKV5X_ETH_TX_BUFFER_SIZE
   #define MKV5X_ETH_TX_BUFFER_SIZE 1536
#elif (MKV5X_ETH_TX_BUFFER_SIZE != 1536)
   #error MKV5X_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef MKV5X_ETH_RX_BUFFER_COUNT
   #define MKV5X_ETH_RX_BUFFER_COUNT 4
#elif (MKV5X_ETH_RX_BUFFER_COUNT < 1)
   #error MKV5X_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef MKV5X_ETH_RX_BUFFER_SIZE
   #define MKV5X_ETH_RX_BUFFER_SIZE 1536
#elif (MKV5X_ETH_RX_BUFFER_SIZE != 1536)
   #error MKV5X_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Interrupt priority grouping
#ifndef MKV5X_ETH_IRQ_PRIORITY_GROUPING
   #define MKV5X_ETH_IRQ_PRIORITY_GROUPING 3
#elif (MKV5X_ETH_IRQ_PRIORITY_GROUPING < 0)
   #error MKV5X_ETH_IRQ_PRIORITY_GROUPING parameter is not valid
#endif

//Ethernet interrupt group priority
#ifndef MKV5X_ETH_IRQ_GROUP_PRIORITY
   #define MKV5X_ETH_IRQ_GROUP_PRIORITY 12
#elif (MKV5X_ETH_IRQ_GROUP_PRIORITY < 0)
   #error MKV5X_ETH_IRQ_GROUP_PRIORITY parameter is not valid
#endif

//Ethernet interrupt subpriority
#ifndef MKV5X_ETH_IRQ_SUB_PRIORITY
   #define MKV5X_ETH_IRQ_SUB_PRIORITY 0
#elif (MKV5X_ETH_IRQ_SUB_PRIORITY < 0)
   #error MKV5X_ETH_IRQ_SUB_PRIORITY parameter is not valid
#endif

//Name of the section where to place DMA buffers
#ifndef MKV5X_ETH_RAM_SECTION
   #define MKV5X_ETH_RAM_SECTION ".ram_no_cache"
#endif

//Enhanced transmit buffer descriptor
#define ENET_TBD0_R                0x80000000
#define ENET_TBD0_TO1              0x40000000
#define ENET_TBD0_W                0x20000000
#define ENET_TBD0_TO2              0x10000000
#define ENET_TBD0_L                0x08000000
#define ENET_TBD0_TC               0x04000000
#define ENET_TBD0_DATA_LENGTH      0x0000FFFF
#define ENET_TBD1_DATA_POINTER     0xFFFFFFFF
#define ENET_TBD2_INT              0x40000000
#define ENET_TBD2_TS               0x20000000
#define ENET_TBD2_PINS             0x10000000
#define ENET_TBD2_IINS             0x08000000
#define ENET_TBD2_TXE              0x00008000
#define ENET_TBD2_UE               0x00002000
#define ENET_TBD2_EE               0x00001000
#define ENET_TBD2_FE               0x00000800
#define ENET_TBD2_LCE              0x00000400
#define ENET_TBD2_OE               0x00000200
#define ENET_TBD2_TSE              0x00000100
#define ENET_TBD4_BDU              0x80000000
#define ENET_TBD5_TIMESTAMP        0xFFFFFFFF

//Enhanced receive buffer descriptor
#define ENET_RBD0_E                0x80000000
#define ENET_RBD0_RO1              0x40000000
#define ENET_RBD0_W                0x20000000
#define ENET_RBD0_RO2              0x10000000
#define ENET_RBD0_L                0x08000000
#define ENET_RBD0_M                0x01000000
#define ENET_RBD0_BC               0x00800000
#define ENET_RBD0_MC               0x00400000
#define ENET_RBD0_LG               0x00200000
#define ENET_RBD0_NO               0x00100000
#define ENET_RBD0_CR               0x00040000
#define ENET_RBD0_OV               0x00020000
#define ENET_RBD0_TR               0x00010000
#define ENET_RBD0_DATA_LENGTH      0x0000FFFF
#define ENET_RBD1_DATA_POINTER     0xFFFFFFFF
#define ENET_RBD2_ME               0x80000000
#define ENET_RBD2_PE               0x04000000
#define ENET_RBD2_CE               0x02000000
#define ENET_RBD2_UC               0x01000000
#define ENET_RBD2_INT              0x00800000
#define ENET_RBD2_VPCP             0x0000E000
#define ENET_RBD2_ICE              0x00000020
#define ENET_RBD2_PCR              0x00000010
#define ENET_RBD2_VLAN             0x00000004
#define ENET_RBD2_IPV6             0x00000002
#define ENET_RBD2_FRAG             0x00000001
#define ENET_RBD3_HEADER_LENGTH    0xF8000000
#define ENET_RBD3_PROTOCOL_TYPE    0x00FF0000
#define ENET_RBD3_PAYLOAD_CHECKSUM 0x0000FFFF
#define ENET_RBD4_BDU              0x80000000
#define ENET_RBD5_TIMESTAMP        0xFFFFFFFF

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Kinetis KV5x Ethernet MAC driver
extern const NicDriver mkv5xEthDriver;

//Kinetis KV5x Ethernet MAC related functions
error_t mkv5xEthInit(NetInterface *interface);
void mkv5xEthInitGpio(NetInterface *interface);
void mkv5xEthInitBufferDesc(NetInterface *interface);

void mkv5xEthTick(NetInterface *interface);

void mkv5xEthEnableIrq(NetInterface *interface);
void mkv5xEthDisableIrq(NetInterface *interface);
void mkv5xEthEventHandler(NetInterface *interface);

error_t mkv5xEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t mkv5xEthReceivePacket(NetInterface *interface);

error_t mkv5xEthUpdateMacAddrFilter(NetInterface *interface);
error_t mkv5xEthUpdateMacConfig(NetInterface *interface);

void mkv5xEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t mkv5xEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

uint32_t mkv5xEthCalcCrc(const void *data, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
