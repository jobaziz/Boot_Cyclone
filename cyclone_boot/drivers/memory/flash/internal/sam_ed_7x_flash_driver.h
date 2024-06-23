//
// Created by Dananjaya RAMANAYAKE on 05/09/2023.
//

#ifndef _SAM_ED_7X_DRIVER_H
#define _SAM_ED_7X_DRIVER_H

#include "same70q21b.h"
#include "component/efc.h"
#include "plib_efc.h"
#include "core/flash.h"

//#define EFC ((Efc *)(uint32_t *)EFC_BASE_ADDRESS)

#define EFC_ACCESS_MODE_128    0
#if (!(SAMV71 || SAMV70 || SAMS70 || SAME70))
#define EFC_ACCESS_MODE_64     EEFC_FMR_FAM
#endif

#define MEMORY_LOWER_BOUND 0x00400000u
#define MEMORY_UPPER_BOUND MEMORY_LOWER_BOUND + 0x200000u

const extern FlashDriver same7x_driver;

//SAM_ED_5x Flash name
#define SAM_ED_7x_FLASH_NAME "SAM_ED_7x Internal Flash"
//SAM_ED_5x Flash start address
#define SAM_ED_7x_FLASH_ADDR IFLASH_ADDR
//SAM_ED_5x Flash write size
//#define SAM_ED_7x_FLASH_WRITE_SIZE 4*4 //Quad-word -> 16 bytes
#define SAM_ED_7x_FLASH_WRITE_SIZE 512 //Quad-word -> 16 bytes
//SAM_ED_5x Flash read size
#define SAM_ED_7x_FLASH_READ_SIZE 0x4 //4-bytes word

//Device flash size
#define SAM_ED_7x_FLASH_SIZE IFLASH_SIZE

//Device flash block (page) size (512 Bytes)
#define SAM_ED_7x_FLASH_BLOCK_SIZE IFLASH_PAGE_SIZE
//Device flash sector size
#define SAM_ED_7x_FLASH_SECTOR_SIZE (IFLASH_PAGE_SIZE*256) //128 Kbytes
//Device flash sector number
#define SAM_ED_7x_FLASH_SECTOR_NUMBER (SAM_ED_7x_FLASH_SIZE/SAM_ED_7x_FLASH_SECTOR_SIZE)

#endif //_SAM_ED_7X_DRIVER_H
