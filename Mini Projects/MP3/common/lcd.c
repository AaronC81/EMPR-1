/* character set R pg.23/86 */
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "stdio.h"
#include "string.h"
#include "serial.h"
#include "i2c.h"
#include "lcd.h"

uint8_t lcd_initialised = 0;

uint8_t l_send[2] = {0,0};
uint8_t l_receive[0] = {};
I2C_M_SETUP_Type l_setup =
{
  .sl_addr7bit = 0x3b,
  .tx_data = l_send,
  .tx_length = 2,
  .tx_count = 0,
  .rx_data = l_receive,
  .rx_length = 0,
  .rx_count = 0,
  .retransmissions_max = 3,
  .retransmissions_count = 0,
  .status = 0
};

void init_lcd(void)
{
  Status result;
  uint32_t index;

  if(!lcd_initialised)
  {
    l_send[0] = 0x00;
    for(index = 0 ; index < 10 ; index++)
    {
      l_send[1] = lcd_init_bytes[index];
      result = write_i2c(&l_setup);
      if(!result)
	debug_to_serial("Error initialising LCD\n\r");
    }
    lcd_initialised = 1;
  }
}

void clear_lcd(void)
{
  /*
   * 10.2.2.1 Clear_display - pg. 32/86 
   * When using character set ‘R’, the following alternative instruction set has to be used:
   * 1.  Switch display off (Display_ctl, bit D = 0).
   * 2.  Write a blank pattern into all DDRAM addresses (Write_data).
   * 3.  Switch display on (Display_ctl, bit D = 1).
   *
   * 10.2.2.4 Display_ctl - pg. 33/86
   * control byte = 0
   * command byte = 0x08 (off) 0x0C (on)
   *
   * blank patterns = 0xA0, 0x91, 0x9B
   */
  uint32_t column;
  // Write blank pattern into all DDRAM addresses
  for(column = 0 ; column < 0x10 ; column++)
    write_lcd_pos(0xA0 , ROW_1 , column);
  for(column = 0 ; column < 0x10 ; column++)
    write_lcd_pos(0xA0 , ROW_2 , column);
}

void write_lcd_pos(uint8_t character , uint8_t row , uint8_t column)
{
  Status result;
  uint8_t offset;

  if(!lcd_initialised)
    init_lcd();

  offset = row ? 0x40 : 0x00;

  // select instruction register
  l_send[0] = 0x00;
  // set position in DDRAM
  l_send[1] = 0x80 + column + offset;
  result = write_i2c(&l_setup);
  if(!result)
    debug_to_serial("Error setting LCD position.\n\r");

  // select data register
  l_send[0] = 0x40;
  // set the character in DDRAM
  l_send[1] = character;
  result = write_i2c(&l_setup);
  if(!result)
    debug_to_serial("Error writing character to LCD.\n\r");
}

uint8_t ascii_to_r( char letter )
{
  // UPPERCASE || lowercase
  if( (letter > 0x40 && letter < 0x5B) || (letter > 0x60 && letter < 0x7B) )
    return (uint8_t)(letter + 0x80);
  else // SPACE
    return 0xA0;
}

void print_string( char* string , uint8_t row )
{
  int column;
  size_t length = strlen(string);
  for(column = 0 ; column < length ; column++)
    write_lcd_pos(ascii_to_r(string[column]) , row , column);
  for( ; column < 0x10 ; column++)
    write_lcd_pos(0xA0 , row , column);
}
