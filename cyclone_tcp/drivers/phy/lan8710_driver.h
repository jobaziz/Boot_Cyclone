/**
 * @file lan8710_driver.h
 * @brief LAN8710 Ethernet PHY driver
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

#ifndef _LAN8710_DRIVER_H
#define _LAN8710_DRIVER_H

//Dependencies
#include "core/nic.h"

//PHY address
#ifndef LAN8710_PHY_ADDR
   #define LAN8710_PHY_ADDR 0
#elif (LAN8710_PHY_ADDR < 0 || LAN8710_PHY_ADDR > 31)
   #error LAN8710_PHY_ADDR parameter is not valid
#endif

//LAN8710 PHY registers
#define LAN8710_BMCR                      0x00
#define LAN8710_BMSR                      0x01
#define LAN8710_PHYID1                    0x02
#define LAN8710_PHYID2                    0x03
#define LAN8710_ANAR                      0x04
#define LAN8710_ANLPAR                    0x05
#define LAN8710_ANER                      0x06
#define LAN8710_MCSR                      0x11
#define LAN8710_SMR                       0x12
#define LAN8710_SECR                      0x1A
#define LAN8710_SCSIR                     0x1B
#define LAN8710_ISR                       0x1D
#define LAN8710_IMR                       0x1E
#define LAN8710_PSCSR                     0x1F

//Basic Control register
#define LAN8710_BMCR_RESET                0x8000
#define LAN8710_BMCR_LOOPBACK             0x4000
#define LAN8710_BMCR_SPEED_SEL            0x2000
#define LAN8710_BMCR_AN_EN                0x1000
#define LAN8710_BMCR_POWER_DOWN           0x0800
#define LAN8710_BMCR_ISOLATE              0x0400
#define LAN8710_BMCR_RESTART_AN           0x0200
#define LAN8710_BMCR_DUPLEX_MODE          0x0100
#define LAN8710_BMCR_COL_TEST             0x0080

//Basic Status register
#define LAN8710_BMSR_100BT4               0x8000
#define LAN8710_BMSR_100BTX_FD            0x4000
#define LAN8710_BMSR_100BTX_HD            0x2000
#define LAN8710_BMSR_10BT_FD              0x1000
#define LAN8710_BMSR_10BT_HD              0x0800
#define LAN8710_BMSR_100BT2_FD            0x0400
#define LAN8710_BMSR_100BT2_HD            0x0200
#define LAN8710_BMSR_EXTENDED_STATUS      0x0100
#define LAN8710_BMSR_AN_COMPLETE          0x0020
#define LAN8710_BMSR_REMOTE_FAULT         0x0010
#define LAN8710_BMSR_AN_CAPABLE           0x0008
#define LAN8710_BMSR_LINK_STATUS          0x0004
#define LAN8710_BMSR_JABBER_DETECT        0x0002
#define LAN8710_BMSR_EXTENDED_CAPABLE     0x0001

//PHY Identifier 1 register
#define LAN8710_PHYID1_PHY_ID_MSB         0xFFFF
#define LAN8710_PHYID1_PHY_ID_MSB_DEFAULT 0x0007

//PHY Identifier 2 register
#define LAN8710_PHYID2_PHY_ID_LSB         0xFC00
#define LAN8710_PHYID2_PHY_ID_LSB_DEFAULT 0xC000
#define LAN8710_PHYID2_MODEL_NUM          0x03F0
#define LAN8710_PHYID2_MODEL_NUM_DEFAULT  0x00F0
#define LAN8710_PHYID2_REVISION_NUM       0x000F

//Auto-Negotiation Advertisement register
#define LAN8710_ANAR_REMOTE_FAULT         0x2000
#define LAN8710_ANAR_PAUSE                0x0C00
#define LAN8710_ANAR_100BTX_FD            0x0100
#define LAN8710_ANAR_100BTX_HD            0x0080
#define LAN8710_ANAR_10BT_FD              0x0040
#define LAN8710_ANAR_10BT_HD              0x0020
#define LAN8710_ANAR_SELECTOR             0x001F
#define LAN8710_ANAR_SELECTOR_DEFAULT     0x0001

//Auto-Negotiation Link Partner Ability register
#define LAN8710_ANLPAR_NEXT_PAGE          0x8000
#define LAN8710_ANLPAR_ACK                0x4000
#define LAN8710_ANLPAR_REMOTE_FAULT       0x2000
#define LAN8710_ANLPAR_PAUSE              0x0400
#define LAN8710_ANLPAR_100BT4             0x0200
#define LAN8710_ANLPAR_100BTX_FD          0x0100
#define LAN8710_ANLPAR_100BTX_HD          0x0080
#define LAN8710_ANLPAR_10BT_FD            0x0040
#define LAN8710_ANLPAR_10BT_HD            0x0020
#define LAN8710_ANLPAR_SELECTOR           0x001F
#define LAN8710_ANLPAR_SELECTOR_DEFAULT   0x0001

//Auto-Negotiation Expansion register
#define LAN8710_ANER_PAR_DETECT_FAULT     0x0010
#define LAN8710_ANER_LP_NEXT_PAGE_ABLE    0x0008
#define LAN8710_ANER_NEXT_PAGE_ABLE       0x0004
#define LAN8710_ANER_PAGE_RECEIVED        0x0002
#define LAN8710_ANER_LP_AN_ABLE           0x0001

//Mode Control/Status register
#define LAN8710_MCSR_EDPWRDOWN            0x2000
#define LAN8710_MCSR_FARLOOPBACK          0x0200
#define LAN8710_MCSR_ALTINT               0x0040
#define LAN8710_MCSR_ENERGYON             0x0002

//Special Modes register
#define LAN8710_SMR_MIIMODE               0x4000
#define LAN8710_SMR_MODE                  0x00E0
#define LAN8710_SMR_PHYAD                 0x001F

//Symbol Error Counter register
#define LAN8710_SECR_SYM_ERR_CNT          0xFFFF

//Special Control/Status Indication register
#define LAN8710_SCSIR_AMDIXCTRL           0x8000
#define LAN8710_SCSIR_CH_SELECT           0x2000
#define LAN8710_SCSIR_SQEOFF              0x0800
#define LAN8710_SCSIR_XPOL                0x0010

//Interrupt Source register
#define LAN8710_ISR_ENERGYON              0x0080
#define LAN8710_ISR_AN_COMPLETE           0x0040
#define LAN8710_ISR_REMOTE_FAULT          0x0020
#define LAN8710_ISR_LINK_DOWN             0x0010
#define LAN8710_ISR_AN_LP_ACK             0x0008
#define LAN8710_ISR_PAR_DETECT_FAULT      0x0004
#define LAN8710_ISR_AN_PAGE_RECEIVED      0x0002

//Interrupt Mask register
#define LAN8710_IMR_ENERGYON              0x0080
#define LAN8710_IMR_AN_COMPLETE           0x0040
#define LAN8710_IMR_REMOTE_FAULT          0x0020
#define LAN8710_IMR_LINK_DOWN             0x0010
#define LAN8710_IMR_AN_LP_ACK             0x0008
#define LAN8710_IMR_PAR_DETECT_FAULT      0x0004
#define LAN8710_IMR_AN_PAGE_RECEIVED      0x0002

//PHY Special Control/Status register
#define LAN8710_PSCSR_AUTODONE            0x1000
#define LAN8710_PSCSR_HCDSPEED            0x001C
#define LAN8710_PSCSR_HCDSPEED_10BT_HD    0x0004
#define LAN8710_PSCSR_HCDSPEED_100BTX_HD  0x0008
#define LAN8710_PSCSR_HCDSPEED_10BT_FD    0x0014
#define LAN8710_PSCSR_HCDSPEED_100BTX_FD  0x0018

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//LAN8710 Ethernet PHY driver
extern const PhyDriver lan8710PhyDriver;

//LAN8710 related functions
error_t lan8710Init(NetInterface *interface);
void lan8710InitHook(NetInterface *interface);

void lan8710Tick(NetInterface *interface);

void lan8710EnableIrq(NetInterface *interface);
void lan8710DisableIrq(NetInterface *interface);

void lan8710EventHandler(NetInterface *interface);

void lan8710WritePhyReg(NetInterface *interface, uint8_t address,
   uint16_t data);

uint16_t lan8710ReadPhyReg(NetInterface *interface, uint8_t address);

void lan8710DumpPhyReg(NetInterface *interface);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
