/**
 * @file utils.h
 * @brief Utility functions used throughout the application
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

#ifndef __UTILS_H
#define  __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/crypto.h"
#include "cipher_modes/cbc.h"
#include "cipher/aria.h"
#include "cipher/cipher_algorithms.h"
#include "rng/yarrow.h"
#include "main.h"
#include "header.h"
#include "body.h"

#define ARRAY_BYTES(arr)        (sizeof((arr)[0]) * ARRAY_SIZE(arr))
#define ARRAY_SBYTES(arr)       ((ssize_t)ARRAY_BYTES(arr))

#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))
#define ARRAY_SSIZE(arr)    ((ptrdiff_t)ARRAY_SIZE(arr))

// Magic number used to check cipher key
#define CIPHER_MAGIC_NUMBER "5ef41578fcfbb9a98ffc218dde463d44"
#define CIPHER_MAGIC_NUMBER_SIZE 16

typedef struct {
    ImageHeader *header;
    ImageBody *body;
} UpdateImage;

int read_file(const char *file_path, char **file_contents, size_t *file_size);
int blockify(size_t blockSize, char* input, size_t inputSize, char** output, size_t* outputSize);
int init_crypto(CipherInfo *cipherInfo);
int encrypt(char *plainData, size_t plainDataSize, char* cipherData, CipherInfo cipherInfo);
int sign(CipherInfo *cipherInfo, CheckDataInfo *checkDataInfo, char *data, size_t dataLen, char **signData, size_t *signDataLen);
int write_image_to_file(UpdateImage *image, CipherInfo *cipherInfo, const char *output_file_path);

void dump_buffer(void *buffer, size_t buffer_size);
void dump_buffer_2(void *buffer, size_t buffer_size, size_t columSize);
void dumpHeader(ImageHeader* header);
void dumpBody(ImageBody* body);
void dumpFooter(char *check_data, size_t check_data_size);
void seedInitVector(uint8_t *buffer, size_t length);

int is_hex(const char *str);
int hex_string_to_byte_array(const char* hexString, unsigned char** byteArray, size_t* byteArraySize);

#endif
