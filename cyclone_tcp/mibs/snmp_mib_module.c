/**
 * @file snmp_mib_module.c
 * @brief SNMP MIB module
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
#define TRACE_LEVEL SNMP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "mibs/mib_common.h"
#include "mibs/snmp_mib_module.h"
#include "mibs/snmp_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SNMP_MIB_SUPPORT == ENABLED)


/**
 * @brief SNMP MIB base
 **/

SnmpMibBase snmpMibBase;


/**
 * @brief SNMP MIB objects
 **/

const MibObject snmpMibObjects[] =
{
   //sysDescr object (1.3.6.1.2.1.1.1)
   {
      "sysDescr",
      {43, 6, 1, 2, 1, 1, 1},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_ONLY,
#if (SNMP_MIB_SYS_DESCR_SIZE > 0)
      &snmpMibBase.sysGroup.sysDescr,
      &snmpMibBase.sysGroup.sysDescrLen,
      SNMP_MIB_SYS_DESCR_SIZE,
      NULL,
      NULL,
      NULL
#else
      NULL,
      NULL,
      0,
      NULL,
      snmpMibGetSysDescr,
      NULL
#endif
   },
   //sysObjectID object (1.3.6.1.2.1.1.2)
   {
      "sysObjectID",
      {43, 6, 1, 2, 1, 1, 2},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OBJECT_IDENTIFIER,
      MIB_ACCESS_READ_ONLY,
#if (SNMP_MIB_SYS_OBJECT_ID_SIZE > 0)
      &snmpMibBase.sysGroup.sysObjectID,
      &snmpMibBase.sysGroup.sysObjectIDLen,
      SNMP_MIB_SYS_OBJECT_ID_SIZE,
      NULL,
      NULL,
      NULL
#else
      NULL,
      NULL,
      0,
      NULL,
      snmpMibGetSysObjectID,
      NULL
#endif
   },
   //sysUpTime object (1.3.6.1.2.1.1.3)
   {
      "sysUpTime",
      {43, 6, 1, 2, 1, 1, 3},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_TIME_TICKS,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(uint32_t),
      NULL,
      snmpMibGetSysUpTime,
      NULL
   },
   //sysContact object (1.3.6.1.2.1.1.4)
   {
      "sysContact",
      {43, 6, 1, 2, 1, 1, 4},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_WRITE,
#if (SNMP_MIB_SYS_CONTACT_SIZE > 0)
      &snmpMibBase.sysGroup.sysContact,
      &snmpMibBase.sysGroup.sysContactLen,
      SNMP_MIB_SYS_CONTACT_SIZE,
      NULL,
      NULL,
      NULL
#else
      NULL,
      NULL,
      0,
      snmpMibSetSysContact,
      snmpMibGetSysContact,
      NULL
#endif
   },
   //sysName object (1.3.6.1.2.1.1.5)
   {
      "sysName",
      {43, 6, 1, 2, 1, 1, 5},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_WRITE,
#if (SNMP_MIB_SYS_NAME_SIZE > 0)
      &snmpMibBase.sysGroup.sysName,
      &snmpMibBase.sysGroup.sysNameLen,
      SNMP_MIB_SYS_NAME_SIZE,
      NULL,
      NULL,
      NULL
#else
      NULL,
      NULL,
      0,
      snmpMibSetSysName,
      snmpMibGetSysName,
      NULL
#endif
   },
   //sysLocation object (1.3.6.1.2.1.1.6)
   {
      "sysLocation",
      {43, 6, 1, 2, 1, 1, 6},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_WRITE,
#if (SNMP_MIB_SYS_LOCATION_SIZE > 0)
      &snmpMibBase.sysGroup.sysLocation,
      &snmpMibBase.sysGroup.sysLocationLen,
      SNMP_MIB_SYS_LOCATION_SIZE,
      NULL,
      NULL,
      NULL
#else
      NULL,
      NULL,
      0,
      snmpMibSetSysLocation,
      snmpMibGetSysLocation,
      NULL
#endif
   },
   //sysServices object (1.3.6.1.2.1.1.7)
   {
      "sysServices",
      {43, 6, 1, 2, 1, 1, 7},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.sysGroup.sysServices,
      NULL,
      sizeof(int32_t),
      NULL,
      NULL,
      NULL
   },
   //sysORLastChange object (1.3.6.1.2.1.1.8)
   {
      "sysORLastChange",
      {43, 6, 1, 2, 1, 1, 8},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_TIME_TICKS,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.sysGroup.sysORLastChange,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //sysORID object (1.3.6.1.2.1.1.9.1.2)
   {
      "sysORID",
      {43, 6, 1, 2, 1, 1, 9, 1, 2},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OBJECT_IDENTIFIER,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      0,
      NULL,
      snmpMibGetSysOREntry,
      snmpMibGetNextSysOREntry
   },
   //sysORDescr object (1.3.6.1.2.1.1.9.1.3)
   {
      "sysORDescr",
      {43, 6, 1, 2, 1, 1, 9, 1, 3},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OCTET_STRING,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      0,
      NULL,
      snmpMibGetSysOREntry,
      snmpMibGetNextSysOREntry
   },
   //sysORUpTime object (1.3.6.1.2.1.1.9.1.4)
   {
      "sysORUpTime",
      {43, 6, 1, 2, 1, 1, 9, 1, 4},
      9,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_TIME_TICKS,
      MIB_ACCESS_READ_ONLY,
      NULL,
      NULL,
      sizeof(uint32_t),
      NULL,
      snmpMibGetSysOREntry,
      snmpMibGetNextSysOREntry
   },
   //snmpInPkts object (1.3.6.1.2.1.11.1)
   {
      "snmpInPkts",
      {43, 6, 1, 2, 1, 11, 1},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpInPkts,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpInBadVersions object (1.3.6.1.2.1.11.3)
   {
      "snmpInBadVersions",
      {43, 6, 1, 2, 1, 11, 3},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpInBadVersions,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpInBadCommunityNames object (1.3.6.1.2.1.11.4)
   {
      "snmpInBadCommunityNames",
      {43, 6, 1, 2, 1, 11, 4},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpInBadCommunityNames,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpInBadCommunityUses object (1.3.6.1.2.1.11.5)
   {
      "snmpInBadCommunityUses",
      {43, 6, 1, 2, 1, 11, 5},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpInBadCommunityUses,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpInASNParseErrs object (1.3.6.1.2.1.11.6)
   {
      "snmpInASNParseErrs",
      {43, 6, 1, 2, 1, 11, 6},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpInASNParseErrs,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpEnableAuthenTraps object (1.3.6.1.2.1.11.30)
   {
      "snmpEnableAuthenTraps",
      {43, 6, 1, 2, 1, 11, 30},
      7,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_WRITE,
      &snmpMibBase.snmpGroup.snmpEnableAuthenTraps,
      NULL,
      sizeof(int32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpSilentDrops object (1.3.6.1.2.1.11.31)
   {
      "snmpSilentDrops",
      {43, 6, 1, 2, 1, 11, 31},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpSilentDrops,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpProxyDrops object (1.3.6.1.2.1.11.32)
   {
      "snmpProxyDrops",
      {43, 6, 1, 2, 1, 11, 32},
      7,
      ASN1_CLASS_APPLICATION,
      MIB_TYPE_COUNTER32,
      MIB_ACCESS_READ_ONLY,
      &snmpMibBase.snmpGroup.snmpProxyDrops,
      NULL,
      sizeof(uint32_t),
      NULL,
      NULL,
      NULL
   },
   //snmpTrapOID object (1.3.6.1.6.3.1.1.4.1)
   {
      "snmpTrapOID",
      {43, 6, 1, 6, 3, 1, 1, 4, 1},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OBJECT_IDENTIFIER,
      MIB_ACCESS_FOR_NOTIFY,
      NULL,
      NULL,
      0,
      NULL,
      snmpv2MibGetSnmpTrapOID,
      NULL
   },
   //snmpTrapEnterprise object (1.3.6.1.6.3.1.1.4.3)
   {
      "snmpTrapEnterprise",
      {43, 6, 1, 6, 3, 1, 1, 4, 3},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_OBJECT_IDENTIFIER,
      MIB_ACCESS_FOR_NOTIFY,
      NULL,
      NULL,
      0,
      NULL,
      snmpv2MibGetSnmpTrapEnterprise,
      NULL
   },
   //snmpSetSerialNo object (1.3.6.1.6.3.1.1.6.1)
   {
      "snmpSetSerialNo",
      {43, 6, 1, 6, 3, 1, 1, 6, 1},
      9,
      ASN1_CLASS_UNIVERSAL,
      ASN1_TYPE_INTEGER,
      MIB_ACCESS_READ_WRITE,
      NULL,
      NULL,
      sizeof(int32_t),
      snmpMibSetSnmpSetSerialNo,
      snmpMibGetSnmpSetSerialNo,
      NULL
   }
};


/**
 * @brief SNMP MIB module
 **/

const MibModule snmpMibModule =
{
   "SNMPv2-MIB",
   {43, 6, 1, 6, 3, 1},
   6,
   snmpMibObjects,
   arraysize(snmpMibObjects),
   snmpMibInit,
   snmpMibLoad,
   snmpMibUnload,
   snmpMibLock,
   snmpMibUnlock
};

#endif
