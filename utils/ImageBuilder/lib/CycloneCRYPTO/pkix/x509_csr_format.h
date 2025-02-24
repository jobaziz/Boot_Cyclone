/**
 * @file x509_csr_format.h
 * @brief CSR (Certificate Signing Request) formatting
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

#ifndef _X509_CSR_FORMAT_H
#define _X509_CSR_FORMAT_H

//Dependencies
#include "core/crypto.h"
#include "pkix/x509_common.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//CSR related functions
error_t x509FormatCertRequestInfo(const X509CertRequestInfo *certReqInfo,
   const void *publicKey, uint8_t *output, size_t *written);

error_t x509FormatAttributes(const X509Attributes *attributes,
   uint8_t *output, size_t *written);

error_t x509FormatChallengePassword(const X509ChallengePassword *challengePwd,
   uint8_t *output, size_t *written);

error_t x509FormatExtensionRequest(const X509Extensions *extensionReq,
   uint8_t *output, size_t *written);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
