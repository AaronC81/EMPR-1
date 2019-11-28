#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_adc.h"
#include "LPC17xx.h"
#include "serial.h"

#include "sine.h"

#define DAC_MIN 0
#define DAC_MAX 0x3FF

void update_sine(uint32_t freq , uint32_t amp);

void SysTick_Handler(void)
{
  static uint32_t ticks = 0;
  static uint32_t amp = 10;
  static uint32_t freq = 10;

  if((ticks % (5 * 1000) == 0))
  {
    amp = (amp + 1) % 10;
    debug_to_serial("Amplitude : %u\n\r" , amp);
  }

  if((ticks % (50 * 1000) == 0))
  {
    freq = freq < 10000 ? freq * 10 : 10;
    debug_to_serial("Frequency : %u Hz\n\r" , freq);
  }

  ticks++;
  update_sine(freq , amp);
}

void update_sine(uint32_t freq , uint32_t amp)
{
  static uint32_t ticks = 0;
  static uint32_t dac_value;

  ticks += freq;
  amp = amp > N_AMPLITUDES ? N_AMPLITUDES : amp;
  dac_value = (amp != 0) ? sin_table[--amp][ticks & (N_SAMPLES - 1)] : 0;

  DAC_UpdateValue(LPC_DAC , dac_value);
}


void main(void)
{
  uint32_t return_code;
  uint32_t AdcData , last;

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

  // milisecond granularity - maximum rate for DAC
  return_code = SysTick_Config(SystemCoreClock / 1000);
  if(return_code != 0)
    debug_to_serial("Error setting up SysTick interrupt.\n\r");


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
	debug_to_serial("%u\n\r" , AdcData);
      last = AdcData;
    }
  }

  while(1);
}
