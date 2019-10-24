// Serial code
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_gpio.h"
#include "serial.h"			// Local functions

// LED numbers
const uint32_t LED_0 = 1 << 18;
const uint32_t LED_1 = 1 << 20;
const uint32_t LED_2 = 1 << 21;
const uint32_t LED_3 = 1 << 23;

void int_to_4bit_bin(char* bin, uint32_t num)
{
	bin[4] = num & (1) ? '1' : '0';
	bin[3] = num & (1 << 1) ? '1' : '0';
	bin[2] = num & (1 << 2) ? '1' : '0';
	bin[1] = num & (1 << 3) ? '1' : '0';
}

void SysTick_Handler(void)
{
	static uint32_t count = -1;
	static uint32_t timer = 0;
	static char dec[5];
	static char hexa[2];
	static char bin[5];

	if (timer++ >= 10 && count++ <= 14)
	{
		GPIO_ClearValue(1, LED_0);
		GPIO_ClearValue(1, LED_1);
		GPIO_ClearValue(1, LED_2);
		GPIO_ClearValue(1, LED_3);

		sprintf(dec, "%02d |", count);
		sprintf(hexa, " %x |", count);
		char bin[8] = " xxxx\n\r";
		int_to_4bit_bin(bin, count);

		write_usb_serial_blocking(dec, 4);
                write_usb_serial_blocking(hexa, 4);
                write_usb_serial_blocking(bin, 8);

		if (count & 1) GPIO_SetValue(1, LED_0);
		if (count & 2) GPIO_SetValue(1, LED_1);
		if (count & 4) GPIO_SetValue(1, LED_2);
		if (count & 8) GPIO_SetValue(1, LED_3);

		timer = 0;
	}
}

// Entry point for the program
void main(void)
{
	serial_init();
	write_usb_serial_blocking("Starting count\n\r",16);
	GPIO_SetDir(1, 0xFFFFFFFF, 1);
	SysTick_Config(SystemCoreClock / 10);

	while(1);
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