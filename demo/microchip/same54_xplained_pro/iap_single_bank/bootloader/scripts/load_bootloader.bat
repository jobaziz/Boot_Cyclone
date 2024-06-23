@echo off

REM J-Link Commander
REM 1. "J-Link Software and Documentation pack" must be installed first: https://www.segger.com/downloads/jlink/ (version 7.22)
REM 2. Add J-Link.exe (J-Link Commander CLI) executable to the environment PATH (C:\Program Files (x86)\SEGGER\JLink_V722)

:: J-Link Commander CLI executable absolute path
set "PATH=%PATH%;C:\Program Files (x86)\SEGGER\JLink_V722"

:: Load initial bootable image (containing initial firmware binary) in MCU flash bank 1 at 0x08020000
JLink.exe -device ATSAME54P20 -if SWD -speed 4000 -autoconnect 1 -CommanderScript ./jlink/load_bootloader.jlink

echo Done.
pause
