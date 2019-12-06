#include "lpc17xx_uart.h"
#include "serial.h"
#include "dac.h"
#include "adc.h"
#include "pwm.h"
#include "keypad.h"

#include "sine.h"

void update_sine(uint32_t freq , uint32_t amp);

uint8_t stage = 0;
uint8_t key , last_key;

void stage2(void)
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

  if(((last_key = check_keypad()) != 0) && key != last_key)
    stage = 3;
  else
    last_key = key;
}

void stage3(void)
{
  uint32_t AdcData , last , dac_value;

  debug_to_serial("Hello ADC\n\r");

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

    if(((key = check_keypad()) != 0) && key != last_key)
      break;
    else
      last_key = key;
  }
}

void stage4(void)
{
  uint8_t tmp , match_value;

  debug_to_serial("Hello PWM\n\r");

  match_value = 0;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    update_match(match_value , tmp);
    match_value += 5;
  }
}

void stage5(void)
{
  static uint8_t match_value = 0;
  static uint8_t match_step = 1;
  static uint8_t count = 0 ;
  static uint32_t ticks = 0;
  uint8_t tmp;

  if((ticks++ % 100) == 0 && count < 5)
  {
    for(tmp = 1 ; tmp < 7 ; tmp++)
    {
      update_match(match_value , tmp);
      match_value += match_step;
      if(match_value == 0)
      {
	count++;
	debug_to_serial("Count : %u\n\r" , count);
      }
    }
  }
  else if(count >= 5)
  {
    if(((last_key = check_keypad()) != 0) && key != last_key)
    {
      stage = 2;
      count = 0;
    }
    else
    {
      last_key = key;
    }
  }
}

void SysTick_Handler(void)
{
  if(stage == 2)
  {
    stage2();
  }
  else if(stage == 3)
  {
    SYSTICK_Cmd(DISABLE);
    debug_to_serial("Disabled SYSTICK\n\r");
    stage3();
    debug_to_serial("Enabled SYSTICK\n\r");
    SYSTICK_Cmd(ENABLE);
    stage = 4;
  }
  else if(stage == 4)
  {
    stage4();
    stage = 5;
  }
  else if(stage == 5)
  {
    stage5();
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

  stage = 2;

  // milisecond granularity - 1kHz
  return_code = SysTick_Config(SystemCoreClock / 1000);
  if(return_code != 0)
    debug_to_serial("Error setting up SysTick interrupt.\n\r");

  while(1);
}
