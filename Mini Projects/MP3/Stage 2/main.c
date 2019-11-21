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
