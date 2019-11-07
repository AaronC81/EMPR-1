// Serial code
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"			// Local functions

#define _GNU_SOURCE
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

uint8_t serial_initialised = 0;

void debug_to_serial(const char* format , ...)
{
  va_list args;
  char* test_string; // unallocated
  int len , result;

  if(!serial_initialised)
    serial_init();

  va_start(args , format);
  result = vasprintf(&test_string , format , args);
  va_end(args);

  if(result < 0)
    write_usb_serial_blocking("VASPRINTF ERROR!\n\r" , 18);

  len = strlen(test_string);
  if(len <= 0)
    write_usb_serial_blocking("STRLEN ERROR!\n\r" , 15);

  result = write_usb_serial_blocking(test_string , len);

  free(test_string);
}

void clear_serial(void)
{
  debug_to_serial("\033[2J",6);
}

// Read options
int read_usb_serial_none_blocking(char *buf,int length)
{
  if(!serial_initialised)
    serial_init();

  return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

// Write options
int write_usb_serial_blocking(char *buf,int length)
{
  if(!serial_initialised)
    serial_init();

  return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

// init code for the USB serial line
void serial_init(void)
{
	UART_CFG_Type UARTConfigStruct;			// UART Configuration structure variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
	PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	if(!serial_initialised)
	{
	  /*
	   * Initialize UART pin connect
	   */
	  PinCfg.Funcnum = 1;
	  PinCfg.OpenDrain = 0;
	  PinCfg.Pinmode = 0;
	  // USB serial first
	  PinCfg.Portnum = 0;
	  PinCfg.Pinnum = 2;
	  PINSEL_ConfigPin(&PinCfg);
	  PinCfg.Pinnum = 3;
	  PINSEL_ConfigPin(&PinCfg);

	  /* Initialize UART Configuration parameter structure to default state:
	   * - Baudrate = 9600bps
	   * - 8 data bit
	   * - 1 Stop bit
	   * - None parity
	   */
	  UART_ConfigStructInit(&UARTConfigStruct);
	  /* Initialize FIFOConfigStruct to default state:
	   * - FIFO_DMAMode = DISABLE
	   * - FIFO_Level = UART_FIFO_TRGLEV0
	   * - FIFO_ResetRxBuf = ENABLE
	   * - FIFO_ResetTxBuf = ENABLE
	   * - FIFO_State = ENABLE
	   */
	  UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	  // Built the basic structures, lets start the devices/
	  // USB serial
	  UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);		// Initialize UART0 peripheral with given to corresponding parameter
	  UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);	// Initialize FIFO for UART0 peripheral
	  UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);			// Enable UART Transmit

	  serial_initialised = 1;
	}
}
