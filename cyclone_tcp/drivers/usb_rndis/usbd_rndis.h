/**
 * @file usbd_rndis.h
 * @brief USB RNDIS class
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

#ifndef _USB_RNDIS_H
#define _USB_RNDIS_H

//Dependencies
#include "usbd_ioreq.h"

//USB endpoints
#define RNDIS_NOTIFICATION_EP 0x81
#define RNDIS_DATA_IN_EP      0x82
#define RNDIS_DATA_OUT_EP     0x03

//Endpoint maximum packet size
#define RNDIS_NOTIFICATION_EP_MPS 64
#define RNDIS_DATA_IN_EP_MPS_FS   64
#define RNDIS_DATA_OUT_EP_MPS_FS  64
#define RNDIS_DATA_IN_EP_MPS_HS   512
#define RNDIS_DATA_OUT_EP_MPS_HS  512

//RNDIS Class specific requests
#define RNDIS_SEND_ENCAPSULATED_COMMAND 0x00
#define RNDIS_GET_ENCAPSULATED_RESPONSE 0x01

//Global variables
extern USBD_HandleTypeDef USBD_Device;
extern USBD_ClassTypeDef usbdRndisClass;
#define USBD_RNDIS_CLASS &usbdRndisClass

//RNDIS related functions
uint8_t usbdRndisInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
uint8_t usbdRndisDeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
uint8_t usbdRndisSetup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
uint8_t usbdRndisEp0RxReady(USBD_HandleTypeDef *pdev);
uint8_t usbdRndisDataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t usbdRndisDataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t *usbdRndisGetHighSpeedConfigDesc(uint16_t *length);
uint8_t *usbdRndisGetFullSpeedConfigDesc(uint16_t *length);
uint8_t *usbdRndisGetOtherSpeedConfigDesc(uint16_t *length);
uint8_t *usbdRndisGetDeviceQualifierDesc(uint16_t *length);

#endif
