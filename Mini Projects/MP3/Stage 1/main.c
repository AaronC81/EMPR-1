#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_adc.h"
#include "serial.h"

void main(void)
{
  uint32_t AdcData , last;
  clear_serial();

  debug_to_serial("Hello ADC\n\r");

  PINSEL_CFG_Type PinCfg;
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 23;
  PINSEL_ConfigPin(&PinCfg);

  CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCAD, ENABLE);
  ADC_Init(LPC_ADC, 100000);
  ADC_ChannelCmd(LPC_ADC, 0, ENABLE);
  ADC_StartCmd(LPC_ADC, ADC_START_NOW);

  last = 0;
  AdcData = 0;
  while(1)
  {
    AdcData = ADC_GlobalGetData(LPC_ADC);
    if(AdcData & (1UL << 31))
    {
      ADC_StartCmd(LPC_ADC, ADC_START_NOW);
      AdcData &= 0xFFF;
      if(AdcData != last)
	debug_to_serial("%u\n\r" , AdcData);
      last = AdcData;
    }
  }

  while(1);
}
