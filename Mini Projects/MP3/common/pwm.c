
/*
 * Cargo cult'd from https://www-users.cs.york.ac.uk/~pcc/MCP/HAPR-Course-web/CMSIS/examples/html/pwm__single__edge_8c_source.html
 */

#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"
#include "serial.h"
#include "pwm.h"

uint8_t pwm_initialised = 0;

void init_pwm(void)
{
  if(!pwm_initialised)
  {
    uint8_t tmp;
    PWM_MATCHCFG_Type PWMMatchCfgDat;

    PWM_TIMERCFG_Type PWMCfgDat;
    PWM_ConfigStructInit(PWM_MODE_TIMER , &PWMCfgDat);
    PWM_Init(LPC_PWM1 , PWM_MODE_TIMER , (void *) &PWMCfgDat);

    PINSEL_CFG_Type pwm_pin_cfg;
    pwm_pin_cfg.Funcnum = 1;
    pwm_pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    pwm_pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
    pwm_pin_cfg.Portnum = 2;
    for(tmp = 0 ; tmp < 7 ; tmp++)
    {
      pwm_pin_cfg.Pinnum = tmp;
      PINSEL_ConfigPin(&pwm_pin_cfg);
    }

    //

    PWM_MatchUpdate(LPC_PWM1 , 0 , 256, PWM_MATCH_UPDATE_NOW);

    PWMMatchCfgDat.IntOnMatch = DISABLE;
    PWMMatchCfgDat.MatchChannel = 0;
    PWMMatchCfgDat.ResetOnMatch = ENABLE;
    PWMMatchCfgDat.StopOnMatch = DISABLE;

    PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);
  
    for(tmp = 2 ; tmp < 7 ; tmp++)
      PWM_ChannelConfig(LPC_PWM1 , tmp , PWM_CHANNEL_SINGLE_EDGE);

    PWM_ResetCounter(LPC_PWM1);
    PWM_CounterCmd(LPC_PWM1 , ENABLE);

    PWM_Cmd(LPC_PWM1 , ENABLE);

    pwm_initialised = 1;
  }
}

void update_match(uint8_t match_value , uint8_t channel)
{
  PWM_MATCHCFG_Type PWMMatchCfgDat;

  if(!pwm_initialised)
    init_pwm();

  if(channel >= 1 && channel < 7)
  {
    PWM_MatchUpdate(LPC_PWM1 , channel , match_value , PWM_MATCH_UPDATE_NOW);
    PWMMatchCfgDat.IntOnMatch = DISABLE;
    PWMMatchCfgDat.MatchChannel = channel;
    PWMMatchCfgDat.ResetOnMatch = DISABLE;
    PWMMatchCfgDat.StopOnMatch = DISABLE;
    PWM_ConfigMatch(LPC_PWM1 , &PWMMatchCfgDat);
    PWM_ChannelCmd(LPC_PWM1 , channel , ENABLE);

    PWM_ResetCounter(LPC_PWM1);
  }
  else
  {
    debug_to_serial("Channel %u is out of range\n\r" , channel);
  }
}

