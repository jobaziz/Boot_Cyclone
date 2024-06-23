/**
 * @file dp83tc812_driver.c
 * @brief DP83TC812 100Base-T1 Ethernet PHY driver
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
#include "drivers/phy/dp83tc812_driver.h"
#include "debug.h"


/**
 * @brief DP83TC812 Ethernet PHY driver
 **/

const PhyDriver dp83tc812PhyDriver =
{
   dp83tc812Init,
   dp83tc812Tick,
   dp83tc812EnableIrq,
   dp83tc812DisableIrq,
   dp83tc812EventHandler
};


/**
 * @brief DP83TC812 PHY transceiver initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t dp83tc812Init(NetInterface *interface)
{
   //Debug message
   TRACE_INFO("Initializing DP83TC812...\r\n");

   //Undefined PHY address?
   if(interface->phyAddr >= 32)
   {
      //Use the default address
      interface->phyAddr = DP83TC812_PHY_ADDR;
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

   //Reset PHY transceiver
   dp83tc812WritePhyReg(interface, DP83TC812_BMCR, DP83TC812_BMCR_RESET);

   //Wait for the reset to complete
   while(dp83tc812ReadPhyReg(interface, DP83TC812_BMCR) & DP83TC812_BMCR_RESET)
   {
   }

   //Dump PHY registers for debugging purpose
   dp83tc812DumpPhyReg(interface);

   //Perform custom configuration
   dp83tc812InitHook(interface);

   //Force the TCP/IP stack to poll the link state at startup
   interface->phyEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief DP83TC812 custom configuration
 * @param[in] interface Underlying network interface
 **/

__weak_func void dp83tc812InitHook(NetInterface *interface)
{
}


/**
 * @brief DP83TC812 timer handler
 * @param[in] interface Underlying network interface
 **/

void dp83tc812Tick(NetInterface *interface)
{
   uint16_t value;
   bool_t linkState;

   //No external interrupt line driver?
   if(interface->extIntDriver == NULL)
   {
      //Read status register
      value = dp83tc812ReadPhyReg(interface, DP83TC812_BMSR);
      //Retrieve current link state
      linkState = (value & DP83TC812_BMSR_LINK_STATUS) ? TRUE : FALSE;

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

void dp83tc812EnableIrq(NetInterface *interface)
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

void dp83tc812DisableIrq(NetInterface *interface)
{
   //Disable PHY transceiver interrupts
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->disableIrq();
   }
}


/**
 * @brief DP83TC812 event handler
 * @param[in] interface Underlying network interface
 **/

void dp83tc812EventHandler(NetInterface *interface)
{
   uint16_t value;

   //Read status register
   value = dp83tc812ReadPhyReg(interface, DP83TC812_BMSR);

   //Link is up?
   if((value & DP83TC812_BMSR_LINK_STATUS) != 0)
   {
      //The PHY is only able to operate in 100 Mbps mode
      interface->linkSpeed = NIC_LINK_SPEED_100MBPS;
      interface->duplexMode = NIC_FULL_DUPLEX_MODE;

      //Adjust MAC configuration parameters for proper operation
      interface->nicDriver->updateMacConfig(interface);

      //Update link state
      interface->linkState = TRUE;
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

void dp83tc812WritePhyReg(NetInterface *interface, uint8_t address,
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

uint16_t dp83tc812ReadPhyReg(NetInterface *interface, uint8_t address)
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

void dp83tc812DumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i,
         dp83tc812ReadPhyReg(interface, i));
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

void dp83tc812WriteMmdReg(NetInterface *interface, uint8_t devAddr,
   uint16_t regAddr, uint16_t data)
{
   //Select register operation
   dp83tc812WritePhyReg(interface, DP83TC812_REGCR,
      DP83TC812_REGCR_CMD_ADDR | (devAddr & DP83TC812_REGCR_DEVAD));

   //Write MMD register address
   dp83tc812WritePhyReg(interface, DP83TC812_ADDAR, regAddr);

   //Select data operation
   dp83tc812WritePhyReg(interface, DP83TC812_REGCR,
      DP83TC812_REGCR_CMD_DATA_NO_POST_INC | (devAddr & DP83TC812_REGCR_DEVAD));

   //Write the content of the MMD register
   dp83tc812WritePhyReg(interface, DP83TC812_ADDAR, data);
}


/**
 * @brief Read MMD register
 * @param[in] interface Underlying network interface
 * @param[in] devAddr Device address
 * @param[in] regAddr Register address
 * @return MMD register value
 **/

uint16_t dp83tc812ReadMmdReg(NetInterface *interface, uint8_t devAddr,
   uint16_t regAddr)
{
   //Select register operation
   dp83tc812WritePhyReg(interface, DP83TC812_REGCR,
      DP83TC812_REGCR_CMD_ADDR | (devAddr & DP83TC812_REGCR_DEVAD));

   //Write MMD register address
   dp83tc812WritePhyReg(interface, DP83TC812_ADDAR, regAddr);

   //Select data operation
   dp83tc812WritePhyReg(interface, DP83TC812_REGCR,
      DP83TC812_REGCR_CMD_DATA_NO_POST_INC | (devAddr & DP83TC812_REGCR_DEVAD));

   //Read the content of the MMD register
   return dp83tc812ReadPhyReg(interface, DP83TC812_ADDAR);
}
