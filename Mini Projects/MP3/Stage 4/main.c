#include "lpc17xx_uart.h"
#include "serial.h"
#include "pwm.h"

void SysTick_Handler(void)
{
  static uint8_t match_value = 1;
  static uint8_t match_step = 1;

  uint8_t tmp;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    update_match(match_value , tmp);
    match_value += match_step;
  }
}


void main(void)
{
  uint8_t tmp , match_value;
  uint32_t return_code;

  clear_serial();

  debug_to_serial("Hello PWM\n\r");

  init_pwm();

  match_value = 0;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    update_match(match_value , tmp);
    match_value += 5;
  }

  return_code = SysTick_Config(SystemCoreClock / 10);
  if(return_code != 0)
    debug_to_serial("Error setting up SysTick interrupt.\n\r");


  while(1);
}
