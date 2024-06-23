/**
 * @file vsc8662_driver.c
 * @brief VSC8662 Gigabit Ethernet PHY driver
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
#include "drivers/phy/vsc8662_driver.h"
#include "debug.h"


/**
 * @brief VSC8662 Ethernet PHY driver
 **/

const PhyDriver vsc8662PhyDriver =
{
   vsc8662Init,
   vsc8662Tick,
   vsc8662EnableIrq,
   vsc8662DisableIrq,
   vsc8662EventHandler
};


/**
 * @brief VSC8662 PHY transceiver initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t vsc8662Init(NetInterface *interface)
{
   //Debug message
   TRACE_INFO("Initializing VSC8662...\r\n");

   //Undefined PHY address?
   if(interface->phyAddr >= 32)
   {
      //Use the default address
      interface->phyAddr = VSC8662_PHY_ADDR;
   }

   //Initialize serial management interface
   if(interface->smiDriver != NULL)
   {
      interface->smiDriver->init();
   }

   //Reset PHY transceiver
   vsc8662WritePhyReg(interface, VSC8662_BMCR, VSC8662_BMCR_RESET);

   //Wait for the reset to complete
   while(vsc8662ReadPhyReg(interface, VSC8662_BMCR) & VSC8662_BMCR_RESET)
   {
   }

   //Dump PHY registers for debugging purpose
   vsc8662DumpPhyReg(interface);

   //Perform custom configuration
   vsc8662InitHook(interface);

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief VSC8662 custom configuration
 * @param[in] interface Underlying network interface
 **/

__weak_func void vsc8662InitHook(NetInterface *interface)
{
}


/**
 * @brief VSC8662 timer handler
 * @param[in] interface Underlying network interface
 **/

void vsc8662Tick(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //Read basic status register
   value = vsc8662ReadPhyReg(interface, VSC8662_BMSR);
   //Retrieve current link state
   linkState = (value & VSC8662_BMSR_LINK_STATUS) ? TRUE : FALSE;

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


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void vsc8662EnableIrq(NetInterface *interface)
{
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void vsc8662DisableIrq(NetInterface *interface)
{
}


/**
 * @brief VSC8662 event handler
 * @param[in] interface Underlying network interface
 **/

void vsc8662EventHandler(NetInterface *interface)
{
   uint16_t status;

   //Read basic status register
   status = vsc8662ReadPhyReg(interface, VSC8662_BMSR);

   //Link is up?
   if((status & VSC8662_BMSR_LINK_STATUS) != 0)
   {
      //Read auxiliary control and status register
      status = vsc8662ReadPhyReg(interface, VSC8662_AUX_CTRL_STAT);

      //Check current speed
      switch(status & VSC8662_AUX_CTRL_STAT_SPEED_STATUS)
      {
      //10BASE-T
      case VSC8662_AUX_CTRL_STAT_SPEED_STATUS_10:
         interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         break;
      //100BASE-TX
      case VSC8662_AUX_CTRL_STAT_SPEED_STATUS_100:
         interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
         break;
      //1000BASE-T
      case VSC8662_AUX_CTRL_STAT_SPEED_STATUS_1000:
         interface->linkSpeed = NIC_LINK_SPEED_1GBPS;
         break;
      //Unknown speed
      default:
         //Debug message
         TRACE_WARNING("Invalid speed\r\n");
         break;
      }

      //Check current duplex mode
      if((status & VSC8662_AUX_CTRL_STAT_FDX_STATUS) != 0)
      {
         interface->duplexMode = NIC_FULL_DUPLEX_MODE;
      }
      else
      {
         interface->duplexMode = NIC_HALF_DUPLEX_MODE;
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


/**
 * @brief Write PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @param[in] data Register value
 **/

void vsc8662WritePhyReg(NetInterface *interface, uint8_t address,
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

uint16_t vsc8662ReadPhyReg(NetInterface *interface, uint8_t address)
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

void vsc8662DumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i,
         vsc8662ReadPhyReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}
