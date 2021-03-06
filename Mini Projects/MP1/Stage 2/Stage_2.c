// Serial code
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"			// Local functions

// Entry point for the program
void main(void)
{
	uint32_t LED_0 = 1 << 18;
	uint32_t LED_1 = 1 << 20;
	uint32_t LED_2 = 1 << 21;
	uint32_t LED_3 = 1 << 23;

	uint32_t count, timer;
	uint32_t LED_array[4] = {LED_0, LED_1, LED_2, LED_3};

	serial_init();
	write_usb_serial_blocking("Stage 2 test code\n\r",19);
	GPIO_SetDir(1, 0xFFFFFFFF, 1);
	count = 0;

	while(1)
	{
		if (count & 1) GPIO_SetValue(1, LED_0);
		if (count & 2) GPIO_SetValue(1, LED_1);
		if (count & 4) GPIO_SetValue(1, LED_2);
		if (count & 8) GPIO_SetValue(1, LED_3);
		for(timer = 0 ; timer < 1 << 23 ; timer++)
			;
		GPIO_ClearValue(1, LED_0);
		GPIO_ClearValue(1, LED_1);
		GPIO_ClearValue(1, LED_2);
		GPIO_ClearValue(1, LED_3);
		count++;
	}
}

// Read options
int read_usb_serial_none_blocking(char *buf,int length)
{
	return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

// Write options
int write_usb_serial_blocking(char *buf,int length)
{
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}
// init code for the USB serial line
void serial_init(void)
{
	UART_CFG_Type UARTConfigStruct;			// UART Configuration structure variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
	PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
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
	
}
