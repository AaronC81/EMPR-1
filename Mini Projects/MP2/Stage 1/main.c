// Serial code
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
  uint32_t count , address;
  int returncode;
  char result_string[31];
  char device_string[22];
  uint8_t send[1] = {0};
  uint8_t receive[1] = {0};
  Status result;

  serial_init();
  pins_i2c();
  I2C_Init(LPC_I2C1, 100000);
  I2C_Cmd(LPC_I2C1, ENABLE);
  I2C_M_SETUP_Type setup =
  {
    .tx_data = send,
    .tx_length = 1,
    .tx_count = 0,
    .rx_data = receive,
    .rx_length = 0,
    .rx_count = 0,
    .retransmissions_max = 3,
    .retransmissions_count = 0,
    .status = 0
  };

  returncode = 0;
  count = 0;
  for(address = 0 ; address < 128 ; address++)
  {
    setup.sl_addr7bit = address;
    result = I2C_MasterTransferData(LPC_I2C1, &setup, I2C_TRANSFER_POLLING);
    if (result)
    {
      returncode = sprintf(result_string , "Device found at address 0x%02x\n\r" , address);
      write_usb_serial_blocking(result_string, 30);
      count++;
    }
  }

  returncode = sprintf(device_string , "%02X devices present.\n\r" , count);
  write_usb_serial_blocking(device_string , returncode);

  while(1);
}
