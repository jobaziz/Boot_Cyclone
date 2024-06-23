//Dependencies
#include "sam.h"

//Linker file constants
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

//Function declaration
void SystemInit(void);
void __libc_init_array(void);
int main(void);

//Default empty handler
void Default_Handler(void);

//Cortex-M4 core handlers
void Reset_Handler            (void);
void NonMaskableInt_Handler   (void)__attribute__((weak, alias("Default_Handler")));
void HardFault_Handler        (void)__attribute__((weak, alias("Default_Handler")));
void MemoryManagement_Handler (void)__attribute__((weak, alias("Default_Handler")));
void BusFault_Handler         (void)__attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler       (void)__attribute__((weak, alias("Default_Handler")));
void SVCall_Handler           (void)__attribute__((weak, alias("Default_Handler")));
void DebugMonitor_Handler     (void)__attribute__((weak, alias("Default_Handler")));
void PendSV_Handler           (void)__attribute__((weak, alias("Default_Handler")));
void SysTick_Handler          (void)__attribute__((weak, alias("Default_Handler")));

//Peripheral handlers
void PM_Handler            (void) __attribute__((weak, alias("Default_Handler")));
void MCLK_Handler          (void) __attribute__((weak, alias("Default_Handler")));
void OSCCTRL_XOSC0_Handler (void) __attribute__((weak, alias("Default_Handler")));
void OSCCTRL_XOSC1_Handler (void) __attribute__((weak, alias("Default_Handler")));
void OSCCTRL_DFLL_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void OSCCTRL_DPLL0_Handler (void) __attribute__((weak, alias("Default_Handler")));
void OSCCTRL_DPLL1_Handler (void) __attribute__((weak, alias("Default_Handler")));
void OSC32KCTRL_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void SUPC_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void SUPC_BODDET_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void WDT_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void RTC_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_0_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_1_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_2_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_3_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_4_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_5_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_6_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_7_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_8_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_9_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_10_Handler (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_11_Handler (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_12_Handler (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_13_Handler (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_14_Handler (void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_15_Handler (void) __attribute__((weak, alias("Default_Handler")));
void FREQM_Handler         (void) __attribute__((weak, alias("Default_Handler")));
void NVMCTRL_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void NVMCTRL_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void DMAC_0_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void DMAC_1_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void DMAC_2_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void DMAC_3_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void DMAC_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void EVSYS_0_Handler       (void) __attribute__((weak, alias("Default_Handler")));
void EVSYS_1_Handler       (void) __attribute__((weak, alias("Default_Handler")));
void EVSYS_2_Handler       (void) __attribute__((weak, alias("Default_Handler")));
void EVSYS_3_Handler       (void) __attribute__((weak, alias("Default_Handler")));
void EVSYS_OTHER_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void PAC_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void RAMECC_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM0_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM0_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM0_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM0_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM1_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM1_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM1_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM1_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM2_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM2_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM2_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM2_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM3_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM3_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM3_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM3_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM4_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM4_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM4_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM4_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM5_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM5_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM5_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM5_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM6_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM6_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM6_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM6_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM7_0_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM7_1_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM7_2_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void SERCOM7_OTHER_Handler (void) __attribute__((weak, alias("Default_Handler")));
void CAN0_Handler          (void) __attribute__((weak, alias("Default_Handler")));
void CAN1_Handler          (void) __attribute__((weak, alias("Default_Handler")));
void USB_OTHER_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void USB_SOF_HSOF_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void USB_TRCPT0_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void USB_TRCPT1_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void GMAC_Handler          (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_MC0_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_MC1_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_MC2_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_MC3_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_MC4_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC0_MC5_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC1_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void TCC1_MC0_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC1_MC1_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC1_MC2_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC1_MC3_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC2_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void TCC2_MC0_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC2_MC1_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC2_MC2_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC3_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void TCC3_MC0_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC3_MC1_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC4_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void TCC4_MC0_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TCC4_MC1_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void TC0_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC1_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC2_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC3_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC4_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC5_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC6_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TC7_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void PDEC_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void PDEC_MC0_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void PDEC_MC1_Handler      (void) __attribute__((weak, alias("Default_Handler")));
void ADC0_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void ADC0_RESRDY_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void ADC1_OTHER_Handler    (void) __attribute__((weak, alias("Default_Handler")));
void ADC1_RESRDY_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void AC_Handler            (void) __attribute__((weak, alias("Default_Handler")));
void DAC_OTHER_Handler     (void) __attribute__((weak, alias("Default_Handler")));
void DAC_EMPTY_0_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void DAC_EMPTY_1_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void DAC_RESRDY_0_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void DAC_RESRDY_1_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void I2S_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void PCC_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void AES_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void TRNG_Handler          (void) __attribute__((weak, alias("Default_Handler")));
void ICM_Handler           (void) __attribute__((weak, alias("Default_Handler")));
void PUKCC_Handler         (void) __attribute__((weak, alias("Default_Handler")));
void QSPI_Handler          (void) __attribute__((weak, alias("Default_Handler")));
void SDHC0_Handler         (void) __attribute__((weak, alias("Default_Handler")));
void SDHC1_Handler         (void) __attribute__((weak, alias("Default_Handler")));

//Vector table
__attribute__((section(".vectors")))
const uint32_t vectorTable[153] =
{
   //Initial stack pointer
   (uint32_t) (&_estack),

   //Core handlers
   (uint32_t) Reset_Handler,
   (uint32_t) NonMaskableInt_Handler,
   (uint32_t) HardFault_Handler,
   (uint32_t) MemoryManagement_Handler,
   (uint32_t) BusFault_Handler,
   (uint32_t) UsageFault_Handler,
   (uint32_t) 0,
   (uint32_t) 0,
   (uint32_t) 0,
   (uint32_t) 0,
   (uint32_t) SVCall_Handler,
   (uint32_t) DebugMonitor_Handler,
   (uint32_t) 0,
   (uint32_t) PendSV_Handler,
   (uint32_t) SysTick_Handler,

   //Peripheral handlers
   (uint32_t) PM_Handler,            // 0  Power Manager
   (uint32_t) MCLK_Handler,          // 1  Main Clock
   (uint32_t) OSCCTRL_XOSC0_Handler, // 2  Oscillators Control
   (uint32_t) OSCCTRL_XOSC1_Handler, // 3  Oscillators Control
   (uint32_t) OSCCTRL_DFLL_Handler,  // 4  Oscillators Control
   (uint32_t) OSCCTRL_DPLL0_Handler, // 5  Oscillators Control
   (uint32_t) OSCCTRL_DPLL1_Handler, // 6  Oscillators Control
   (uint32_t) OSC32KCTRL_Handler,    // 7  32kHz Oscillators Control
   (uint32_t) SUPC_OTHER_Handler,    // 8  Supply Controller
   (uint32_t) SUPC_BODDET_Handler,   // 9  Supply Controller
   (uint32_t) WDT_Handler,           // 10 Watchdog Timer
   (uint32_t) RTC_Handler,           // 11 Real-Time Counter
   (uint32_t) EIC_EXTINT_0_Handler,  // 12 External Interrupt Controller
   (uint32_t) EIC_EXTINT_1_Handler,  // 13 External Interrupt Controller
   (uint32_t) EIC_EXTINT_2_Handler,  // 14 External Interrupt Controller
   (uint32_t) EIC_EXTINT_3_Handler,  // 15 External Interrupt Controller
   (uint32_t) EIC_EXTINT_4_Handler,  // 16 External Interrupt Controller
   (uint32_t) EIC_EXTINT_5_Handler,  // 17 External Interrupt Controller
   (uint32_t) EIC_EXTINT_6_Handler,  // 18 External Interrupt Controller
   (uint32_t) EIC_EXTINT_7_Handler,  // 19 External Interrupt Controller
   (uint32_t) EIC_EXTINT_8_Handler,  // 20 External Interrupt Controller
   (uint32_t) EIC_EXTINT_9_Handler,  // 21 External Interrupt Controller
   (uint32_t) EIC_EXTINT_10_Handler, // 22 External Interrupt Controller
   (uint32_t) EIC_EXTINT_11_Handler, // 23 External Interrupt Controller
   (uint32_t) EIC_EXTINT_12_Handler, // 24 External Interrupt Controller
   (uint32_t) EIC_EXTINT_13_Handler, // 25 External Interrupt Controller
   (uint32_t) EIC_EXTINT_14_Handler, // 26 External Interrupt Controller
   (uint32_t) EIC_EXTINT_15_Handler, // 27 External Interrupt Controller
   (uint32_t) FREQM_Handler,         // 28 Frequency Meter
   (uint32_t) NVMCTRL_0_Handler,     // 29 Non-Volatile Memory Controller
   (uint32_t) NVMCTRL_1_Handler,     // 30 Non-Volatile Memory Controller
   (uint32_t) DMAC_0_Handler,        // 31 Direct Memory Access Controller
   (uint32_t) DMAC_1_Handler,        // 32 Direct Memory Access Controller
   (uint32_t) DMAC_2_Handler,        // 33 Direct Memory Access Controller
   (uint32_t) DMAC_3_Handler,        // 34 Direct Memory Access Controller
   (uint32_t) DMAC_OTHER_Handler,    // 35 Direct Memory Access Controller
   (uint32_t) EVSYS_0_Handler,       // 36 Event System Interface
   (uint32_t) EVSYS_1_Handler,       // 37 Event System Interface
   (uint32_t) EVSYS_2_Handler,       // 38 Event System Interface
   (uint32_t) EVSYS_3_Handler,       // 39 Event System Interface
   (uint32_t) EVSYS_OTHER_Handler,   // 40 Event System Interface
   (uint32_t) PAC_Handler,           // 41 Peripheral Access Controller
   (uint32_t) 0,                     // 42 Reserved
   (uint32_t) 0,                     // 43 Reserved
   (uint32_t) 0,                     // 44 Reserved
   (uint32_t) RAMECC_Handler,        // 45 RAM ECC
   (uint32_t) SERCOM0_0_Handler,     // 46 Serial Communication Interface
   (uint32_t) SERCOM0_1_Handler,     // 47 Serial Communication Interface
   (uint32_t) SERCOM0_2_Handler,     // 48 Serial Communication Interface
   (uint32_t) SERCOM0_OTHER_Handler, // 49 Serial Communication Interface
   (uint32_t) SERCOM1_0_Handler,     // 50 Serial Communication Interface
   (uint32_t) SERCOM1_1_Handler,     // 51 Serial Communication Interface
   (uint32_t) SERCOM1_2_Handler,     // 52 Serial Communication Interface
   (uint32_t) SERCOM1_OTHER_Handler, // 53 Serial Communication Interface
   (uint32_t) SERCOM2_0_Handler,     // 54 Serial Communication Interface
   (uint32_t) SERCOM2_1_Handler,     // 55 Serial Communication Interface
   (uint32_t) SERCOM2_2_Handler,     // 56 Serial Communication Interface
   (uint32_t) SERCOM2_OTHER_Handler, // 57 Serial Communication Interface
   (uint32_t) SERCOM3_0_Handler,     // 58 Serial Communication Interface
   (uint32_t) SERCOM3_1_Handler,     // 59 Serial Communication Interface
   (uint32_t) SERCOM3_2_Handler,     // 60 Serial Communication Interface
   (uint32_t) SERCOM3_OTHER_Handler, // 61 Serial Communication Interface
   (uint32_t) SERCOM4_0_Handler,     // 62 Serial Communication Interface
   (uint32_t) SERCOM4_1_Handler,     // 63 Serial Communication Interface
   (uint32_t) SERCOM4_2_Handler,     // 64 Serial Communication Interface
   (uint32_t) SERCOM4_OTHER_Handler, // 65 Serial Communication Interface
   (uint32_t) SERCOM5_0_Handler,     // 66 Serial Communication Interface
   (uint32_t) SERCOM5_1_Handler,     // 67 Serial Communication Interface
   (uint32_t) SERCOM5_2_Handler,     // 68 Serial Communication Interface
   (uint32_t) SERCOM5_OTHER_Handler, // 69 Serial Communication Interface
   (uint32_t) SERCOM6_0_Handler,     // 70 Serial Communication Interface
   (uint32_t) SERCOM6_1_Handler,     // 71 Serial Communication Interface
   (uint32_t) SERCOM6_2_Handler,     // 72 Serial Communication Interface
   (uint32_t) SERCOM6_OTHER_Handler, // 73 Serial Communication Interface
   (uint32_t) SERCOM7_0_Handler,     // 74 Serial Communication Interface
   (uint32_t) SERCOM7_1_Handler,     // 75 Serial Communication Interface
   (uint32_t) SERCOM7_2_Handler,     // 76 Serial Communication Interface
   (uint32_t) SERCOM7_OTHER_Handler, // 77 Serial Communication Interface
   (uint32_t) CAN0_Handler,          // 78 Control Area Network
   (uint32_t) CAN1_Handler,          // 79 Control Area Network
   (uint32_t) USB_OTHER_Handler,     // 80 Universal Serial Bus
   (uint32_t) USB_SOF_HSOF_Handler,  // 81 Universal Serial Bus
   (uint32_t) USB_TRCPT0_Handler,    // 82 Universal Serial Bus
   (uint32_t) USB_TRCPT1_Handler,    // 83 Universal Serial Bus
   (uint32_t) GMAC_Handler,          // 84 Ethernet MAC
   (uint32_t) TCC0_OTHER_Handler,    // 85 Timer Counter Control
   (uint32_t) TCC0_MC0_Handler,      // 86 Timer Counter Control
   (uint32_t) TCC0_MC1_Handler,      // 87 Timer Counter Control
   (uint32_t) TCC0_MC2_Handler,      // 88 Timer Counter Control
   (uint32_t) TCC0_MC3_Handler,      // 89 Timer Counter Control
   (uint32_t) TCC0_MC4_Handler,      // 90 Timer Counter Control
   (uint32_t) TCC0_MC5_Handler,      // 91 Timer Counter Control
   (uint32_t) TCC1_OTHER_Handler,    // 92 Timer Counter Control
   (uint32_t) TCC1_MC0_Handler,      // 93 Timer Counter Control
   (uint32_t) TCC1_MC1_Handler,      // 94 Timer Counter Control
   (uint32_t) TCC1_MC2_Handler,      // 95 Timer Counter Control
   (uint32_t) TCC1_MC3_Handler,      // 96 Timer Counter Control
   (uint32_t) TCC2_OTHER_Handler,    // 97 Timer Counter Control
   (uint32_t) TCC2_MC0_Handler,      // 98 Timer Counter Control
   (uint32_t) TCC2_MC1_Handler,      // 99 Timer Counter Control
   (uint32_t) TCC2_MC2_Handler,      // 100 Timer Counter Control
   (uint32_t) TCC3_OTHER_Handler,    // 101 Timer Counter Control
   (uint32_t) TCC3_MC0_Handler,      // 102 Timer Counter Control
   (uint32_t) TCC3_MC1_Handler,      // 103 Timer Counter Control
   (uint32_t) TCC4_OTHER_Handler,    // 104 Timer Counter Control
   (uint32_t) TCC4_MC0_Handler,      // 105 Timer Counter Control
   (uint32_t) TCC4_MC1_Handler,      // 106 Timer Counter Control
   (uint32_t) TC0_Handler,           // 107 Basic Timer Counter
   (uint32_t) TC1_Handler,           // 108 Basic Timer Counter
   (uint32_t) TC2_Handler,           // 109 Basic Timer Counter
   (uint32_t) TC3_Handler,           // 110 Basic Timer Counter
   (uint32_t) TC4_Handler,           // 111 Basic Timer Counter
   (uint32_t) TC5_Handler,           // 112 Basic Timer Counter
   (uint32_t) TC6_Handler,           // 113 Basic Timer Counter
   (uint32_t) TC7_Handler,           // 114 Basic Timer Counter
   (uint32_t) PDEC_OTHER_Handler,    // 115 Quadrature Decodeur
   (uint32_t) PDEC_MC0_Handler,      // 116 Quadrature Decodeur
   (uint32_t) PDEC_MC1_Handler,      // 117 Quadrature Decodeur
   (uint32_t) ADC0_OTHER_Handler,    // 118 Analog Digital Converter
   (uint32_t) ADC0_RESRDY_Handler,   // 119 Analog Digital Converter
   (uint32_t) ADC1_OTHER_Handler,    // 120 Analog Digital Converter
   (uint32_t) ADC1_RESRDY_Handler,   // 121 Analog Digital Converter
   (uint32_t) AC_Handler,            // 122 Analog Comparators
   (uint32_t) DAC_OTHER_Handler,     // 123 Digital-to-Analog Converter
   (uint32_t) DAC_EMPTY_0_Handler,   // 124 Digital-to-Analog Converter
   (uint32_t) DAC_EMPTY_1_Handler,   // 125 Digital-to-Analog Converter
   (uint32_t) DAC_RESRDY_0_Handler,  // 126 Digital-to-Analog Converter
   (uint32_t) DAC_RESRDY_1_Handler,  // 127 Digital-to-Analog Converter
   (uint32_t) I2S_Handler,           // 128 Inter-IC Sound Interface
   (uint32_t) PCC_Handler,           // 129 Parallel Capture Controller
   (uint32_t) AES_Handler,           // 130 Advanced Encryption Standard
   (uint32_t) TRNG_Handler,          // 131 True Random Generator
   (uint32_t) ICM_Handler,           // 132 Integrity Check Monitor
   (uint32_t) PUKCC_Handler,         // 133 PUblic-Key Cryptography Controller
   (uint32_t) QSPI_Handler,          // 134 Quad SPI interface
   (uint32_t) SDHC0_Handler,         // 135 SD/MMC Host Controller
   (uint32_t) SDHC1_Handler          // 136 SD/MMC Host Controller
};


/**
 * @brief Reset handler
 **/

void Reset_Handler(void)
{
   uint32_t *src;
   uint32_t *dest;

   //System initialization
   SystemInit();

   //Initialize the relocate segment
   src = &_etext;
   dest = &_srelocate;

   if(src != dest)
   {
      while(dest < &_erelocate)
	  {
         *dest++ = *src++;
      }
   }

   //Clear the zero segment
   for(dest = &_szero; dest < &_ezero;)
   {
      *dest++ = 0;
   }

   //Set the vector table base address
   src = (uint32_t *) & _sfixed;
   SCB->VTOR = ((uint32_t) src & SCB_VTOR_TBLOFF_Msk);

   //C library initialization
   __libc_init_array();

   //Branch to main function
   main();

   //Endless loop
   while(1);
}


/**
 * @brief Default interrupt handler
 **/

void Default_Handler(void)
{
   while(1)
   {
   }
}
