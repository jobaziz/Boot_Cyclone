/**
 * @file rx62n_eth_driver.h
 * @brief Renesas RX62N Ethernet MAC driver
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

#ifndef _RX62N_ETH_DRIVER_H
#define _RX62N_ETH_DRIVER_H

//Dependencies
#include "core/nic.h"

//Number of TX buffers
#ifndef RX62N_ETH_TX_BUFFER_COUNT
   #define RX62N_ETH_TX_BUFFER_COUNT 3
#elif (RX62N_ETH_TX_BUFFER_COUNT < 1)
   #error RX62N_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#ifndef RX62N_ETH_TX_BUFFER_SIZE
   #define RX62N_ETH_TX_BUFFER_SIZE 1536
#elif (RX62N_ETH_TX_BUFFER_SIZE != 1536)
   #error RX62N_ETH_TX_BUFFER_SIZE parameter is not valid
#endif

//Number of RX buffers
#ifndef RX62N_ETH_RX_BUFFER_COUNT
   #define RX62N_ETH_RX_BUFFER_COUNT 6
#elif (RX62N_ETH_RX_BUFFER_COUNT < 1)
   #error RX62N_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#ifndef RX62N_ETH_RX_BUFFER_SIZE
   #define RX62N_ETH_RX_BUFFER_SIZE 1536
#elif (RX62N_ETH_RX_BUFFER_SIZE != 1536)
   #error RX62N_ETH_RX_BUFFER_SIZE parameter is not valid
#endif

//Ethernet interrupt priority
#ifndef RX62N_ETH_IRQ_PRIORITY
   #define RX62N_ETH_IRQ_PRIORITY 2
#elif (RX62N_ETH_IRQ_PRIORITY < 0)
   #error RX62N_ETH_IRQ_PRIORITY parameter is not valid
#endif

//EESR register
#define EDMAC_EESR_TWB     0x40000000
#define EDMAC_EESR_TABT    0x04000000
#define EDMAC_EESR_RABT    0x02000000
#define EDMAC_EESR_RFCOF   0x01000000
#define EDMAC_EESR_ADE     0x00800000
#define EDMAC_EESR_ECI     0x00400000
#define EDMAC_EESR_TC      0x00200000
#define EDMAC_EESR_TDE     0x00100000
#define EDMAC_EESR_TFUF    0x00080000
#define EDMAC_EESR_FR      0x00040000
#define EDMAC_EESR_RDE     0x00020000
#define EDMAC_EESR_RFOF    0x00010000
#define EDMAC_EESR_CND     0x00000800
#define EDMAC_EESR_DLC     0x00000400
#define EDMAC_EESR_CD      0x00000200
#define EDMAC_EESR_TRO     0x00000100
#define EDMAC_EESR_RMAF    0x00000080
#define EDMAC_EESR_RRF     0x00000010
#define EDMAC_EESR_RTLF    0x00000008
#define EDMAC_EESR_RTSF    0x00000004
#define EDMAC_EESR_PRE     0x00000002
#define EDMAC_EESR_CERF    0x00000001

//Transmit DMA descriptor flags
#define EDMAC_TD0_TACT     0x80000000
#define EDMAC_TD0_TDLE     0x40000000
#define EDMAC_TD0_TFP_SOF  0x20000000
#define EDMAC_TD0_TFP_EOF  0x10000000
#define EDMAC_TD0_TFE      0x08000000
#define EDMAC_TD0_TWBI     0x04000000
#define EDMAC_TD0_TFS_MASK 0x0000010F
#define EDMAC_TD0_TFS_TABT 0x00000100
#define EDMAC_TD0_TFS_CND  0x00000008
#define EDMAC_TD0_TFS_DLC  0x00000004
#define EDMAC_TD0_TFS_CD   0x00000002
#define EDMAC_TD0_TFS_TRO  0x00000001
#define EDMAC_TD1_TBL      0xFFFF0000
#define EDMAC_TD2_TBA      0xFFFFFFFF

//Receive DMA descriptor flags
#define EDMAC_RD0_RACT     0x80000000
#define EDMAC_RD0_RDLE     0x40000000
#define EDMAC_RD0_RFP_SOF  0x20000000
#define EDMAC_RD0_RFP_EOF  0x10000000
#define EDMAC_RD0_RFE      0x08000000
#define EDMAC_RD0_RFS_MASK 0x0000039F
#define EDMAC_RD0_RFS_RFOF 0x00000200
#define EDMAC_RD0_RFS_RABT 0x00000100
#define EDMAC_RD0_RFS_RMAF 0x00000080
#define EDMAC_RD0_RFS_RRF  0x00000010
#define EDMAC_RD0_RFS_RTLF 0x00000008
#define EDMAC_RD0_RFS_RTSF 0x00000004
#define EDMAC_RD0_RFS_PRE  0x00000002
#define EDMAC_RD0_RFS_CERF 0x00000001
#define EDMAC_RD1_RBL      0xFFFF0000
#define EDMAC_RD1_RFL      0x0000FFFF
#define EDMAC_RD2_RBA      0xFFFFFFFF

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Transmit DMA descriptor
 **/

typedef struct
{
   uint32_t td0;
   uint32_t td1;
   uint32_t td2;
   uint32_t padding;
} Rx62nTxDmaDesc;


/**
 * @brief Receive DMA descriptor
 **/

typedef struct
{
   uint32_t rd0;
   uint32_t rd1;
   uint32_t rd2;
   uint32_t padding;
} Rx62nRxDmaDesc;


//RX62N Ethernet MAC driver
extern const NicDriver rx62nEthDriver;

//RX62N Ethernet MAC related functions
error_t rx62nEthInit(NetInterface *interface);
void rx62nEthInitGpio(NetInterface *interface);
void rx62nEthInitDmaDesc(NetInterface *interface);

void rx62nEthTick(NetInterface *interface);

void rx62nEthEnableIrq(NetInterface *interface);
void rx62nEthDisableIrq(NetInterface *interface);
void rx62nEthEventHandler(NetInterface *interface);

error_t rx62nEthSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary);

error_t rx62nEthReceivePacket(NetInterface *interface);

error_t rx62nEthUpdateMacAddrFilter(NetInterface *interface);
error_t rx62nEthUpdateMacConfig(NetInterface *interface);

void rx62nEthWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t rx62nEthReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

void rx62nEthWriteSmi(uint32_t data, uint_t length);
uint32_t rx62nEthReadSmi(uint_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
