/* character set R */
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "serial.h"			// Local functions

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

// Entry point for the program
void main(void)
{
  uint32_t count , tick , index , position;
  int returncode;
  char result_string[11];
  char device_string[22];
  uint8_t send[2] = {0,0};
  uint8_t receive[1] = {0};
  uint32_t address = 0x3b;
  uint32_t init_bytes[10] = {0x34 , 0x0c , 0x06 , 0x35 , 0x04 , 0x10 , 0x42 , 0x9f , 0x34 , 0x02};
  Status result;

  serial_init();
  write_usb_serial_blocking("LCD\n\r" , 5);
  pins_i2c();
  I2C_Init(LPC_I2C1, 100000);
  I2C_Cmd(LPC_I2C1, ENABLE);
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

  returncode = 0;

  setup.sl_addr7bit = address;

  send[0] = 0x00;
  for(index = 0 ; index < 10 ; index++)
  {
    send[1] = init_bytes[index];
    result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
  }

  send[1] = 0x01;
  result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
  while(tick++ < 1024)
    ;
  sprintf(result_string, "%02X %02X %02X\n\r", setup.sl_addr7bit , *setup.tx_data , *(setup.tx_data+1));
  write_usb_serial_blocking(result_string , 10);

  for(position = 0 ; position < 32 ; position++)
  {
    send[0] = 0x00;
    send[1] = 0x80 + position;
    result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
    sprintf(result_string, "%02X %02X %02X\n\r", setup.sl_addr7bit , *setup.tx_data , *(setup.tx_data+1));
    write_usb_serial_blocking(result_string , 10);

    send[0] = 0x40;
    send[1] = 0x14 ; //0x64;
    result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
    sprintf(result_string, "%02X %02X %02X\n\r", setup.sl_addr7bit , *setup.tx_data , *(setup.tx_data+1));
    write_usb_serial_blocking(result_string , 10);
  }

  write_usb_serial_blocking("End\n\r" , 5);

  while(1);
}
