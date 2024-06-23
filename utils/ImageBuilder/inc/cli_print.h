//
// Created by Dananjaya RAMANAYAKE on 30/01/2024.
//

#ifndef _CLI_PRINT_H
#define _CLI_PRINT_H

#include "cargs.h"

static struct cag_option options_print[] = {
        {.identifier = 'i',
                .access_letters = "i",
                .access_name = "input",
                .value_name = "<firmware.bin>",
                .description = "[REQUIRED] Path to firmware binary"},

        {.identifier = 'o',
                .access_letters = "o",
                .access_name = "output",
                .value_name = "<firmware_update.img>",
                .description = "[REQUIRED] Path to save firmware update image"},

        {.identifier = 'f',
                .access_letters = NULL,
                .access_name = "firmware-version",
                .value_name = "<x.x.x>",
                .description = "[OPTIONAL] Firmware version, mandatory for anti-rollback support"},

        {.identifier = 'p',
                .access_letters = NULL,
                .access_name = "vtor-align",
                .value_name = NULL,
                .description = "[OPTIONAL] Set the vector table alignment when necessary"},

        {.identifier = 'e',
                .access_letters = NULL,
                .access_name = "enc-algo",
                .value_name = "<aes-cbc>",
                .description = "[OPTIONAL] Encryption algorithm to be used if encryption is required"},

        {.identifier = 'k',
                .access_letters = NULL,
                .access_name = "enc-key",
                .value_name = "<encryption_key>",
                .description = "[OPTIONAL] Encryption Key if encryption is required"},

        {.identifier = 'n',
                .access_letters = NULL,
                .access_name = "integrity-algo",
                .value_name = "<crc32|md5|sha1|sha224|sha384|sha256|sha512>",
                .description = "[OPTIONAL] Integrity Algorithm to be used if integrity check is chosen"},

        {.identifier = 'a',
                .access_letters = NULL,
                .access_name = "auth-algo",
                .value_name = "<hmac-md5|hmac-sha256|hmac-sha512|>",
                .description = "[OPTIONAL] Authentication Algorithm to be used if authentication is chosen"},

        {.identifier = 'u',
                .access_letters = NULL,
                .access_name = "auth-key",
                .value_name = "<auth_key>",
                .description = "[OPTIONAL] Authentication Key if authentication is chosen"},

        {.identifier = 's',
                .access_letters = NULL,
                .access_name = "sign-algo",
                .value_name = "<ecdsa-sha256|rsa-sha256>",
                .description = "[OPTIONAL] Signature Algorithm to be used if signature is chosen"},

        {.identifier = 'g',
                .access_letters = NULL,
                .access_name = "sign-key",
                .value_name = "<sign_key.pem>",
                .description = "[OPTIONAL] Private Signature Key if signature is chosen"},

        {.identifier = 'b',
                .access_letters = NULL,
                .access_name = "verbose",
                .value_name = NULL,
                .description = "[OPTIONAL] Enable Verbose mode"},


        {.identifier = 'v',
                .access_letters = "v",
                .access_name = "version",
                .value_name = NULL,
                .description = "[OPTIONAL] Display ImageBuilder CLI version"},

        {.identifier = 'h',
                .access_letters = "h",
                .access_name = "help",
                .description = "[OPTIONAL] Display the help message"}
};

#endif //_CLI_PRINT_H
