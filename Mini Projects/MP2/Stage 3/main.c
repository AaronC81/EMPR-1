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

	uint8_t value , last , pos , row;
	pos = 0;
	row = ROW_1;
	while(1)
	{
	  if((value = check_keypad()) != 0x00)
	  {
	    if(last != value)
	    {
	      debug_to_serial("%02X\n\r" , value);
	      write_lcd_pos(value , row , pos);
	      pos = (pos + 1) & 0x0F;
	      if(!pos)
		row = row == ROW_1 ? ROW_2 : ROW_1;
	    }
	  }
	  last = value;
	}
	// shouldn't reach here !
	write_usb_serial_blocking("exit\n\r" , 6);
	while(1);
}
