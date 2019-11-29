#include "lpc17xx_uart.h"
#include "serial.h"
#include "lpc17xx_rtc.h"
#include "rtc.h"

void RTC_IRQHandler(void)
{
  static uint8_t counter = 0;
  static uint8_t match_value = 1;
  static uint8_t match_step = 1;

  if(RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);

  uint8_t tmp;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    update_match(match_value , tmp);
    match_value += match_step;
    debug_to_serial("Match value = %u\n\r" , match_value);
  }

  if((counter++ & 3) == 0)
    match_step++;

  if(match_step == 10)
    match_step = 1;

  debug_to_serial("Match step = %u\n\r" , match_step);
}


void main(void)
{
  uint8_t tmp , match_value;

  clear_serial();

  debug_to_serial("Hello PWM\n\r");

  init_pwm();

  match_value = 0;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    update_match(match_value , tmp);
    match_value += 5;
  }

  init_rtc();
  enable_rtc(RTC_ENABLE);

  while(1);
}
