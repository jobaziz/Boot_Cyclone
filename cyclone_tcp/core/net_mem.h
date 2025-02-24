/**
 * @file net_mem.h
 * @brief Memory management
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

#ifndef _NET_MEM_H
#define _NET_MEM_H

//Dependencies
#include "net_config.h"
#include "os_port.h"
#include "error.h"

//Use fixed-size blocks allocation?
#ifndef NET_MEM_POOL_SUPPORT
   #define NET_MEM_POOL_SUPPORT DISABLED
#elif (NET_MEM_POOL_SUPPORT != ENABLED && NET_MEM_POOL_SUPPORT != DISABLED)
   #error NET_MEM_POOL_SUPPORT parameter is not valid
#endif

//Number of buffers available
#ifndef NET_MEM_POOL_BUFFER_COUNT
   #define NET_MEM_POOL_BUFFER_COUNT 32
#elif (NET_MEM_POOL_BUFFER_COUNT < 1)
   #error NET_MEM_POOL_BUFFER_COUNT parameter is not valid
#endif

//Size of the buffers
#ifndef NET_MEM_POOL_BUFFER_SIZE
   #define NET_MEM_POOL_BUFFER_SIZE 1536
#elif (NET_MEM_POOL_BUFFER_SIZE < 128)
   #error NET_MEM_POOL_BUFFER_SIZE parameter is not valid
#endif

//Size of the header part of the buffer
#define CHUNKED_BUFFER_HEADER_SIZE (sizeof(NetBuffer) + MAX_CHUNK_COUNT * sizeof(ChunkDesc))

//Helper macro for defining a buffer
#define N(size) (((size) + NET_MEM_POOL_BUFFER_SIZE - 1) / NET_MEM_POOL_BUFFER_SIZE)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Structure describing a chunk of data
 **/

typedef struct
{
   void *address;
   uint16_t length;
   uint16_t size;
} ChunkDesc;


/**
 * @brief Structure describing a buffer that spans multiple chunks
 **/

typedef struct
{
   uint_t chunkCount;
   uint_t maxChunkCount;
   ChunkDesc chunk[];
} NetBuffer;


typedef struct
{
   uint_t chunkCount;
   uint_t maxChunkCount;
   ChunkDesc chunk[1];
} NetBuffer1;


//Memory management functions
error_t memPoolInit(void);
void *memPoolAlloc(size_t size);
void memPoolFree(void *p);
void memPoolGetStats(uint_t *currentUsage, uint_t *maxUsage, uint_t *size);

NetBuffer *netBufferAlloc(size_t length);
void netBufferFree(NetBuffer *buffer);

size_t netBufferGetLength(const NetBuffer *buffer);
error_t netBufferSetLength(NetBuffer *buffer, size_t length);

void *netBufferAt(const NetBuffer *buffer, size_t offset);

error_t netBufferConcat(NetBuffer *dest,
   const NetBuffer *src, size_t srcOffset, size_t length);

error_t netBufferCopy(NetBuffer *dest, size_t destOffset,
   const NetBuffer *src, size_t srcOffset, size_t length);

error_t netBufferAppend(NetBuffer *dest, const void *src, size_t length);

size_t netBufferWrite(NetBuffer *dest,
   size_t destOffset, const void *src, size_t length);

size_t netBufferRead(void *dest, const NetBuffer *src,
   size_t srcOffset, size_t length);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
