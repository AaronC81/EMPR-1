#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "LPC17xx.h"
#include "serial.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_rtc.h"
#include "rtc.h"

uint8_t counter = 0;
uint8_t match_value = 1;
uint8_t match_step = 1;
PWM_MATCHCFG_Type PWMMatchCfgDat;

void RTC_IRQHandler(void)
{
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);

  uint8_t tmp;
  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    PWM_MatchUpdate(LPC_PWM1 , tmp, match_value , PWM_MATCH_UPDATE_NOW);
    PWMMatchCfgDat.IntOnMatch = DISABLE;
    PWMMatchCfgDat.MatchChannel = tmp;
    PWMMatchCfgDat.ResetOnMatch = DISABLE;
    PWMMatchCfgDat.StopOnMatch = DISABLE;
    PWM_ConfigMatch(LPC_PWM1 , &PWMMatchCfgDat);
    PWM_ChannelCmd(LPC_PWM1 , tmp , ENABLE);
    match_value += match_step;
    debug_to_serial("Match value = %u\n\r" , match_value);
  }

  PWM_ResetCounter(LPC_PWM1);

  if((counter++ & 3) == 0)
    match_step++;

  if(match_step == 10)
    match_step = 1;

  debug_to_serial("Match step = %u\n\r" , match_step);
}


void main(void)
{
  uint8_t tmp;

  clear_serial();

  debug_to_serial("Hello PWM\n\r");

  PWM_TIMERCFG_Type PWMCfgDat;
  PWM_ConfigStructInit(PWM_MODE_TIMER , &PWMCfgDat);
  PWM_Init(LPC_PWM1 , PWM_MODE_TIMER , (void *) &PWMCfgDat);

  PINSEL_CFG_Type PinCfg;
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
  PinCfg.Portnum = 2;
  for(tmp = 0 ; tmp < 7 ; tmp++)
  {
    PinCfg.Pinnum = tmp;
    PINSEL_ConfigPin(&PinCfg);
  }

  PWM_MatchUpdate(LPC_PWM1 , 0 , 256, PWM_MATCH_UPDATE_NOW);

  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 0;
  PWMMatchCfgDat.ResetOnMatch = ENABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;

  PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);

  for(tmp = 2 ; tmp < 7 ; tmp++)
    PWM_ChannelConfig(LPC_PWM1 , tmp , PWM_CHANNEL_SINGLE_EDGE);

  for(tmp = 1 ; tmp < 7 ; tmp++)
  {
    PWM_MatchUpdate(LPC_PWM1 , tmp, match_value , PWM_MATCH_UPDATE_NOW);
    PWMMatchCfgDat.IntOnMatch = DISABLE;
    PWMMatchCfgDat.MatchChannel = tmp;
    PWMMatchCfgDat.ResetOnMatch = DISABLE;
    PWMMatchCfgDat.StopOnMatch = DISABLE;
    PWM_ConfigMatch(LPC_PWM1 , &PWMMatchCfgDat);
    PWM_ChannelCmd(LPC_PWM1 , tmp , ENABLE);
    match_value += 5;
  }

  PWM_ResetCounter(LPC_PWM1);
  PWM_CounterCmd(LPC_PWM1 , ENABLE);

  PWM_Cmd(LPC_PWM1 , ENABLE);

  init_rtc();
  enable_rtc(RTC_ENABLE);

  while(1);
}
