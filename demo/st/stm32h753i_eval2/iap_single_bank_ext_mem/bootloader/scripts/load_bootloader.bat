@echo off

REM STM32CubeProgrammer
REM   1. STM32CubeProgrammer must be installed first : https://www.st.com/en/development-tools/stm32cubeprog.html
REM   2. Set the STM32CubeProgrammer CLI executable path in the variable bellow according your installation path

:: STM32CubeProgrammer CLI executable absolute path
set "PATH=%PATH%;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"

:: Load bootloader binary in MCU flash bank 1 at 0x08000000
STM32_Programmer_CLI.exe -c port=SWD index=0 -d ..\bootloader.bin 0x08000000 -halt -rst

pause
