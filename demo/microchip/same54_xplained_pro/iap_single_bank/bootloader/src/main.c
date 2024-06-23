/**
 * @file main.c
 * @brief Main routine
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
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
 * @version 3.0.4
 **/

//Dependencies
#include <stdlib.h>
#include "sam.h"
#include "same54_xplained_pro.h"
#include "bootloader/boot.h"
#include "drivers/memory/flash/internal/sam_ed_5x_flash_driver.h"
#include "core/cboot_error.h"
#include "boot_config.h"
#include "debug.h"

//Global variables
uint8_t ledState = 0;
systime_t ledStart = 0;
systime_t ledTimeout = 0;

BootSettings bootSettings;
BootContext bootContext;

/**
 * @brief I/O initialization
 **/

void ioInit(void)
{
   //Enable PORT bus clock (CLK_PORT_APB)
   MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_PORT_Msk;

   //Configure LED0
   PORT_REGS->GROUP[LED0_GROUP].PORT_DIRSET = LED0_MASK;
   PORT_REGS->GROUP[LED0_GROUP].PORT_OUTSET = LED0_MASK;

   //Configure SW0 button
   PORT_REGS->GROUP[SW0_GROUP].PORT_DIRCLR = SW0_MASK;
   PORT_REGS->GROUP[SW0_GROUP].PORT_PINCFG[SW0_PIN] |= PORT_PINCFG_INEN_Msk;
}


/**
 * @brief LED task
 * @param[in] param Unused parameter
 **/

void ledTask(void)
{
   if(systemTicks - ledStart >= ledTimeout)
   {
      if(ledState == 0)
      {
         PORT_REGS->GROUP[LED0_GROUP].PORT_OUTCLR = LED0_MASK;
         ledStart = systemTicks;
         ledTimeout = 100;
         ledState = 1;
      }
      else
      {
         PORT_REGS->GROUP[LED0_GROUP].PORT_OUTSET = LED0_MASK;
         ledStart = systemTicks;
         ledTimeout = 900;
         ledState = 0;
      }
   }
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   cboot_error_t cerror;

   //Update system core clock
   SystemCoreClockUpdate();

   //Initialize kernel
   osInitKernel();
   //Configure debug UART
   debugInit(115200);

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("**********************************************\r\n");
   TRACE_INFO("*** CycloneBOOT IAP Single-Bank Bootloader ***\r\n");
   TRACE_INFO("**********************************************\r\n");
   TRACE_INFO("Copyright: 2010-2024 Oryx Embedded SARL\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: SAME54\r\n");
   TRACE_INFO("\r\n");

   //Configure I/Os
   ioInit();

   //Begining of handling exception
   do
   {
      //Get default booloader user settings
      bootGetDefaultSettings(&bootSettings);

      //User settings primary memory configuration
      bootSettings.memories[0].memoryType = MEMORY_TYPE_FLASH;
      bootSettings.memories[0].memoryRole = MEMORY_ROLE_PRIMARY;
      bootSettings.memories[0].driver = &sam_ed_5x_FlashDriver;
      bootSettings.memories[0].nbSlots = 2;
      //User settings primary memory slot 0 configuration
      bootSettings.memories[0].slots[0].type = SLOT_TYPE_DIRECT;
      bootSettings.memories[0].slots[0].cType = SLOT_CONTENT_BINARY;
      bootSettings.memories[0].slots[0].memParent = &bootSettings.memories[0];
      bootSettings.memories[0].slots[0].addr = 0x00020000;
      bootSettings.memories[0].slots[0].size = 0x60000;
      //User settings primary memory slot 1 configuration
      bootSettings.memories[0].slots[1].type = SLOT_TYPE_DIRECT;
      bootSettings.memories[0].slots[1].cType = SLOT_CONTENT_UPDATE;
      bootSettings.memories[0].slots[1].memParent = &bootSettings.memories[0];
      bootSettings.memories[0].slots[1].addr = 0x00080000;
      bootSettings.memories[0].slots[1].size = 0x60000;

      //Initialize bootloader
      cerror = bootInit(&bootContext, &bootSettings);
      //Is any error?
      if(cerror)
         break;
   }while(0);

   //Is any error?
   if(cerror)
   {
      //Debug message
      TRACE_ERROR("Bootloader configuration failed!\r\n");
      while(1);
   }

   while(1)
   {
      ledTask();
      cerror = bootTask(&bootContext);
      //Is any error?
      if(cerror)
      {
         //Debug message
         TRACE_ERROR("Bootloader failure!\r\n");
         break;
      }
   }

   //This function should never return
   return 0;
}
