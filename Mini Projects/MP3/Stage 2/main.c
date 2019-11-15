#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_rtc.h"
#include "serial.h"
#include "rtc.h"

#define DAC_MIN 0
#define DAC_MAX 0x3FF

void RTC_IRQHandler(void)
{
  static uint32_t dac_value;
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);

  dac_value = (dac_value == DAC_MIN) ? DAC_MAX : DAC_MIN;
  DAC_UpdateValue(LPC_DAC , dac_value);
  debug_to_serial("Writing 0x%X to DAC\n\r" , dac_value);
}


void main(void)
{
  clear_serial();

  debug_to_serial("Hello DAC\n\r");

  init_rtc();

  PINSEL_CFG_Type PinCfg;
  PinCfg.Funcnum = 2;
  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
  PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
  PinCfg.Portnum = 0;
  PinCfg.Pinnum = 26;
  PINSEL_ConfigPin(&PinCfg);

  DAC_Init(LPC_DAC);
  enable_rtc(ENABLE);
  
  while(1);
}
