/* character set R */
#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "stdio.h"
#include "serial.h"
#include "i2c.h"

uint8_t i2c_initialised = 0;

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
  if(!i2c_initialised)
  {
    pins_i2c();
    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);
    i2c_initialised = 1;
  }
}

void write_i2c(I2C_M_SETUP_Type* setup)
{
  Status result;
  if(!i2c_initialised)
    init_i2c();

  result = I2C_MasterTransferData(LPC_I2C1, setup, I2C_TRANSFER_POLLING);
  if(!result)
    debug_to_serial("I2C ERROR!\n\r");
}
