/**
 * @file cli.c
 * @brief Front-end to the CLI to fetch user parameters
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


#include <stdio.h>
#include "inc/cli.h"
#include "ImageBuilderConfig.h"

// Function to make sure that crypto settings provided by the user are correct
int check_constraints_encryption(const char *encryption_algo, const uint8_t *encryption_key, size_t encryption_key_len) {

#ifdef IS_LINUX
    // Make sure encryption algo is nothing else but AES-CBC
    if (encryption_algo == NULL || strcasecmp(encryption_algo, "aes-cbc") != 0) {
        printf("\nError: Unknown encryption algorithm. Supported algorithms: aes-cbc.\n");
        return EXIT_FAILURE;
    }
#endif
#ifdef IS_WINDOWS
    // Make sure encryption algo is nothing else but AES-CBC
    if (encryption_algo == NULL || strnicmp(encryption_algo, "aes-cbc",7) != 0)
    {
        printf("\nError: Unknown encryption algorithm. Supported algorithms: aes-cbc.\n");
        return EXIT_FAILURE;
    }
#endif

    if (encryption_key == NULL) {
        printf("\nError: Missing encryption key.\n");
        return EXIT_FAILURE;
    }

    // Make sure that the encryption key is at least 64 bytes
    const size_t n = encryption_key_len;

    if ((n != 16) && (n != 24) && (n != 32)) {
        printf("\nError: Invalid encryption key size (AES-256 requires a 256-bit key).\n");
        return EXIT_FAILURE;
    }

    // Make sure that the pair algo / key is both present if one is chosen
    if (encryption_algo && !encryption_key) {
        printf("\nError: Please specify an encryption key.");
        return EXIT_FAILURE;
    } else if (!encryption_algo && encryption_key) {
        printf("\nError: Please specify an encryption algorithm. Supported algorithms: aes-cbc.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Utility message
void display_supported_auth_algorithms_message(void) {
    printf("Supported algorithms: hmac-md5, hmac-sha256, hmac-sha512\n");
}

// Function to make sure that authentication settings provided by the user are correct
int check_constraints_authentication(const char *authentication_algo, const char *authentication_key) {

    if (authentication_algo == NULL || authentication_key == NULL) {
        printf("Error: Missing authentication algorithm or key.\r\n");
        return EXIT_FAILURE;
    }

#ifdef IS_LINUX
    if (strcasecmp(authentication_algo, "hmac-md5") != 0 &&
        strcasecmp(authentication_algo, "hmac-sha256") != 0 &&
        strcasecmp(authentication_algo, "hmac-sha512") != 0) {
        printf("Invalid Authentication algorithm. ");
        display_supported_auth_algorithms_message();
        return EXIT_FAILURE;
    }
#endif
#ifdef IS_WINDOWS
    if (strnicmp(authentication_algo, "hmac-md5",8) != 0 &&
        strnicmp(authentication_algo, "hmac-sha256",11) != 0 &&
        strnicmp(authentication_algo, "hmac-sha512",11) != 0)
    {
        printf("Invalid Authentication algorithm. ");
        display_supported_auth_algorithms_message();
        return EXIT_FAILURE;
    }
#endif

    // Make sure that the pair algo / key is both present if one is chosen
    if (authentication_algo && !authentication_key) {
        printf("\nError: Please specify an authentication key.");
        return EXIT_FAILURE;
    } else if (!authentication_algo && authentication_key) {
        display_supported_auth_algorithms_message();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Function to make sure that signature settings provided by the user are correct
int check_constraints_signature(const char *signature_algo, const char *signature_key) {
    if (signature_algo == NULL || signature_key == NULL) {
        printf("Error: Missing signature algorithm or key.\r\n");
        printf("Supported algorithms: ecdsa-sha256, rsa-sha256\n");
        return EXIT_FAILURE;
    }
#ifdef IS_LINUX
    if (strcasecmp(signature_algo, "ecdsa-sha256") != 0 &&
        strcasecmp(signature_algo, "rsa-sha256") != 0) {
        printf("Invalid Signature algorithm. ");
        printf("Supported algorithms: ecdsa-sha256, rsa-sha256\n");
        return EXIT_FAILURE;
    }
#endif
#ifdef IS_WINDOWS
    if (strnicmp(signature_algo, "ecdsa-sha256",12) != 0 &&
        strnicmp(signature_algo, "rsa-sha256",10) != 0)
    {
        printf("Invalid Signature algorithm. ");
        printf("Supported algorithms: ecdsa-sha256, rsa-sha256\n");
        return EXIT_FAILURE;
    }
#endif
    return EXIT_SUCCESS;
}

void print_cli_help_message() {
    char *exe_name;

    printf("Usage: image_builder [OPTION]...\n");
    printf("Generates a firmware update image compatible with CycloneBOOT.\n");
    printf("CLI Tool Version: %d.%d.%d.%s\n\n",
           image_builder_VERSION_MAJOR, image_builder_VERSION_MINOR, image_builder_VERSION_PATCH, image_builder_TIMESTAMP);

#ifdef IS_WINDOWS
    exe_name = "image_builder_windows.exe";
    printf("Windows Usage: %s.exe [options] <arguments>\n",exe_name);
#endif

#ifdef IS_LINUX
    exe_name = "image_builder_linux";
    printf("Linux Usage: %s [options] <arguments>\n",exe_name);
#endif

    cag_option_print(printable_options, CAG_ARRAY_SIZE(printable_options), stdout);

    printf("\nNote: You must choose one of the three available verification methods (integrity check,authentication, "
           "or signature).\n\n");

    printf("Examples:\n\n");

    printf("Create a clear-text firmware update image with CRC32 integrity algorithm:\n"
           "./%s -i <firmware.bin> -o <firmware_update.img> --integrity-algo\n"
           "crc32\n\n",exe_name);
    printf("Create a cipher-text firmware update image with HMAC-MD5 authentication algorithm:\n"
           "./%s -i <firmware.bin> -o <firmware_update.img> --enc-algo aes-cbc\n"
           "--enc-key-ascii aa3ff7d43cc015682c7dfd00de9379e7 --auth-algo hmac-md5 --auth-key-ascii\n"
           "107b8d4bf62d1c9e26968e8ff51a74df\n\n",exe_name);

    printf("Create a cipher-text firmware update image with RSA-SHA256 signature algorithm:\n"
           "./%s -i <firmware.bin> -o <firmware_update.img> --enc-algo aes-cbc\n"
           "--enc-key-ascii aa3ff7d43cc015682c7dfd00de9379e7--sign-algo rsa-sha256 --sign-key\n"
           "../resources/keys/rsa_private_key.pem\n\n",exe_name);

}

/**
 * This is the main configuration of all CLI options available.
 */
static struct cag_option options[] = {

        {.identifier = 'i',
                .access_letters = "iI",
                .access_name = "input",
                .value_name = "<my_firmware.bin>",
                .description = "[REQUIRED] Path to firmware binary."},

        {.identifier = 'o',
                .access_letters = "oO",
                .access_name = "output",
                .value_name = "<my_firmware_update.img>",
                .description = "[REQUIRED] Path to save firmware update image."},

        {.identifier = 'x',
                .access_letters = "xX",
                .access_name = "image-index",
                .value_name = "<numeric value>",
                .description = "[OPTIONAL] Custom update image index. Default value: 0"},

        {.identifier = 'p',
                .access_letters = NULL,
                .access_name = "vtor-align",
                .value_name = "<number of bytes>",
                .description = "[OPTIONAL] Padding before binary for vector table offset"},

        {.identifier = 'f',
                .access_letters = NULL,
                .access_name = "firmware-version",
                .value_name = "X.X.X",
                .description = "[OPTIONAL] Version of firmware update. Obligatory for fallback support."},

        {.identifier = 'e',
                .access_letters = NULL,
                .access_name = "enc-algo",
                .value_name = "<AES-CBC>",
                .description = "[OPTIONAL] Encryption algorithm used. Supported algorithms: aes-cbc."},

        {.identifier = 'k',
                .access_letters = NULL,
                .access_name = "enc-key-ascii",
                .value_name = "<my_encryption_key>",
                .description = "[OPTIONAL] Encryption Key (in ASCII). Optional unless encryption is required."},

        {.identifier = 'd',
                .access_letters = NULL,
                .access_name = "enc-key-hex",
                .value_name = "<my_encryption_key_in_hex>",
                .description = "[OPTIONAL] Encryption Key (in Hexadecimal). Optional unless encryption is required."},

        {.identifier = 'a',
                .access_letters = NULL,
                .access_name = "auth-algo",
                .value_name = "<HMAC-|MD5|SHA256|SHA512|>",
                .description = "[OPTIONAL] Authentication algorithm used. Supported algorithms: hmac-md5, hmac-sha256, hmac-sha512."},

        {.identifier = 'u',
                .access_letters = NULL,
                .access_name = "auth-key-ascii",
                .value_name = "<my_auth_key>",
                .description = "[OPTIONAL] Authentication Key (in ASCII). Optional unless authentication is required."},

        {.identifier = 'j',
                .access_letters = NULL,
                .access_name = "auth-key-hex",
                .value_name = "<my_auth_key_in_hex>",
                .description = "[OPTIONAL] Authentication Key (in Hexadecimal). Optional unless authentication is required."},

        {.identifier = 's',
                .access_letters = NULL,
                .access_name = "sign-algo",
                .value_name = "<ECDSA-SHA256|RSA-SHA256>",
                .description = "[OPTIONAL] Signature algorithm used. Supported algorithms: ecdsa-sha256, rsa-sha256."},

        {.identifier = 'g',
                .access_letters = NULL,
                .access_name = "sign-key",
                .value_name = "<my_sign_key.pem>",
                .description = "[OPTIONAL] Signature Key. Optional unless signature is required."},

        {.identifier = 'n',
                .access_letters = NULL,
                .access_name = "integrity-algo",
                .value_name = "<CRC32|MD5|SHA1|SHA224|SHA384|SHA256|SHA512>",
                .description = "[OPTIONAL] Integrity algorithm used. CRC32 by default."},

        {.identifier = 'b',
                .access_letters = NULL,
                .access_name = "verbose",
                .value_name = NULL,
                .description = "[OPTIONAL] Verbose mode."},

        {.identifier = 'v',
                .access_letters = "vV",
                .access_name = "version",
                .value_name = NULL,
                .description = "Show CLI version."},

        {.identifier = 'h',
                .access_letters = "h",
                .access_name = "help",
                .description = "Show the help message."}};

/**
 * @brief Parse the CLI options provided by the user
 * @param[in] argc Argument count
 * @param[in] argv Argument vector
 * @param[in] cli_options Struct to store the user arguments
 * @return Status code
 **/
int parse_options(int argc, char **argv, struct builder_cli_configuration *cli_options) {
    char identifier;
    const char *value;
    cag_option_context context;

    // This flag will be set to true if the --vtor-align flag was supplied without a value
    bool vtor_flag = 0;

    // Initialize user arguments struct with default values
    struct builder_cli_configuration config = {
            false,
            false,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            NULL,
            NULL,
            NULL,
            false,
            false,
            false};

    // Help message
    if (argc == 1) {
        print_cli_help_message();
        return CLI_OK;
    }


    // Now we just prepare the context and iterate over all options.
    cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
    while (cag_option_fetch(&context)) {
        identifier = cag_option_get(&context);
        switch (identifier) {
            case 'i':
                value = cag_option_get_value(&context);
                config.input = value;
                break;
            case 'b':
                config.verbose = true;
                break;
            case 'o':
                value = cag_option_get_value(&context);
                config.output = value;
                break;
            case 'p':
                value = cag_option_get_value(&context);
                config.vtor_align = value;
                vtor_flag = true;
                break;
            case 'f':
                value = cag_option_get_value(&context);
                config.firmware_version = value;
                break;
            case 'x':
                value = cag_option_get_value(&context);
                config.firmware_index = value;
                break;
            case 'e':
                value = cag_option_get_value(&context);
                config.encryption_algo = value;
                break;
            case 'k':
                value = cag_option_get_value(&context);
                config.encryption_key_ascii = value;
                break;
            case 'd':
                value = cag_option_get_value(&context);
                config.encryption_key_hex = value;
                break;
            case 'a':
                value = cag_option_get_value(&context);
                config.authentication_algo = value;
                break;
            case 'u':
                value = cag_option_get_value(&context);
                config.authentication_key_ascii = value;
                break;
            case 'j':
                value = cag_option_get_value(&context);
                config.authentication_key_hex = value;
                break;
            case 's':
                value = cag_option_get_value(&context);
                config.signature_algo = value;
                break;
            case 'g':
                value = cag_option_get_value(&context);
                config.signature_key = value;
                break;
            case 'n':
                value = cag_option_get_value(&context);
                config.integrity_algo = value;
                break;
            case 'v':
                config.version = true;
                break;
            case 'h':
            default:
                print_cli_help_message();
                return CLI_OK;
        }
    }

    // Display the image builder utility version
    if(config.version) {
        printf("CLI Tool Version: %d.%d.%d.%s\n",
               image_builder_VERSION_MAJOR, image_builder_VERSION_MINOR, image_builder_VERSION_PATCH, image_builder_TIMESTAMP);
        return CLI_OK;
    }

    // Warning message if the user supplies no firmware version
    if (config.firmware_version == NULL && config.verbose)
        printf("\nWarning: You haven't specified a firmware version."
               "\nIf you are using fallback mode. Please enter the firmware version (ex. --firmware-version 1.0.1).\n");

    // Set the default firmware version if no user-supplied firmware version now
    if (config.firmware_version == NULL) {
        config.firmware_version = "0";
    }

    // Calculate the padding to apply between image header and the firmware binary in certain contexts
    // (i.e., Single Bank mode)
    if (!config.vtor_align && config.verbose) {
        printf("\nWarning: You haven't specified a padding."
               "\nIf you are generating a firmware image in 'Single Bank' mode. Please add -p flag and regenerate an image.\n");
    }

    if(!config.vtor_align && vtor_flag) {
        config.vtor_align = "1024";
    }

    if(!vtor_flag) {
        config.vtor_align = "0";
    }

    // Make sure the path to input binary is present
    if (!config.input) {
        printf("\nError: Input binary path is missing.\n");
        return EXIT_FAILURE;
    }

    // Make sure the path to output image is present
    if (!config.output) {
        printf("\nError: Output image path is missing.\n");
        return EXIT_FAILURE;
    }

    // convert the encryption key (either ASCII or HEX) to a unified format
    if(config.encryption_key_ascii || config.encryption_key_hex) {
        if(config.encryption_key_ascii) {
            config.encryption_key_len = strlen(config.encryption_key_ascii);
            config.encryption_key = (uint8_t *)config.encryption_key_ascii;
        }

        if(config.encryption_key_hex) {

            size_t len;
            if(hex_string_to_byte_array(config.encryption_key_hex,&config.encryption_key,
                                     &len))
                return EXIT_FAILURE;

            config.encryption_key_len = len;
        }
    }

    if(config.encryption_key_ascii && config.encryption_key_hex) {
        printf("ERROR: cannot parse two encryption key types.\r\n");
        return EXIT_FAILURE;
    }

    // check encryption options, if passed
    if ((config.encryption_algo || config.encryption_key) &&
        check_constraints_encryption(config.encryption_algo, config.encryption_key, config.encryption_key_len)) {
        return EXIT_FAILURE;
    }

    // convert the authentication key (either ASCII or HEX) to a unified format
    if(config.authentication_key_ascii || config.authentication_key_hex) {
        if(config.authentication_key_ascii) {
            config.authentication_key_len = strlen(config.authentication_key_ascii);
            config.authentication_key = (uint8_t *)config.authentication_key_ascii;
        }

        if(config.authentication_key_hex) {

            size_t len;
            hex_string_to_byte_array(config.authentication_key_hex,&config.authentication_key,
                                     &len);
            config.authentication_key_len = len;
        }
    }

    if(config.authentication_key_ascii && config.authentication_key_hex) {
        printf("ERROR: cannot parse two authentication key types.\r\n");
        return EXIT_FAILURE;
    }

    // check authentication options, if passed
    if ((config.authentication_algo || config.authentication_key) &&
        check_constraints_authentication(config.authentication_algo, config.authentication_key)) {
        return EXIT_FAILURE;
    }

    // check signature options, if passed
    if ((config.signature_algo || config.signature_key) &&
        check_constraints_signature(config.signature_algo, config.signature_key)) {
        return EXIT_FAILURE;
    }

// check integrity options, if passed
#ifdef IS_WINDOWS
    if (config.integrity_algo)
    {
        if (_strnicmp(config.integrity_algo, "md5",3) != 0 &&
            _strnicmp(config.integrity_algo, "sha256",6) != 0 &&
            _strnicmp(config.integrity_algo, "sha512",6) != 0 &&
            _strnicmp(config.integrity_algo, "crc32",5) != 0 &&
            _strnicmp(config.integrity_algo, "sha1",3) != 0)
            return EXIT_FAILURE;
    }
#endif
#ifdef IS_LINUX

    if (config.integrity_algo && strcasecmp(config.integrity_algo, "md5") != 0 &&
        strcasecmp(config.integrity_algo, "sha256") != 0 &&
        strcasecmp(config.integrity_algo, "sha224") != 0 &&
        strcasecmp(config.integrity_algo, "sha384") != 0 &&
        strcasecmp(config.integrity_algo, "sha512") != 0 &&
        strcasecmp(config.integrity_algo, "crc32") != 0 &&
        strcasecmp(config.integrity_algo, "sha1") != 0)
        return EXIT_FAILURE;

#endif

    // check data field validation
    if (config.integrity_algo && config.signature_algo && config.authentication_algo) {
        printf("Error: please choose ONE image integrity validation method.\n");
        return EXIT_FAILURE;
    }

    // copying the CLI options to the input parameter struct
    memcpy((void *) cli_options, (void *) &config, sizeof(config));

    return EXIT_SUCCESS;
}
