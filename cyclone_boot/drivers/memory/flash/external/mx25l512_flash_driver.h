/**
 * @file mx25l512_flash_driver.h
 * @brief CycloneBOOT MX25L512 Flash Driver Template
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

#ifndef _MX25L512_FLASH_DRIVER_H
#define _MX25L512_FLASH_DRIVER_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "core/flash.h"
#include "error.h"

//MX25L512 name
#define MX25L512_NAME "MX25L512 External QPSI Nor Flash"
//MX25L512 start addr
#define MX25L512_ADDR 0x00000000
//MX25L512 write size
#define MX25L512_WRITE_SIZE 0x04 //4-bytes word
//MX25L512 read size
#define MX25L512_READ_SIZE 0x04 //4-bytes word


//MX25L512 size
#define MX25L512_SIZE 0x4000000
//MX25L512 sectors number
#define MX25L512_SECTORS_NUMBER 1024
//MX25L512 Sectors size
#define MX25L512_SECTORS_SIZE 0x10000
//MX25L512 Subsectors 4KB number
#define MX25L512_SUBSECTORS_NUMBER 16384
//MX25L512 Subsectors 4KB size
#define MX25L512_SUBSECTORS_SIZE 0x1000

//MX25L512 Internal Memory Flash driver
extern const FlashDriver mx25l512FlashDriver;

#endif //!_MX25L512_FLASH_DRIVER_H
