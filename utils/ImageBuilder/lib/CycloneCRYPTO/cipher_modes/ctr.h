/**
 * @file ctr.h
 * @brief Counter(CTR) mode
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

#ifndef _CTR_H
#define _CTR_H

//Dependencies
#include "core/crypto.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CTR encryption and decryption routines
error_t ctrEncrypt(const CipherAlgo *cipher, void *context, uint_t m,
   uint8_t *t, const uint8_t *p, uint8_t *c, size_t length);

error_t ctrDecrypt(const CipherAlgo *cipher, void *context, uint_t m,
   uint8_t *t, const uint8_t *c, uint8_t *p, size_t length);

void ctrIncBlock(uint8_t *ctr, uint32_t inc, size_t blockSize, size_t m);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
