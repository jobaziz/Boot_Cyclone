/**
 * \file
 *
 * \brief System configuration file for SAME54
 *
 * Copyright (c) 2022 Microchip Technology Inc.
 *
 * \license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \license_stop
 *
 */

#include "sam.h"

/**
 * Initial system clock frequency.
 */
#define __SYSTEM_CLOCK    (120000000)

uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/

/**
 * Initialize the system
 *
 * \brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit(void)
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    // Enable FPU
    SCB->CPACR |= (3UL << (10 * 2)) | (3UL << (11 * 2));
#endif

    // Run with a 12MHz external crystal on XOSC1
    OSCCTRL_REGS->OSCCTRL_XOSCCTRL[1] = OSCCTRL_XOSCCTRL_ENALC(1) |
        OSCCTRL_XOSCCTRL_IMULT(4) | OSCCTRL_XOSCCTRL_IPTAT(3) |
        OSCCTRL_XOSCCTRL_ONDEMAND(0) | OSCCTRL_XOSCCTRL_XTALEN(1) |
        OSCCTRL_XOSCCTRL_ENABLE(1);

    // Wait for OSC to be ready
    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_XOSCRDY1_Msk) == 0)
    {
    }

    // Set up DPLL0 to output 120MHz using XOSC1 output divided by 12
    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLRATIO = OSCCTRL_DPLLRATIO_LDRFRAC(0) |
        OSCCTRL_DPLLRATIO_LDR(119);

    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLB = OSCCTRL_DPLLCTRLB_DIV(5) |
        OSCCTRL_DPLLCTRLB_REFCLK(3);

    OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLCTRLA = OSCCTRL_DPLLCTRLA_ONDEMAND(0) |
        OSCCTRL_DPLLCTRLA_ENABLE(1);

    // Wait for PLL to be locked and ready
    while((OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSTATUS & OSCCTRL_DPLLSTATUS_LOCK_Msk) == 0 ||
    (OSCCTRL_REGS->DPLL[0].OSCCTRL_DPLLSTATUS & OSCCTRL_DPLLSTATUS_CLKRDY_Msk) == 0)
    {
    }

    // Connect DPLL0 to clock generator 0 (120MHz)
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_SRC_DPLL0 | GCLK_GENCTRL_DIV(1) |
        GCLK_GENCTRL_GENEN_Msk;

    // Wait for synchronization
    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK0) != 0)
    {
    }

    // Update system clock frequency
    SystemCoreClock = __SYSTEM_CLOCK;
    return;
}

/**
 * Update SystemCoreClock variable
 *
 * \brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
void SystemCoreClockUpdate(void)
{
    // Not implemented
    SystemCoreClock = __SYSTEM_CLOCK;
    return;
}
