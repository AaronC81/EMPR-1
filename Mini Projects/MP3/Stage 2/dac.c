
#include "lpc17xx_pinsel.h"
#include "lpc17xx_dac.h"
#include "dac.h"

void init_dac(void)
{
  PINSEL_CFG_Type dac_pin_cfg;
  dac_pin_cfg.Funcnum = 2;
  dac_pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  dac_pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;
  dac_pin_cfg.Portnum = 0;
  dac_pin_cfg.Pinnum = 26; // P18
  PINSEL_ConfigPin(&dac_pin_cfg);

  DAC_Init(LPC_DAC);
}

void write_dac(uint32_t value)
{
  DAC_UpdateValue(LPC_DAC , value);
}
