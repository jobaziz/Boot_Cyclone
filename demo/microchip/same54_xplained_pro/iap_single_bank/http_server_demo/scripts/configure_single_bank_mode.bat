@echo off

REM J-Link Commander
REM 1. "J-Link Software and Documentation pack" must be installed first: https://www.segger.com/downloads/jlink/ (version 7.22)
REM 2. Add J-Link.exe (J-Link Commander CLI) executable to the environment PATH (C:\Program Files (x86)\SEGGER\JLink_V722)

:: J-Link Commander CLI executable absolute path
set "PATH=%PATH%;C:\Program Files (x86)\SEGGER\JLink_V722"

set LOG_FILE=temp.log

:: Get NVMCTR STATUS register value and save it in "temp.log" file
echo Configuring ATMEL MCU flash in dual bank mode...
JLink.exe -device ATSAME54P20 -if SWD -speed 4000 -autoconnect 1 -CommanderScript ./jlink/read_nvmctrl_reg.jlink 1>Nul

:: Extract from "temp.log" the line that contains NVMCTRL STATUS register value
set str=
for /f "delims=" %%a in ('findstr Data: %LOG_FILE%') do @set str=%%a

:: Get NVMCTRL STATUS AFIRST bit from extracted line
set bit=%str:~-2,1%
set /a "AFIRST=%bit% & 1"

:: Is current app running in flash bankA?
if "%AFIRST%"=="1" goto :end else goto :swap

:: Manage swap back operation
:: ==========================
:swap
echo Running in flash bankB.

:: Swap back on flash bankA
echo Swaping back on flash bankA...
JLink.exe -device ATSAME54P20 -if SWD -speed 4000 -autoconnect 1 -CommanderScript ./jlink/swap_flash_bank.jlink 1>Nul
goto :end   


:: End of process
:: ==========================
:end
del /F %LOG_FILE%
echo Done.
pause
