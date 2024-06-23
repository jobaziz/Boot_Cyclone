# CycloneBOOT

## Package Content:

Our "Open" and "Eval" packages include all the components you need to evaluate CycloneBOOT before deciding to use it in an open-source project or moving to a commercial license.

This package also includes CycloneTCP (TCP/IP stack) and various third-party software (BSP drivers, HAL layers, RTOS, etc.) for demonstration purpose.

- **common/**: Common features to all ORYX libraries
- **cyclone_boot/**: CycloneBOOT source code (Bootloader and Update Library)
- **cyclone_crypto/**: CycloneCRYPTO source code (Crypto Library)
- **cyclone_tcp/**: CycloneTCP source code (TCP/IP Stack)
- **demo/**: Ready-to-use secure firmware update demo projects for various evaluation boards and IDEs/compilers
- **third_party/**: Third-party libraries (CMSIS, FreeRTOS, ST HAL)
- **utils/**:
   - CycloneBOOT ImageBuilder CLI utility used to generate firmware update images
   - ResourceCompiler CLI utility used in some demo projects to convert static assets (HTML files, JavaScript fil

## Available Demonstration Projects

The **demo/** directory contains various demonstration projects that are organized per chip vendor and then per evaluation board (example: `demo/st/nucleo_f429zi/`). The number of available demonstration projects may vary depending on the evaluation board's and MCU's features.

We offer three types of demonstrations, all focusing on secure firmware updates:

- **iap_dual_bank/** for In-Application Programming (IAP) demonstration designed for MCUs with Dual Bank flash capability
- **iap_single_bank/** for In-Application Programming (IAP) demonstration designed for MCUs with Single Bank flash
- **iap_single_bank_ext_mem/** for In-Application Programming (IAP) demonstration designed for MCUs with Single Bank flash used in conjunction with an External flash

For each type of demonstration, you can then choose a specific protocol for fetching the firmware update, like `http_server_demo` or `uart_ymodem_demo`. We will add other protocol demos over time.

**Notes:**
- Demonstrations (`*_demo`) within `iap_dual_bank/` are autonomous as they contain all the necessary stuff to conduct the firmware update process.
- Demonstrations (`*_demo`) within `iap_single_bank/` or `iap_single_bank_ext_mem/` require also a bootloader to complete the firmware update process. Therefore, you will find a `bootloader` directory in addition to the demonstration folder.

## Running the Demonstration Projects

Please carefully read and follow the detailed instructions in the  **README.md** file of the demonstration project you want to evaluate. This document contains important information on how to use the software effectively, ensuring a smooth onboarding experience.

For example: `demo/st/nucleo_f429zi/iap_dual_bank/http_server_demo/README.md`

Feel free to contact us if you need support during your evaluation period or if you require a custom demo:
[info@oryx-embedded.com](info@oryx-embedded.com)