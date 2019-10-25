/* character set R */
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "stdio.h"
#include "serial.h"			// Local functions

uint8_t send[2] = {0,0};
uint8_t receive[1] = {0};
I2C_M_SETUP_Type setup =
{
  .tx_data = &send,
  .tx_length = 2,
  .tx_count = 0,
  .rx_data = receive,
  .rx_length = 0,
  .rx_count = 0,
  .retransmissions_max = 3,
  .retransmissions_count = 0,
  .status = 0
};

void pins_i2c(void)
{
  PINSEL_CFG_Type PinCfg;
  PinCfg.Funcnum = 3;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 0;
  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 1;
  PINSEL_ConfigPin(&PinCfg);
}

void init_i2c(void)
{
  pins_i2c();
  I2C_Init(LPC_I2C1, 100000);
  I2C_Cmd(LPC_I2C1, ENABLE);
}

void init_lcd(void)
{
  Status result;
  uint32_t index;
  const uint32_t init_bytes[10] = {0x34 , 0x0c , 0x06 , 0x35 , 0x04 , 0x10 , 0x42 , 0x9f , 0x34 , 0x02};
  setup.sl_addr7bit = 0x3b;
  send[0] = 0x00;
  for(index = 0 ; index < 10 ; index++)
  {
    send[1] = init_bytes[index];
    result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
  }
}

void clear_lcd(void)
{
  Status result;
  uint32_t tick;
  send[0] = 0x00;
  send[1] = 0x01;
  result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
  tick = 0;
  while(tick++ < 1024)
    ;
}

void write_lcd_pos(uint8_t character , uint8_t position)
{
  Status result;
  send[0] = 0x00;
  send[1] = 0x80 + position;
  result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);

  send[0] = 0x40;
  send[1] = character;
  result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
}

// Entry point for the program
void main(void)
{
  uint32_t position;
  int returncode;
  char result_string[11];

  serial_init();
  write_usb_serial_blocking("LCD\n\r" , 5);
  init_i2c();

  returncode = 0;

  init_lcd();
  clear_lcd();

  sprintf(result_string, "%02X %02X %02X\n\r", setup.sl_addr7bit , *setup.tx_data , *(setup.tx_data+1));
  write_usb_serial_blocking(result_string , 10);

  for(position = 0 ; position < 32 ; position++)
  {
    write_lcd_pos(0x14 , position);
    sprintf(result_string, "%02X %02X %02X\n\r", setup.sl_addr7bit , *setup.tx_data , *(setup.tx_data+1));
    write_usb_serial_blocking(result_string , 10);
  }

  write_usb_serial_blocking("End\n\r" , 5);

  while(1);
}
