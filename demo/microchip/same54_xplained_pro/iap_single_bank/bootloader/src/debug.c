/**
 * @file debug.c
 * @brief Debugging facilities
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

//Dependencies
#include "sam.h"
#include "debug.h"


/**
 * @brief Debug UART initialization
 * @param[in] baudrate UART baudrate
 **/

void debugInit(uint32_t baudrate)
{
   uint32_t temp;

   //Enable SERCOM2 core clock
   GCLK_REGS->GCLK_PCHCTRL[SERCOM2_GCLK_ID_CORE] = GCLK_PCHCTRL_GEN_GCLK0 |
      GCLK_PCHCTRL_CHEN_Msk;

   //Enable PORT bus clock (CLK_PORT_APB)
   MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_PORT_Msk;
   //Enable SERCOM2 bus clock (CLK_SERCOM2_APB)
   MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_SERCOM2_Msk;

   //Configure RXD pin (PB24)
   PORT_REGS->GROUP[1].PORT_PINCFG[24] |= PORT_PINCFG_PULLEN_Msk;
   PORT_REGS->GROUP[1].PORT_PINCFG[24] |= PORT_PINCFG_PMUXEN_Msk;
   temp = PORT_REGS->GROUP[1].PORT_PMUX[12] & ~PORT_PMUX_PMUXE_Msk;
   PORT_REGS->GROUP[1].PORT_PMUX[12] = temp | PORT_PMUX_PMUXE(MUX_PB24D_SERCOM2_PAD1);

   //Configure TXD pin (PB25)
   PORT_REGS->GROUP[1].PORT_PINCFG[25] |= PORT_PINCFG_PMUXEN_Msk;
   temp = PORT_REGS->GROUP[1].PORT_PMUX[12] & ~PORT_PMUX_PMUXO_Msk;
   PORT_REGS->GROUP[1].PORT_PMUX[12] = temp | PORT_PMUX_PMUXO(MUX_PB25D_SERCOM2_PAD0);

   //Perform software reset
   SERCOM2_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_SWRST_Msk;

   //Resetting the SERCOM (CTRLA.SWRST) requires synchronization
   while((SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_SWRST_Msk) != 0)
   {
   }

   //Configure SERCOM2
   SERCOM2_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_DORD_Msk |
      SERCOM_USART_INT_CTRLA_RXPO(1) | SERCOM_USART_INT_CTRLA_TXPO(0) |
      SERCOM_USART_INT_CTRLA_SAMPR(0) | SERCOM_USART_INT_CTRLA_MODE(1);

   SERCOM2_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_TXEN_Msk |
      SERCOM_USART_INT_CTRLB_RXEN_Msk | SERCOM_USART_INT_CTRLB_CHSIZE(0);

   //Writing to the CTRLB register when the SERCOM is enabled requires
   //synchronization
   while((SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_CTRLB_Msk) != 0)
   {
   }

   //Configure baud rate
   SERCOM2_REGS->USART_INT.SERCOM_BAUD = 65535 - ((baudrate * 16384) / (SystemCoreClock / 64));

   //Enable SERCOM2
   SERCOM2_REGS->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

   //Enabling and disabling the SERCOM requires synchronization
   while((SERCOM2_REGS->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk) != 0)
   {
   }
}


/**
 * @brief Display the contents of an array
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @param[in] prepend String to prepend to the left of each line
 * @param[in] data Pointer to the data array
 * @param[in] length Number of bytes to display
 **/

void debugDisplayArray(FILE *stream,
   const char_t *prepend, const void *data, size_t length)
{
   uint_t i;

   for(i = 0; i < length; i++)
   {
      //Beginning of a new line?
      if((i % 16) == 0)
         fprintf(stream, "%s", prepend);
      //Display current data byte
      fprintf(stream, "%02" PRIX8 " ", *((uint8_t *) data + i));
      //End of current line?
      if((i % 16) == 15 || i == (length - 1))
         fprintf(stream, "\r\n");
   }
}


/**
 * @brief Write character to stream
 * @param[in] c The character to be written
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @return On success, the character written is returned. If a writing
 *   error occurs, EOF is returned
 **/

int_t fputc(int_t c, FILE *stream)
{
   //Standard output or error output?
   if(stream == stdout || stream == stderr)
   {
      //Send character
      SERCOM2_REGS->USART_INT.SERCOM_DATA = c;

      //Wait for the transfer to complete
      while((SERCOM2_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == 0)
      {
      }

      //On success, the character written is returned
      return c;
   }
   //Unknown output?
   else
   {
      //If a writing error occurs, EOF is returned
      return EOF;
   }
}
