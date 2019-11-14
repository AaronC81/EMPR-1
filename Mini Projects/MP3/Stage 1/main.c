#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "serial.h"

void main(void)
{
	clear_serial();

	debug_to_serial("Hello ADC\n\r");

	while(1);
}
