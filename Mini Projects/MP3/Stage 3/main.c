#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_adc.h"
#include "LPC17xx.h"
#include "serial.h"

#define DAC_MIN 0
#define DAC_MAX 0x3FF

void main(void)
{
  uint32_t AdcData , last , dac_value;

  clear_serial();

  // DAC
  debug_to_serial("Hello DAC\n\r");

  PINSEL_CFG_Type PinCfg;
  PinCfg.Funcnum = 2;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 26; // P18
  PINSEL_ConfigPin(&PinCfg);

  DAC_Init(LPC_DAC);

  // ADC
  debug_to_serial("Hello ADC\n\r");

  //PINSEL_CFG_Type PinCfg;
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 23; // *should* be P15 ?!
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
      {
	//	debug_to_serial("%u\n\r" , AdcData);
	dac_value = AdcData >> 2;
	DAC_UpdateValue(LPC_DAC , dac_value);
      }
      last = AdcData;
    }
  }

  while(1);
}
