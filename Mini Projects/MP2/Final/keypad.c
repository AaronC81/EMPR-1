/* character set R */
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "stdio.h"
#include "string.h"
#include "serial.h"
#include "i2c.h"
#include "keypad.h"

uint8_t k_send;
uint8_t k_receive;
I2C_M_SETUP_Type k_setup =
{
  .sl_addr7bit = 0x21,
  .tx_data = &k_send,
  .tx_count = 0,
  .rx_data = &k_receive,
  .rx_count = 0,
  .retransmissions_max = 3,
  .retransmissions_count = 0,
  .status = 0
};

void init_keypad(void)
{
	k_send = 0x0F;
	write_i2c(&k_setup);
}

uint8_t lookup(uint8_t column)
{
	uint8_t result = 0;
	uint8_t tmp = ~k_receive & 0x0F;
	while(tmp != 0)
	{
		tmp >>= 1;
		result++;
	}
	return LUT[result - 1][4 - column];
}

uint8_t check_column(uint8_t column)
{
	k_setup.rx_length = 0;
	k_setup.tx_length = 1;	
	k_send = ~(1 << (column + 3));
	write_i2c(&k_setup);
	k_setup.rx_length = 1;
	k_setup.tx_length = 0;
	write_i2c(&k_setup);
	uint8_t result = 0;
	uint8_t tmp = ~k_receive & 0x0F;
	if(tmp != 0)
		result = lookup(column);
	return result;
}

uint8_t check_keypad(void)
{
  uint8_t i , result , tmp;
  result = 0;
  for(i = 1 ; i < 5 ; i++)
  {
    if((tmp = check_column(i)) != 0)
    {
      result = tmp;
      break;
    }
  }
  return result;
}
