/**
 * @file adin1110_driver.c
 * @brief ADIN1110 10Base-T1L Ethernet controller
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
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
 * @version 2.4.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "drivers/eth/adin1110_driver.h"
#include "debug.h"


/**
 * @brief ADIN1110 driver
 **/

const NicDriver adin1110Driver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   adin1110Init,
   adin1110Tick,
   adin1110EnableIrq,
   adin1110DisableIrq,
   adin1110EventHandler,
   adin1110SendPacket,
   adin1110UpdateMacAddrFilter,
   NULL,
   NULL,
   NULL,
   FALSE,
   TRUE,
   TRUE,
   FALSE
};


/**
 * @brief ADIN1110 controller initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t adin1110Init(NetInterface *interface)
{
   uint32_t value;

   //Debug message
   TRACE_INFO("Initializing ADIN1110 Ethernet controller...\r\n");

   //Initialize SPI interface
   interface->spiDriver->init();

   //Initialize external interrupt line driver
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->init();
   }

   //A full chip software reset can be initiated by writing 1 to the SWRESET
   //field of the RESET register
   adin1110WriteReg(interface, ADIN1110_RESET, ADIN1110_RESET_SWRESET);

   //Wait for the MAC to exit reset
   do
   {
      //To confirm that the MAC has exited reset, read the PHY identification
      //register
      value = adin1110ReadReg(interface, ADIN1110_PHYID);

      //If the reset value of the register can be read, the device has exited
      //reset and is ready for configuration
   } while(value != (ADIN1110_PHYID_OUI_DEFAULT | ADIN1110_PHYID_MODEL_DEFAULT |
      ADIN1110_PHYID_REVISION_DEFAULT));

   //Next, the host must read the STATUS0 register and confirm that the RESETC
   //field is 1
   do
   {
      //Read the status register 0
      value = adin1110ReadReg(interface, ADIN1110_STATUS0);

      //Check the value of the RESETC bit
   } while((value & ADIN1110_STATUS0_RESETC) == 0);

   //Write 1 to the RESETC field in the STATUS0 register to clear this field
   adin1110WriteReg(interface, ADIN1110_STATUS0, ADIN1110_STATUS0_RESETC);

   //The system ready bit can also be read to verify that the start-up sequence
   //is complete and the system is ready for normal operation
   do
   {
      //Read the CRSM status register
      value = adin1110ReadMmdReg(interface, ADIN1110_CRSM_STAT);

      //Check the value of the CRSM_SYS_RDY bit
   } while((value & ADIN1110_CRSM_STAT_CRSM_SYS_RDY) == 0);

   //Dump SPI registers for debugging purpose
   adin1110DumpReg(interface);
   //Dump PHY registers for debugging purpose
   adin1110DumpPhyReg(interface);

   //Configure MAC address filtering
   adin1110UpdateMacAddrFilter(interface);

#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   //Configure the SPI protocol engine
   adin1110WriteReg(interface, ADIN1110_CONFIG0, ADIN1110_CONFIG0_CSARFE |
      ADIN1110_CONFIG0_ZARFE | ADIN1110_CONFIG0_TXCTHRESH_16_CREDITS |
      ADIN1110_CONFIG0_CPS_64B);
#else
   //Enable store and forward mode
   value = adin1110ReadReg(interface, ADIN1110_CONFIG0);
   value &= ~(ADIN1110_CONFIG0_TXCTE | ADIN1110_CONFIG0_RXCTE);
   adin1110WriteReg(interface, ADIN1110_CONFIG0, value);
#endif

   //Enable CRC append in the MAC TX path
   value = adin1110ReadReg(interface, ADIN1110_CONFIG2);
   value |= ADIN1110_CONFIG2_CRC_APPEND;
   adin1110WriteReg(interface, ADIN1110_CONFIG2, value);

   //Clear IMASK0 register
   adin1110WriteReg(interface, ADIN1110_IMASK0, 0xFFFFFFFF);

#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   //Disable generic SPI protocol interrupts
   adin1110WriteReg(interface, ADIN1110_IMASK1, 0xFFFFFFFF);
#else
   //Write the IMASK1 register to enable interrupts as required
   adin1110WriteReg(interface, ADIN1110_IMASK1, ~(ADIN1110_IMASK1_TX_RDY_MASK |
      ADIN1110_IMASK1_P1_RX_RDY_MASK | ADIN1110_IMASK1_LINK_CHANGE_MASK));
#endif

   //When the MAC is configured, write 1 to the SYNC field in the CONFIG0
   //register to indicate that the MAC configuration is complete
   value = adin1110ReadReg(interface, ADIN1110_CONFIG0);
   value |= ADIN1110_CONFIG0_SYNC;
   adin1110WriteReg(interface, ADIN1110_CONFIG0, value);

   //Enable LED1 output
   value = adin1110ReadMmdReg(interface, ADIN1110_DIGIO_PINMUX);
   value &= ~ADIN1110_DIGIO_PINMUX_DIGIO_LED1_PINMUX;
   value |= ADIN1110_DIGIO_PINMUX_DIGIO_LED1_PINMUX_LED_1;
   adin1110WriteMmdReg(interface, ADIN1110_DIGIO_PINMUX, value);

   //Configure LED0 and LED1 function
   adin1110WriteMmdReg(interface, ADIN1110_LED_CNTRL,
      ADIN1110_LED_CNTRL_LED0_EN |
      ADIN1110_LED_CNTRL_LED0_FUNCTION_LINKUP_TXRX_ACTIVITY |
      ADIN1110_LED_CNTRL_LED1_EN |
      ADIN1110_LED_CNTRL_LED1_FUNCTION_MASTER);

   //Set LED0 and LED1 polarity
   adin1110WriteMmdReg(interface, ADIN1110_LED_POLARITY,
      ADIN1110_LED_POLARITY_LED0_POLARITY_AUTOSENSE |
      ADIN1110_LED_POLARITY_LED1_POLARITY_AUTOSENSE);

   //Perform custom configuration
   adin1110InitHook(interface);

   //Clear the CRSM_SFT_PD bit to exit software power-down mode. At this point,
   //the MAC-PHY starts autonegotiation and attempts to bring up a link after
   //autonegotiation completes
   value = adin1110ReadMmdReg(interface, ADIN1110_CRSM_SFT_PD_CNTRL);
   value &= ~ADIN1110_CRSM_SFT_PD_CNTRL_CRSM_SFT_PD;
   adin1110WriteMmdReg(interface, ADIN1110_CRSM_SFT_PD_CNTRL, value);

   //Accept any packets from the upper layer
   osSetEvent(&interface->nicTxEvent);

   //Force the TCP/IP stack to poll the link state at startup
   interface->nicEvent = TRUE;
   //Notify the TCP/IP stack of the event
   osSetEvent(&netEvent);

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief ADIN1110 custom configuration
 * @param[in] interface Underlying network interface
 **/

__weak_func void adin1110InitHook(NetInterface *interface)
{
}


/**
 * @brief ADIN1110 timer handler
 * @param[in] interface Underlying network interface
 **/

void adin1110Tick(NetInterface *interface)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   uint32_t value;
   bool_t linkState;

   //Read PHY status register
   value = adin1110ReadReg(interface, ADIN1110_STATUS1);
   //Retrieve current link state
   linkState = (value & ADIN1110_STATUS1_P1_LINK_STATUS) ? TRUE : FALSE;

   //Link up event?
   if(linkState && !interface->linkState)
   {
      //The PHY is only able to operate in 10 Mbps mode
      interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
      interface->duplexMode = NIC_FULL_DUPLEX_MODE;

      //Update link state
      interface->linkState = TRUE;

      //Process link state change event
      nicNotifyLinkChange(interface);
   }
   //Link down event?
   else if(!linkState && interface->linkState)
   {
      //Update link state
      interface->linkState = FALSE;

      //Process link state change event
      nicNotifyLinkChange(interface);
   }
#endif
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void adin1110EnableIrq(NetInterface *interface)
{
   //Enable interrupts
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->enableIrq();
   }
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void adin1110DisableIrq(NetInterface *interface)
{
   //Disable interrupts
   if(interface->extIntDriver != NULL)
   {
      interface->extIntDriver->disableIrq();
   }
}


/**
 * @brief ADIN1110 interrupt service routine
 * @param[in] interface Underlying network interface
 * @return TRUE if a higher priority task must be woken. Else FALSE is returned
 **/

bool_t adin1110IrqHandler(NetInterface *interface)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   //When the SPI host detects an asserted IRQn from the MACPHY, it should
   //initiate a data chunk transfer to obtain the current data footer
   interface->nicEvent = TRUE;

   //Notify the TCP/IP stack of the event
   return osSetEventFromIsr(&netEvent);
#else
   bool_t flag;
   size_t n;
   uint32_t mask;
   uint32_t status;

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Save interrupt mask register value
   mask = adin1110ReadReg(interface, ADIN1110_IMASK1);
   //Disable interrupts to release the interrupt line
   adin1110WriteReg(interface, ADIN1110_IMASK1, 0xFFFFFFFF);

   //Read interrupt status register
   status = adin1110ReadReg(interface, ADIN1110_STATUS1);

   //Link status changed interrupt?
   if((status & ADIN1110_STATUS1_LINK_CHANGE) != 0)
   {
      //Disable link status changed interrupt
      mask |= ADIN1110_IMASK1_LINK_CHANGE_MASK;

      //Set event flag
      interface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Packet transmission complete?
   if((status & ADIN1110_STATUS1_TX_RDY) != 0)
   {
      //Clear interrupt flag
      adin1110WriteReg(interface, ADIN1110_STATUS1, ADIN1110_STATUS1_TX_RDY);

      //The TX_SPACE register indicates the remaining space in the TX FIFO
      n = adin1110ReadReg(interface, ADIN1110_TX_SPACE) &
         ADIN1110_TX_SPACE_TX_SPACE;

      //Verify that there is space for a new frame
      if((n * 2) >= (ADIN1110_ETH_TX_BUFFER_SIZE + ADIN1110_TX_FRAME_OVERHEAD))
      {
         //Notify the TCP/IP stack that the transmitter is ready to send
         flag |= osSetEventFromIsr(&interface->nicTxEvent);
      }
   }

   //Packet received?
   if((status & ADIN1110_STATUS1_P1_RX_RDY) != 0)
   {
      //Disable P1_RX_RDY interrupt
      mask |= ADIN1110_IMASK1_P1_RX_RDY_MASK;

      //Set event flag
      interface->nicEvent = TRUE;
      //Notify the TCP/IP stack of the event
      flag |= osSetEventFromIsr(&netEvent);
   }

   //Re-enable interrupts once the interrupt has been serviced
   adin1110WriteReg(interface, ADIN1110_IMASK1, mask);

   //A higher priority task must be woken?
   return flag;
#endif
}


/**
 * @brief ADIN1110 event handler
 * @param[in] interface Underlying network interface
 **/

void adin1110EventHandler(NetInterface *interface)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   uint32_t status;

   //Read buffer status register
   status = adin1110ReadReg(interface, ADIN1110_BUFSTS);

   //Process all the data chunks currently available
   while((status & ADIN1110_BUFSTS_RCA) != 0)
   {
      //Read incoming packet
      adin1110ReceivePacket(interface);

      //Read buffer status register
      status = adin1110ReadReg(interface, ADIN1110_BUFSTS);
   }
#else
   uint32_t status;

   //When an interrupt occurs, the system can poll the MAC status registers
   //(STATUS0 and STATUS1) to determine the origin of the interrupt
   status = adin1110ReadReg(interface, ADIN1110_STATUS1);

   //Link status changed interrupt?
   if((status & ADIN1110_STATUS1_LINK_CHANGE) != 0)
   {
      //Clear interrupt flag
      adin1110WriteReg(interface, ADIN1110_STATUS1,
         ADIN1110_STATUS1_LINK_CHANGE);

      //Check link state
      if((status & ADIN1110_STATUS1_P1_LINK_STATUS) != 0)
      {
         //The PHY is only able to operate in 10 Mbps mode
         interface->linkSpeed = NIC_LINK_SPEED_10MBPS;
         interface->duplexMode = NIC_FULL_DUPLEX_MODE;

         //Link is up
         interface->linkState = TRUE;
      }
      else
      {
         //Link is down
         interface->linkState = FALSE;
      }

      //Process link state change event
      nicNotifyLinkChange(interface);
   }

   //Packet received?
   if((status & ADIN1110_STATUS1_P1_RX_RDY) != 0)
   {
      //Process all pending packets
      do
      {
         //Read incoming packet
         adin1110ReceivePacket(interface);

         //Read STATUS1 again
         status = adin1110ReadReg(interface, ADIN1110_STATUS1);

         //If the P1_RX_RDY bit is set, another frame is available to read
      } while((status & ADIN1110_STATUS1_P1_RX_RDY) != 0);
   }

   //Re-enable interrupts
   adin1110WriteReg(interface, ADIN1110_IMASK1, ~(ADIN1110_IMASK1_TX_RDY_MASK |
      ADIN1110_IMASK1_P1_RX_RDY_MASK | ADIN1110_IMASK1_LINK_CHANGE_MASK));
#endif
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @param[in] ancillary Additional options passed to the stack along with
 *   the packet
 * @return Error code
 **/

error_t adin1110SendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset, NetTxAncillary *ancillary)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   static uint8_t chunk[ADIN1110_CHUNK_PAYLOAD_SIZE + 4];
   size_t i;
   size_t j;
   size_t n;
   size_t length;
   uint32_t status;
   uint32_t header;
   uint32_t footer;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Read buffer status register
   status = adin1110ReadReg(interface, ADIN1110_BUFSTS);
   //Get the number of data chunks available in the transmit buffer
   n = (status & ADIN1110_BUFSTS_TXC) >> 8;

   //Check the number of transmit credits available
   if(length <= (n * ADIN1110_CHUNK_PAYLOAD_SIZE))
   {
      //A data transaction consists of multiple chunks
      for(i = 0; i < length; i += n)
      {
         //The default size of the data chunk payload is 64 bytes
         n = MIN(length - i, ADIN1110_CHUNK_PAYLOAD_SIZE);

         //Set up a data transfer
         header = ADIN1110_TX_HEADER_DNC | ADIN1110_TX_HEADER_NORX |
            ADIN1110_TX_HEADER_DV;

         //Start of packet?
         if(i == 0)
         {
            //The SPI host shall set the SV bit when the beginning of an
            //Ethernet frame is present in the current transmit data chunk
            //payload
            header |= ADIN1110_TX_HEADER_SV;
         }

         //End of packet?
         if((i + n) == length)
         {
            //The SPI host shall set the EV bit when the end of an Ethernet
            //frame is present in the current transmit data chunk payload
            header |= ADIN1110_TX_HEADER_EV;

            //When EV is 1, the EBO field shall contain the byte offset into
            //the transmit data chunk payload that points to the last byte of
            //the Ethernet frame to transmit
            header |= ((n - 1) << 8) & ADIN1110_TX_HEADER_EBO;
         }

         //The parity bit is calculated over the transmit data header
         if(adin1110CalcParity(header) != 0)
         {
            header |= ADIN1110_CTRL_HEADER_P;
         }

         //A chunk is composed of 4 bytes of overhead plus the configured
         //payload size
         STORE32BE(header, chunk);

         //Copy data chunk payload
         netBufferRead(chunk + 4, buffer, offset + i, n);

         //Pad frames shorter than the data chunk payload
         if(n < ADIN1110_CHUNK_PAYLOAD_SIZE)
         {
            osMemset(chunk + 4 + n, 0, ADIN1110_CHUNK_PAYLOAD_SIZE - n);
         }

         //Pull the CS pin low
         interface->spiDriver->assertCs();

         //Perform data transfer
         for(j = 0; j < (ADIN1110_CHUNK_PAYLOAD_SIZE + 4); j++)
         {
            chunk[j] = interface->spiDriver->transfer(chunk[j]);
         }

         //Terminate the operation by raising the CS pin
         interface->spiDriver->deassertCs();

         //Receive data chunks consist of the receive data chunk payload followed
         //by a 4-byte footer
         footer = LOAD32BE(chunk + ADIN1110_CHUNK_PAYLOAD_SIZE);

         //The RCA field indicates the number of receive data chunks available
         if((footer & ADIN1110_RX_FOOTER_RCA) != 0)
         {
            //Some data chunks are available for reading
            interface->nicEvent = TRUE;
            //Notify the TCP/IP stack of the event
            osSetEvent(&netEvent);
         }
      }
   }
   else
   {
      //No sufficient credits available
   }

   //The transmitter can accept another packet
   osSetEvent(&interface->nicTxEvent);

   //Successful processing
   return NO_ERROR;
#else
   static uint8_t temp[ADIN1110_ETH_TX_BUFFER_SIZE];
   size_t n;
   size_t length;

   //Retrieve the length of the packet
   length = netBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > ADIN1110_ETH_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return ERROR_INVALID_LENGTH;
   }

   //The TX_SPACE register indicates the remaining space in the TX FIFO
   n = adin1110ReadReg(interface, ADIN1110_TX_SPACE) &
      ADIN1110_TX_SPACE_TX_SPACE;

   //Ensure that there is sufficient space for the Ethernet frame plus 2-byte
   //header plus 2-byte size field
   if((n * 2) < (length + ADIN1110_TX_FRAME_OVERHEAD))
   {
      return ERROR_FAILURE;
   }

   //Copy user data
   netBufferRead(temp, buffer, offset, length);

   //TX_FSIZE is still written with the original frame size + 2 bytes for the
   //frame header
   adin1110WriteReg(interface, ADIN1110_TX_FSIZE, length +
      ADIN1110_FRAME_HEADER_SIZE);

   //Write frame data
   adin1110WriteFifo(interface, 0, temp, length);

   //The TX_SPACE register indicates the remaining space in the TX FIFO
   n = adin1110ReadReg(interface, ADIN1110_TX_SPACE) &
      ADIN1110_TX_SPACE_TX_SPACE;

   //Verify that there is space for a new frame
   if((n * 2) >= (ADIN1110_ETH_TX_BUFFER_SIZE + ADIN1110_TX_FRAME_OVERHEAD))
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
   }

   //Successful processing
   return NO_ERROR;
#endif
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t adin1110ReceivePacket(NetInterface *interface)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   static uint8_t buffer[ADIN1110_ETH_RX_BUFFER_SIZE];
   static uint8_t chunk[ADIN1110_CHUNK_PAYLOAD_SIZE + 4];
   error_t error;
   size_t i;
   size_t n;
   size_t length;
   uint32_t header;
   uint32_t footer;

   //Initialize variable
   length = 0;

   //A data transaction consists of multiple chunks
   while(1)
   {
      //Check the length of the received packet
      if((length + ADIN1110_CHUNK_PAYLOAD_SIZE) > ADIN1110_ETH_RX_BUFFER_SIZE)
      {
         error = ERROR_BUFFER_OVERFLOW;
         break;
      }

      //The SPI host sets NORX to 0 to indicate that it accepts and process
      //any receive frame data within the current chunk
      header = ADIN1110_TX_HEADER_DNC;

      //The parity bit is calculated over the transmit data header
      if(adin1110CalcParity(header) != 0)
      {
         header |= ADIN1110_CTRL_HEADER_P;
      }

      //Transmit data chunks consist of a 4-byte header followed by the
      //transmit data chunk payload,
      STORE32BE(header, chunk);

      //Clear data chunk payload
      osMemset(chunk + 4, 0, ADIN1110_CHUNK_PAYLOAD_SIZE);

      //Pull the CS pin low
      interface->spiDriver->assertCs();

      //Perform data transfer
      for(i = 0; i < (ADIN1110_CHUNK_PAYLOAD_SIZE + 4); i++)
      {
         chunk[i] = interface->spiDriver->transfer(chunk[i]);
      }

      //Terminate the operation by raising the CS pin
      interface->spiDriver->deassertCs();

      //Receive data chunks consist of the receive data chunk payload followed
      //by a 4-byte footer
      footer = LOAD32BE(chunk + ADIN1110_CHUNK_PAYLOAD_SIZE);

      //When the DV bit is 0, the SPI host ignores the chunk payload
      if((footer & ADIN1110_RX_FOOTER_DV) == 0)
      {
         error = ERROR_BUFFER_EMPTY;
         break;
      }

      //When the SV bit is 1, the beginning of an Ethernet frame is present in
      //the current transmit data chunk payload
      if(length == 0)
      {
         if((footer & ADIN1110_RX_FOOTER_SV) == 0)
         {
            error = ERROR_INVALID_PACKET;
            break;
         }
      }
      else
      {
         if((footer & ADIN1110_RX_FOOTER_SV) != 0)
         {
            error = ERROR_INVALID_PACKET;
            break;
         }
      }

      //When EV is 1, the EBO field contains the byte offset into the
      //receive data chunk payload that points to the last byte of the
      //received Ethernet frame
      if((footer & ADIN1110_RX_FOOTER_EV) != 0)
      {
         n = ((footer & ADIN1110_RX_FOOTER_EBO) >> 8) + 1;
      }
      else
      {
         n = ADIN1110_CHUNK_PAYLOAD_SIZE;
      }

      //Copy data chunk payload
      osMemcpy(buffer + length, chunk, n);
      //Adjust the length of the packet
      length += n;

      //When the EV bit is 1, the end of an Ethernet frame is present in the
      //current receive data chunk payload
      if((footer & ADIN1110_RX_FOOTER_EV) != 0)
      {
         NetRxAncillary ancillary;

         //Additional options can be passed to the stack along with the packet
         ancillary = NET_DEFAULT_RX_ANCILLARY;
         //Pass the packet to the upper layer
         nicProcessPacket(interface, buffer, length, &ancillary);

         //Successful processing
         error = NO_ERROR;
         break;
      }
   }

   //Return status code
   return error;
#else
   static uint8_t temp[ADIN1110_ETH_RX_BUFFER_SIZE];
   error_t error;
   size_t length;
   uint16_t header;

   //Get the size of the frame at the head of the RX FIFO in bytes
   length = adin1110ReadReg(interface, ADIN1110_P1_RX_FSIZE) &
      ADIN1110_P1_RX_FSIZE_P1_RX_FRM_SIZE;

   //Any packet pending in the receive buffer?
   if(length >= ADIN1110_FRAME_HEADER_SIZE)
   {
      NetRxAncillary ancillary;

      //The size of the frame includes the appended header
      length -= ADIN1110_FRAME_HEADER_SIZE;
      //Read frame data
      adin1110ReadFifo(interface, &header, temp, length);

      //Limit the length of the payload
      length = MIN(length, ADIN1110_ETH_RX_BUFFER_SIZE);
      //Additional options can be passed to the stack along with the packet
      ancillary = NET_DEFAULT_RX_ANCILLARY;

      //Pass the packet to the upper layer
      nicProcessPacket(interface, temp, length, &ancillary);

      //Successful processing
      error = NO_ERROR;
   }
   else
   {
      //The RX FIFO is empty
      error = ERROR_BUFFER_EMPTY;
   }

   //Return status code
   return error;
#endif
}


/**
 * @brief Configure MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t adin1110UpdateMacAddrFilter(NetInterface *interface)
{
   uint_t i;
   uint_t j;
   MacFilterEntry *entry;

   //Debug message
   TRACE_DEBUG("Updating MAC filter...\r\n");

   //Set the upper 16 bits of the broadcast MAC address
   adin1110WriteReg(interface, ADIN1110_ADDR_FILT_UPRn(0),
      ADIN1110_ADDR_FILT_UPR_APPLY2PORT1 | ADIN1110_ADDR_FILT_UPR_TO_HOST |
      ADIN1110_ADDR_FILT_UPR_MAC_ADDR_47_32);

   //Set the lower 32 bits of the broadcast MAC address
   adin1110WriteReg(interface, ADIN1110_ADDR_FILT_LWRn(0),
      ADIN1110_ADDR_FILT_LWR_MAC_ADDR_31_0);

   //Set the upper 16 bits of the station MAC address
   adin1110WriteReg(interface, ADIN1110_ADDR_FILT_UPRn(1),
      ADIN1110_ADDR_FILT_UPR_APPLY2PORT1 | ADIN1110_ADDR_FILT_UPR_TO_HOST |
      (interface->macAddr.b[0] << 8) | interface->macAddr.b[1]);

   //Set the lower 32 bits of the station MAC address
   adin1110WriteReg(interface, ADIN1110_ADDR_FILT_LWRn(1),
      (interface->macAddr.b[2] << 24) | (interface->macAddr.b[3] << 16) |
      (interface->macAddr.b[4] << 8) | interface->macAddr.b[5]);

   //The MAC address filter contains the list of MAC addresses to accept
   //when receiving an Ethernet frame
   for(i = 0, j = 2; i < MAC_ADDR_FILTER_SIZE &&
      j < ADIN1110_ADDR_TABLE_SIZE; i++)
   {
      //Point to the current entry
      entry = &interface->macAddrFilter[i];

      //Valid entry?
      if(entry->refCount > 0)
      {
         //Set the upper 16 bits of the current MAC address
         adin1110WriteReg(interface, ADIN1110_ADDR_FILT_UPRn(j),
            ADIN1110_ADDR_FILT_UPR_APPLY2PORT1 | ADIN1110_ADDR_FILT_UPR_TO_HOST |
            (entry->addr.b[0] << 8) | entry->addr.b[1]);

         //Set the lower 32 bits of the current MAC address
         adin1110WriteReg(interface, ADIN1110_ADDR_FILT_LWRn(j),
            (entry->addr.b[2] << 24) | (entry->addr.b[3] << 16) |
            (entry->addr.b[4] << 8) | entry->addr.b[5]);

         //Increment index
         j++;
      }
   }

   //Clear unused table entries
   for(; j < ADIN1110_ADDR_TABLE_SIZE; j++)
   {
      //Clear current MAC address
      adin1110WriteReg(interface, ADIN1110_ADDR_FILT_UPRn(j), 0);
      adin1110WriteReg(interface, ADIN1110_ADDR_FILT_LWRn(j), 0);
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write SPI register
 * @param[in] interface Underlying network interface
 * @param[in] address Register address
 * @param[in] data System register value
 **/

void adin1110WriteReg(NetInterface *interface, uint16_t address,
   uint32_t data)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   uint32_t header;

   //Set up a register write operation
   header = ADIN1110_CTRL_HEADER_WNR | ADIN1110_CTRL_HEADER_AID;

   //The MMS field selects the specific register memory map to access
   if(address < 0x30)
   {
      header |= (ADIN1110_MMS_STD << 24) & ADIN1110_CTRL_HEADER_MMS;
   }
   else
   {
      header |= (ADIN1110_MMS_MAC << 24) & ADIN1110_CTRL_HEADER_MMS;
   }

   //Address of the first register to access
   header |= (address << 8) & ADIN1110_CTRL_HEADER_ADDR;
   //Specifies the number of registers to write
   header |= (0 << 1) & ADIN1110_CTRL_HEADER_LEN;

   //The parity bit is calculated over the control command header
   if(adin1110CalcParity(header) != 0)
   {
      header |= ADIN1110_CTRL_HEADER_P;
   }

   //Pull the CS pin low
   interface->spiDriver->assertCs();

   //Write control command header
   interface->spiDriver->transfer((header >> 24) & 0xFF);
   interface->spiDriver->transfer((header >> 16) & 0xFF);
   interface->spiDriver->transfer((header >> 8) & 0xFF);
   interface->spiDriver->transfer(header & 0xFF);

   //Write data
   interface->spiDriver->transfer((data >> 24) & 0xFF);
   interface->spiDriver->transfer((data >> 16) & 0xFF);
   interface->spiDriver->transfer((data >> 8) & 0xFF);
   interface->spiDriver->transfer(data & 0xFF);

#if (ADIN1110_PROTECTION_SUPPORT == ENABLED)
   //Protection is accomplished by duplication of each 32-bit word containing
   //register data with its ones' complement
   data = ~data;

   //Write complement
   interface->spiDriver->transfer((data >> 24) & 0xFF);
   interface->spiDriver->transfer((data >> 16) & 0xFF);
   interface->spiDriver->transfer((data >> 8) & 0xFF);
   interface->spiDriver->transfer(data & 0xFF);
#endif

   //Send 32 bits of dummy data at the end of the control write command
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);

   //Terminate the operation by raising the CS pin
   interface->spiDriver->deassertCs();
#else
   //Pull the CS pin low
   interface->spiDriver->assertCs();

   //Write command
   interface->spiDriver->transfer(ADIN1110_SPI_CMD_WRITE | (address >> 8));
   interface->spiDriver->transfer(address & 0xFF);

   //Write data
   interface->spiDriver->transfer((data >> 24) & 0xFF);
   interface->spiDriver->transfer((data >> 16) & 0xFF);
   interface->spiDriver->transfer((data >> 8) & 0xFF);
   interface->spiDriver->transfer(data & 0xFF);

   //Terminate the operation by raising the CS pin
   interface->spiDriver->deassertCs();
#endif
}


/**
 * @brief Read SPI register
 * @param[in] interface Underlying network interface
 * @param[in] address System register address
 * @return Register value
 **/

uint32_t adin1110ReadReg(NetInterface *interface, uint16_t address)
{
#if (ADIN1110_OA_SPI_SUPPORT == ENABLED)
   uint32_t data;
   uint32_t header;

   //Set up a register read operation
   header = ADIN1110_CTRL_HEADER_AID;

   //The MMS field selects the specific register memory map to access
   if(address < 0x30)
   {
      header |= (ADIN1110_MMS_STD << 24) & ADIN1110_CTRL_HEADER_MMS;
   }
   else
   {
      header |= (ADIN1110_MMS_MAC << 24) & ADIN1110_CTRL_HEADER_MMS;
   }

   //Address of the first register to access
   header |= (address << 8) & ADIN1110_CTRL_HEADER_ADDR;
   //Specifies the number of registers to read
   header |= (0 << 1) & ADIN1110_CTRL_HEADER_LEN;

   //The parity bit is calculated over the control command header
   if(adin1110CalcParity(header) != 0)
   {
      header |= ADIN1110_CTRL_HEADER_P;
   }

   //Pull the CS pin low
   interface->spiDriver->assertCs();

   //Write control command header
   interface->spiDriver->transfer((header >> 24) & 0xFF);
   interface->spiDriver->transfer((header >> 16) & 0xFF);
   interface->spiDriver->transfer((header >> 8) & 0xFF);
   interface->spiDriver->transfer(header & 0xFF);

   //Discard the echoed control header
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);

   //Read data
   data = interface->spiDriver->transfer(0x00) << 24;
   data |= interface->spiDriver->transfer(0x00) << 16;
   data |= interface->spiDriver->transfer(0x00) << 8;
   data |= interface->spiDriver->transfer(0x00);

#if (ADIN1110_PROTECTION_SUPPORT == ENABLED)
   //Protection is accomplished by duplication of each 32-bit word containing
   //register data with its ones' complement
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
   interface->spiDriver->transfer(0x00);
#endif

   //Terminate the operation by raising the CS pin
   interface->spiDriver->deassertCs();

   //Return register value
   return data;
#else
   uint32_t data;

   //Pull the CS pin low
   interface->spiDriver->assertCs();

   //Write command
   interface->spiDriver->transfer(ADIN1110_SPI_CMD_READ | (address >> 8));
   interface->spiDriver->transfer(address & 0xFF);

   //Turn around
   interface->spiDriver->transfer(0x00);

   //Read data
   data = interface->spiDriver->transfer(0x00) << 24;
   data |= interface->spiDriver->transfer(0x00) << 16;
   data |= interface->spiDriver->transfer(0x00) << 8;
   data |= interface->spiDriver->transfer(0x00);

   //Terminate the operation by raising the CS pin
   interface->spiDriver->deassertCs();

   //Return register value
   return data;
#endif
}


/**
 * @brief Dump SPI registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void adin1110DumpReg(NetInterface *interface)
{
   uint16_t i;

   //Loop through system registers
   for(i = 0; i < 256; i++)
   {
      //Display current SPI register
      TRACE_DEBUG("0x%02" PRIX16 ": 0x%08" PRIX32 "\r\n", i,
         adin1110ReadReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}


/**
 * @brief Write PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @param[in] data Register value
 **/

void adin1110WritePhyReg(NetInterface *interface, uint8_t address,
   uint16_t data)
{
   uint32_t value;

   //Perform a Clause 22 write operation
   value = ADIN1110_MDIOACC_MDIO_ST_CLAUSE_22 | ADIN1110_MDIOACC_MDIO_OP_WRITE;
   //Set PHY address
   value |= ADIN1110_MDIOACC_MDIO_PRTAD_DEFAULT;
   //Set register address
   value |= (address << 16) & ADIN1110_MDIOACC_MDIO_DEVAD;
   //Set register value
   value |= data & ADIN1110_MDIOACC_MDIO_DATA;

   //Write MDIOACC0 register
   adin1110WriteReg(interface, ADIN1110_MDIOACC0, value);

   //Poll MDIOACC0.TRDONE to determine that the write operation has completed
   do
   {
      //Read MDIOACC0 register
      value = adin1110ReadReg(interface, ADIN1110_MDIOACC0);

      //When the MDIO transaction completes, the TRDONE bit is set to 1
   } while((value & ADIN1110_MDIOACC_MDIO_TRDONE) == 0);
}


/**
 * @brief Read PHY register
 * @param[in] interface Underlying network interface
 * @param[in] address PHY register address
 * @return Register value
 **/

uint16_t adin1110ReadPhyReg(NetInterface *interface, uint8_t address)
{
   uint32_t value;

   //Perform a Clause 22 read operation
   value = ADIN1110_MDIOACC_MDIO_ST_CLAUSE_22 | ADIN1110_MDIOACC_MDIO_OP_READ;
   //Set PHY address
   value |= ADIN1110_MDIOACC_MDIO_PRTAD_DEFAULT;
   //Set register address
   value |= (address << 16) & ADIN1110_MDIOACC_MDIO_DEVAD;

   //Write MDIOACC0 register
   adin1110WriteReg(interface, ADIN1110_MDIOACC0, value);

   //Poll MDIOACC0.TRDONE to determine that the read operation has completed
   do
   {
      //Read MDIOACC0 register
      value = adin1110ReadReg(interface, ADIN1110_MDIOACC0);

      //When the MDIO transaction completes, the TRDONE bit is set to 1
   } while((value & ADIN1110_MDIOACC_MDIO_TRDONE) == 0);

   //MDIOACC0.MDIO_DATA reflects the content of register
   return value & ADIN1110_MDIOACC_MDIO_DATA;
}


/**
 * @brief Dump PHY registers for debugging purpose
 * @param[in] interface Underlying network interface
 **/

void adin1110DumpPhyReg(NetInterface *interface)
{
   uint8_t i;

   //Loop through PHY registers
   for(i = 0; i < 32; i++)
   {
      //Display current PHY register
      TRACE_DEBUG("%02" PRIu8 ": 0x%04" PRIX16 "\r\n", i,
         adin1110ReadPhyReg(interface, i));
   }

   //Terminate with a line feed
   TRACE_DEBUG("\r\n");
}


/**
 * @brief Write MMD register
 * @param[in] interface Underlying network interface
 * @param[in] devAddr Device address
 * @param[in] regAddr Register address
 * @param[in] data MMD register value
 **/

void adin1110WriteMmdReg(NetInterface *interface, uint8_t devAddr,
   uint16_t regAddr, uint16_t data)
{
   uint32_t value;

   //Perform a Clause 45 address write operation
   value = ADIN1110_MDIOACC_MDIO_ST_CLAUSE_45 | ADIN1110_MDIOACC_MDIO_OP_ADDR;
   //MDIO_PRTAD is always written to 1
   value |= ADIN1110_MDIOACC_MDIO_PRTAD_DEFAULT;
   //Set device address
   value |= (devAddr << 16) & ADIN1110_MDIOACC_MDIO_DEVAD;
   //Set register address
   value |= regAddr & ADIN1110_MDIOACC_MDIO_DATA;

   //Write MDIOACC0 register
   adin1110WriteReg(interface, ADIN1110_MDIOACC0, value);

   //Perform a Clause 45 write operation
   value = ADIN1110_MDIOACC_MDIO_ST_CLAUSE_45 | ADIN1110_MDIOACC_MDIO_OP_WRITE;
   //MDIO_PRTAD is always written to 1
   value |= ADIN1110_MDIOACC_MDIO_PRTAD_DEFAULT;
   //Set device address
   value |= (devAddr << 16) & ADIN1110_MDIOACC_MDIO_DEVAD;
   //Set register value
   value |= data & ADIN1110_MDIOACC_MDIO_DATA;

   //Write MDIOACC1 register
   adin1110WriteReg(interface, ADIN1110_MDIOACC1, value);

   //Poll MDIOACC1.TRDONE to determine that the write operation has completed
   do
   {
      //Read MDIOACC1 register
      value = adin1110ReadReg(interface, ADIN1110_MDIOACC1);

      //When the MDIO transaction completes, the TRDONE bit is set to 1
   } while((value & ADIN1110_MDIOACC_MDIO_TRDONE) == 0);
}


/**
 * @brief Read MMD register
 * @param[in] interface Underlying network interface
 * @param[in] devAddr Device address
 * @param[in] regAddr Register address
 * @return MMD register value
 **/

uint16_t adin1110ReadMmdReg(NetInterface *interface, uint8_t devAddr,
   uint16_t regAddr)
{
   uint32_t value;

   //Perform a Clause 45 address write operation
   value = ADIN1110_MDIOACC_MDIO_ST_CLAUSE_45 | ADIN1110_MDIOACC_MDIO_OP_ADDR;
   //MDIO_PRTAD is always written to 1
   value |= ADIN1110_MDIOACC_MDIO_PRTAD_DEFAULT;
   //Set device address
   value |= (devAddr << 16) & ADIN1110_MDIOACC_MDIO_DEVAD;
   //Set register address
   value |= regAddr & ADIN1110_MDIOACC_MDIO_DATA;

   //Write MDIOACC0 register
   adin1110WriteReg(interface, ADIN1110_MDIOACC0, value);

   //Perform a Clause 45 read operation
   value = ADIN1110_MDIOACC_MDIO_ST_CLAUSE_45 | ADIN1110_MDIOACC_MDIO_OP_READ;
   //MDIO_PRTAD is always written to 1
   value |= ADIN1110_MDIOACC_MDIO_PRTAD_DEFAULT;
   //Set device address
   value |= (devAddr << 16) & ADIN1110_MDIOACC_MDIO_DEVAD;

   //Write MDIOACC1 register
   adin1110WriteReg(interface, ADIN1110_MDIOACC1, value);

   //Poll MDIOACC1.TRDONE to determine that the read operation has completed
   do
   {
      //Read MDIOACC1 register
      value = adin1110ReadReg(interface, ADIN1110_MDIOACC1);

      //When the MDIO transaction completes, the TRDONE bit is set to 1
   } while((value & ADIN1110_MDIOACC_MDIO_TRDONE) == 0);

   //MDIOACC1.MDIO_DATA reflects the content of register
   return value & ADIN1110_MDIOACC_MDIO_DATA;
}


/**
 * @brief Write TX FIFO
 * @param[in] interface Underlying network interface
 * @param[in] header Frame header
 * @param[in] data Pointer to the data being written
 * @param[in] length Number of data to write
 **/

void adin1110WriteFifo(NetInterface *interface, uint16_t header,
   const uint8_t *data, size_t length)
{
#if (ADIN1110_OA_SPI_SUPPORT == DISABLED)
   size_t i;

   //Pull the CS pin low
   interface->spiDriver->assertCs();

   //Write command
   interface->spiDriver->transfer(ADIN1110_SPI_CMD_WRITE | (ADIN1110_TX >> 8));
   interface->spiDriver->transfer(ADIN1110_TX & 0xFF);

   //The 2-byte frame header is appended to all transmitted frames. This always
   //precedes the frame data
   interface->spiDriver->transfer((header >> 8) & 0xFF);
   interface->spiDriver->transfer(header & 0xFF);

   //Write frame data
   for(i = 0; i < length; i++)
   {
      interface->spiDriver->transfer(data[i]);
   }

   //The burst write data must always be in multiples of 4 bytes
   for(; ((i + ADIN1110_FRAME_HEADER_SIZE) % 4) != 0; i++)
   {
      interface->spiDriver->transfer(0x00);
   }

   //Terminate the operation by raising the CS pin
   interface->spiDriver->deassertCs();
#endif
}


/**
 * @brief Read RX FIFO
 * @param[in] interface Underlying network interface
 * @param[out] header Frame header
 * @param[out] data Buffer where to store the incoming data
 * @param[in] length Number of data to read
 **/

void adin1110ReadFifo(NetInterface *interface, uint16_t *header,
   uint8_t *data, size_t length)
{
#if (ADIN1110_OA_SPI_SUPPORT == DISABLED)
   size_t i;

   //Pull the CS pin low
   interface->spiDriver->assertCs();

   //Write command
   interface->spiDriver->transfer(ADIN1110_SPI_CMD_READ | (ADIN1110_P1_RX >> 8));
   interface->spiDriver->transfer(ADIN1110_P1_RX & 0xFF);

   //Turn around
   interface->spiDriver->transfer(0x00);

   //The 2-byte frame header is appended to all received frames. This always
   //precedes the frame data
   *header = interface->spiDriver->transfer(0x00) << 16;
   *header |= interface->spiDriver->transfer(0x00);

   //Read frame data
   for(i = 0; i < length && i < ADIN1110_ETH_RX_BUFFER_SIZE; i++)
   {
      data[i] = interface->spiDriver->transfer(0x00);
   }

   //Discard extra bytes
   for(; i < length; i++)
   {
      interface->spiDriver->transfer(0x00);
   }

   //The burst read data must always be in multiples of 4 bytes
   for(; ((i + ADIN1110_FRAME_HEADER_SIZE) % 4) != 0; i++)
   {
      interface->spiDriver->transfer(0x00);
   }

   //Terminate the operation by raising the CS pin
   interface->spiDriver->deassertCs();
#endif
}


/**
 * @brief Calculate parity bit over a 32-bit data
 * @param[in] data 32-bit bit stream
 * @return Odd parity bit computed over the supplied data
 **/

uint32_t adin1110CalcParity(uint32_t data)
{
   //Calculate the odd parity bit computed over the supplied bit stream
   data ^= data >> 1;
   data ^= data >> 2;
   data ^= data >> 4;
   data ^= data >> 8;
   data ^= data >> 16;

   //Return '1' when the number of bits set to one in the supplied bit
   //stream is even (resulting in an odd number of ones when the parity is
   //included), otherwise return '0'
   return ~data & 0x01;
}
