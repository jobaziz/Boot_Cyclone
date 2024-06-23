/**
 * @file lan8741_driver.c
 * @brief LAN8741 Ethernet PHY driver
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
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "drivers/phy/lan8741_driver.h"
#include "debug.h"


/**
 * @brief LAN8741 Ethernet PHY driver
 **/

const PhyDriver lan8741PhyDriver =
{
   lan8741Init,
   lan8741Tick,
   lan8741EnableIrq,
   lan8741DisableIrq,
   lan8741EventHandler
};


/**
 * @brief LAN8741 PHY transceiver initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t lan8741Init(NetInterface *interface)
{
   //Debug message
   TRACE_INFO("Initializing LAN8741...\r\n");

   //Undefined PHY address?
   if(interface->phyAddr >= 32)
   {
      //Use the default address
      interface->phyAddr = LAN8741_PHY_ADDR;
   }

   //Initialize serial management interface
   if(interface->smiDriver != NULL)
   {
      interface->smiDriver->init();
   }

   //Initialize external interrupt line driver
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->init();
   }

   //Reset PHY transceiver (soft reset)
   lan8741WritePhyReg(interface, LAN8741_BMCR, LAN8741_BMCR_RESET);

   //Wait for the reset to complete
   while(lan8741ReadPhyReg(interface, LAN8741_BMCR) & LAN8741_BMCR_RESET)
   {
   }

   //Dump PHY registers for debugging purpose
   lan8741DumpPhyReg(interface);

   //The PHY will generate interrupts when link status changes are detected
   lan8741WritePhyReg(interface, LAN8741_IMR, LAN8741_IMR_AN_COMPLETE |
      LAN8741_IMR_LINK_DOWN);

   //Perform custom configuration
   lan8741InitHook(interface);

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief LAN8741 custom configuration
 * @param[in] interface Underlying network interface
 **/

__weak_func void lan8741InitHook(NetInterface *interface)
{
}


/**
 * @brief LAN8741 timer handler
 * @param[in] interface Underlying network interface
 **/

void lan8741Tick(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //No external interrupt line driver?
   if(interface->extIntDriver == NULL)
   {
      //Read basic status register
      value = lan8741ReadPhyReg(interface, LAN8741_BMSR);
      //Retrieve current link state
      linkState = (value & LAN8741_BMSR_LINK_STATUS) ? TRUE : FALSE;

      //Link up event?
      if(linkState && !interface->linkState)
      {
         //Set event flag
         interface->phyEvent = TRUE;
         //Notify the TCP/IP stack of the event
         osSetEvent(&netEvent);
      }
      //Link down event?
      else if(!linkState && interface->linkState)
      {
         //Set event flag
         interface->phyEvent = TRUE;
         //Notify the TCP/IP stack of the event
         osSetEvent(&netEvent);
      }
   }
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void lan8741EnableIrq(NetInterface *interface)
{
   //Enable PHY transceiver interrupts
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->enableIrq();
   }
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void lan8741DisableIrq(NetInterface *interface)
{
   //Disable PHY transceiver interrupts
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->disableIrq();
   }
}


/**
 * @brief LAN8741 event handler
 * @param[in] interface Underlying network interface
 **/

void lan8741EventHandler(NetInterface *interface)
{
   uint16_t value;

   //Read status register to acknowledge the interrupt
   value = lan8741ReadPhyReg(interface, LAN8741_ISR);

   //Link status change?
   if((value & (LAN8741_IMR_AN_COMPLETE | LAN8741_IMR_LINK_DOWN)) != 0)
   {
      //Any link failure condition is latched in the BMSR register. Reading
      //the register twice will always return the actual link status
      value = lan8741ReadPhyReg(interface, LAN8741_BMSR);
      value = lan8741ReadPhyReg(interface, LAN8741_BMSR);

      //Link is up?
      if((value & LAN8741_BMSR_LINK_STATUS) != 0)
      {
         //Read PHY special control/status register
         value = lan8741ReadPhyReg(interface, LAN8741_PSCSR);

         //Check current operation mode
         switch(value & LAN8741_PSCSR_HCDSPEED)
         {
         //10BASE-T half-duplex
         case LAN8741_PSCSR_HCDSPEED_10BT_HD:
            interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
            interface->duplexMode = NIC_HALF_DUPLEX_MODE;
            break;

         //10BASE-T full-duplex
         case LAN8741_PSCSR_HCDSPEED_10BT_FD:
            interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
            interface->duplexMode = NIC_FULL_DUPLEX_MODE;
            break;

         //100BASE-TX half-duplex
         case LAN8741_PSCSR_HCDSPEED_100BTX_HD:
            interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
            interface->duplexMode = NIC_HALF_DUPLEX_MODE;
            break;

         //100BASE-TX full-duplex
         case LAN8741_PSCSR_HCDSPEED_100BTX_FD:
            interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
            interface->duplexMode = NIC_FULL_DUPLEX_MODE;
            break;

         //Unknown operation mode
         default:
            //Debug message
            TRACE_WARNING("Invalid operation mode!\r\n");
            break;
         }

         //Update link state
         interface->linkState = TRUE;

         //Adjust MAC configuration parameters for proper operation
         interface->nicDriver->updateMacConfig(interface);
      }
      else
      {
         //Update link state
         interface->linkState = FALSE;
      }

      //Process link state change event
      nicNotifyLinkChange(interface);
   }
}


/**
 * @brief Write PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @param[in] data Register value
 **/

void lan8741WritePhyReg(NetInterface *interface, uint8_t address,
   uint16_t data)
{
   //Write the specified PHY register
   if(interface->smiDriver != NULL)
   {
      interface->smiDriver->writePhyReg(SMI_OPCODE_WRITE,
         interface->phyAddr, address, data);
   }
   else
   {
      interface->nicDriver->writePhyReg(SMI_OPCODE_WRITE,
         interface->phyAddr, address, data);
   }
}


/**
 * @brief Read PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @return Register value
 **/

uint16_t lan8741ReadPhyReg(NetInterface *interface, uint8_t address)
{
   uint16_t data;

   //Read the specified PHY register
   if(interface->smiDriver != NULL)
   {
      data = interface->smiDriver->readPhyReg(SMI_OPCODE_READ,
         interface->phyAddr, address);
   }
   else
   {
      data = interface->nicDriver->readPhyReg(SMI_OPCODE_READ,
         interface->phyAddr, address);
   }

   //Return the value of the PHY register
   return data;
}


/**
 * @brief Dump PHY registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void lan8741DumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i,
         lan8741ReadPhyReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}


/**
 * @brief Write MMD register
 * @param[in] interface Underlying network interface
 * @param[in] devAddr Device address
 * @param[in] regAddr Register address
 * @param[in] data MMD register value
 **/

void lan8741WriteMmdReg(NetInterface *interface, uint8_t devAddr,
   uint16_t regAddr, uint16_t data)
{
   //Select register operation
   lan8741WritePhyReg(interface, LAN8741_MMDACR,
      LAN8741_MMDACR_FUNC_ADDR | (devAddr & LAN8741_MMDACR_DEVAD));

   //Write MMD register address
   lan8741WritePhyReg(interface, LAN8741_MMDAADR, regAddr);

   //Select data operation
   lan8741WritePhyReg(interface, LAN8741_MMDACR,
      LAN8741_MMDACR_FUNC_DATA_NO_POST_INC | (devAddr & LAN8741_MMDACR_DEVAD));

   //Write the content of the MMD register
   lan8741WritePhyReg(interface, LAN8741_MMDAADR, data);
}


/**
 * @brief Read MMD register
 * @param[in] interface Underlying network interface
 * @param[in] devAddr Device address
 * @param[in] regAddr Register address
 * @return MMD register value
 **/

uint16_t lan8741ReadMmdReg(NetInterface *interface, uint8_t devAddr,
   uint16_t regAddr)
{
   //Select register operation
   lan8741WritePhyReg(interface, LAN8741_MMDACR,
      LAN8741_MMDACR_FUNC_ADDR | (devAddr & LAN8741_MMDACR_DEVAD));

   //Write MMD register address
   lan8741WritePhyReg(interface, LAN8741_MMDAADR, regAddr);

   //Select data operation
   lan8741WritePhyReg(interface, LAN8741_MMDACR,
      LAN8741_MMDACR_FUNC_DATA_NO_POST_INC | (devAddr & LAN8741_MMDACR_DEVAD));

   //Read the content of the MMD register
   return lan8741ReadPhyReg(interface, LAN8741_MMDAADR);
}
