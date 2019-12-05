/* Set oscilloscope to 10ms */

#include "lpc17xx_uart.h"
#include "serial.h"
#include "dac.h"

#include "sine.h"

void update_sine(uint32_t freq , uint32_t amp);

void SysTick_Handler(void)
{
  static uint32_t ticks = 0;
  static uint32_t amp = 10;
  static uint32_t freq = 0;

  if((ticks % (50 * 1000) == 0))
  {
    freq = freq < 500 ? freq + 100 : 100;
    debug_to_serial("Frequency : %u Hz\n\r" , freq);
  }

  if((ticks % (5 * 1000) == 0))
  {
    amp = (amp + 1) % 10;
    debug_to_serial("Amplitude : %u\n\r" , amp);
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

  write_dac(dac_value);
}


void main(void)
{
  uint32_t return_code;

  clear_serial();

  debug_to_serial("Hello DAC\n\r");

  init_dac();

  // milisecond granularity - 1kHz
  return_code = SysTick_Config(SystemCoreClock / 1000);
  if(return_code != 0)
    debug_to_serial("Error setting up SysTick interrupt.\n\r");

  while(1);
}
