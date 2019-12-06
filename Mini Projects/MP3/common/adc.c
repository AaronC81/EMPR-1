#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_adc.h"
#include "adc.h"

uint8_t adc_initialised = 0;

void init_adc(void)
{
  if(!adc_initialised)
  {
    PINSEL_CFG_Type adc_pin_cfg;
    adc_pin_cfg.Funcnum = 1;
    adc_pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    adc_pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;
    adc_pin_cfg.Portnum = 0;
    adc_pin_cfg.Pinnum = 23; // *should* be P15 ?!
    PINSEL_ConfigPin(&adc_pin_cfg);

    CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCAD, ENABLE);
    ADC_Init(LPC_ADC, 100000);
    ADC_ChannelCmd(LPC_ADC, 0, ENABLE);
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);

    adc_initialised = 1;
  }
}

uint32_t read_adc(void)
{
  uint32_t adc_data;

  if(!adc_initialised)
    init_adc();

  adc_data = 0;

  adc_data = ADC_GlobalGetData(LPC_ADC);
  if(adc_data & (1UL << 31))
  {
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);
    adc_data &= 0xFFF;
  }

  return adc_data;
}
