/**
 * @file STM32L4xx_flash_driver.h
 * @brief CycloneBOOT STM32L4xx Flash Driver Template
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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

#ifndef _STM32L4xx_FLASH_DRIVER_H
#define _STM32L4xx_FLASH_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_flash.h"
#include "core/flash.h"
#include "error.h"

//STM32L4xx name
#define STM32L4xx_FLASH_NAME "STM32L4xx Internal Flash"
//STM32L4xx start addr
#define STM32L4xx_FLASH_ADDR FLASH_BASE
//STM32L4xx write size
#define STM32L4xx_FLASH_WRITE_SIZE (4*2) //N-bytes word
//STM32L4xx read size
#define STM32L4xx_FLASH_READ_SIZE 0x4 //32-bytes word

//Device flash size
#if (defined(STM32L47xG) || defined(STM32L48xx) || defined(STM32L49xG) || \
       defined(STM32L4A6xx))
#define STM32L4xx_FLASH_SIZE 0x100000 //1MB flash
#elif (defined(STM32L47xE) || defined(STM32L49xE))
#define STM32L4xx_FLASH_SIZE 0x80000 //512KB flash
#elif (defined(STM32L47xC))
#define STM32L4xx_FLASH_SIZE 0x40000 //256KB flash
#else
#error You MUST define one of the above device part number!
#endif

//Device flash sector size
#define STM32L4xx_FLASH_SECTOR_SIZE 0x800 //2KB sector

//Device flash sector number
#define STM32L4xx_FLASH_SECTOR_NUMBER (STM32L4xx_FLASH_SIZE/STM32L4xx_FLASH_SECTOR_SIZE)

//Device flash bank IDs
#define STM32L4xx_FLASH_BANK1_ID 1
#define STM32L4xx_FLASH_BANK2_ID 2

//Device flash bank info
#if defined(FLASH_DB_MODE)
#define STM32L4xx_FLASH_BANK_SIZE STM32L4xx_FLASH_SIZE/2
#define STM32L4xx_FLASH_BANK1_ADDR FLASH_BASE
#define STM32L4xx_FLASH_BANK2_ADDR FLASH_BASE+STM32L4xx_FLASH_BANK_SIZE
#else
#define STM32L4xx_FLASH_BANK_SIZE STM32L4xx_FLASH_SIZE
#define STM32L4xx_FLASH_BANK1_ADDR FLASH_BASE
#define STM32L4xx_FLASH_BANK2_ADDR FLASH_BASE
#endif

//STM32L4xx Internal Memory Flash driver
extern const FlashDriver stm32l4xxFlashDriver;

#endif //!_STM32L4xx_FLASH_DRIVER_H
