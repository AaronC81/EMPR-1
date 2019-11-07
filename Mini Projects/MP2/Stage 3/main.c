#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "stdio.h"
#include "string.h"
#include "serial.h"
#include "i2c.h"
#include "lcd.h"
#include "keypad.h"

void main(void)
{	
	serial_init();
	clear_serial();

	init_i2c();
	init_lcd();
	init_keypad();

	clear_lcd();
	print_string("test", ROW_1);

	uint8_t value;
	while(1)
	{
	  if((value = check_keypad()) != 0x00)
	  {
	    debug_to_serial("%02X\n\r" , value);
	    write_lcd_pos(value , ROW_1 , 0);
	  }
	}
	// shouldn't reach here !
	write_usb_serial_blocking("exit\n\r" , 6);
	while(1);
}
