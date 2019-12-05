#include "lpc17xx_uart.h"
#include "serial.h"
#include "dac.h"
#include "adc.h"
#include "pwm.h"
#include "keypad.h"

#include "sine.h"

void update_sine(uint32_t freq , uint32_t amp);

uint8_t stage = 0;

void stage3(void)
{
  uint32_t AdcData , last , dac_value;

  debug_to_serial("Hello ADC\n\r");
  init_adc();

  last = 0;
  AdcData = 0;
  while(1)
  {
    AdcData = read_adc();
    if(AdcData != last)
    {
      dac_value = AdcData >> 2;
      write_dac(dac_value);
    }
    last = AdcData;
    /*
    if(check_keypad() != 0)
      break;
    */
  }
  stage++;
}

void stage4(void)
{
  uint8_t tmp , match_value;

  debug_to_serial("Hello PWM\n\r");
  init_pwm();

  match_value = 0;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    update_match(match_value , tmp);
    match_value += 5;
  }
}

void SysTick_Handler(void)
{
  static uint32_t ticks = 0;
  static uint32_t amp = 10;
  static uint32_t freq = 0;
  uint8_t tmp;
  static uint8_t match_value = 200;
  static uint8_t match_step = 1;

  if(stage == 2)
  {
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

    if(check_keypad() !=0)
      stage = 4;
  }
  else if(stage == 3)
  {
    SYSTICK_Cmd(DISABLE);
    debug_to_serial("Disabled SYSTICK\n\r");
    stage3();
    debug_to_serial("Enabled SYSTICK\n\r");
    SYSTICK_Cmd(ENABLE);
  }
  else if(stage == 4)
  {
    stage4();
    stage = 5;
  }
  else if(stage == 5)
  {
    if((ticks++ % 100) == 0)
    {
      for(tmp = 1 ; tmp < 7 ; tmp++)
      {
	update_match(match_value , tmp);
	match_value += match_step;
	if(match_value == 240)
	  match_value = 208;
	debug_to_serial("Match value %u\n\r" , match_value);
      }
    }
  }
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

  stage = 4;

  // milisecond granularity - 1kHz
  return_code = SysTick_Config(SystemCoreClock / 1000);
  if(return_code != 0)
    debug_to_serial("Error setting up SysTick interrupt.\n\r");

  while(1);
}
