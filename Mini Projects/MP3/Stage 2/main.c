#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_dac.h"
#include "LPC17xx.h"
#include "serial.h"

#include "sine.h"

#define DAC_MIN 0
#define DAC_MAX 0x3FF

volatile uint32_t ticks = 0;

void SysTick_Handler(void)
{
  static uint32_t dac_value;

  ticks += 1000;
  dac_value = sin_table[ticks & 0x3FF];
  DAC_UpdateValue(LPC_DAC , dac_value);
}


void main(void)
{
  uint32_t return_code;

  clear_serial();

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

  while(1);
}
