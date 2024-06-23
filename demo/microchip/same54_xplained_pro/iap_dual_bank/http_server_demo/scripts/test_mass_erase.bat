@echo off

REM J-Link Commander
REM 1. "J-Link Software and Documentation pack" must be installed first: https://www.segger.com/downloads/jlink/ (version 7.22)
REM 2. Add J-Link.exe (J-Link Commander CLI) executable to the environment PATH (C:\Program Files (x86)\SEGGER\JLink_V722)

:: J-Link Commander CLI executable absolute path
set "PATH=%PATH%;C:\Program Files (x86)\SEGGER\JLink_V722"

:: Mass erase internal MCU flash memory
JLink.exe -device ATSAME54P20 -if SWD -speed 4000 -autoconnect 1 -CommanderScript ./jlink/test_mass_erase.jlink

echo Done.
pause
