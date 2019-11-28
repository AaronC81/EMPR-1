
/*
 * Cargo cult'd from
 * https://www-users.cs.york.ac.uk/~pcc/MCP/HAPR-Course-web/CMSIS/examples/html/rtc__alarm__cntincr__int_8c_source.html
 */

#include "lpc17xx_rtc.h"
#include "serial.h"
#include "rtc.h"

void init_rtc(void)
{
  RTC_Init(LPC_RTC);

  enable_rtc(RTC_DISABLE);

  NVIC_SetPriority(RTC_IRQn, ((0x01<<3)|0x01));

  RTC_ResetClockTickCounter(LPC_RTC);
  RTC_Cmd(LPC_RTC , RTC_ENABLE);
  RTC_CalibCounterCmd(LPC_RTC , RTC_DISABLE);

  // 10am 8/11/2019
  // see also : time.h
  RTC_SetTime(LPC_RTC , RTC_TIMETYPE_SECOND , 0);
  RTC_SetTime(LPC_RTC , RTC_TIMETYPE_MINUTE , 0);
  RTC_SetTime(LPC_RTC , RTC_TIMETYPE_HOUR , 13);
  RTC_SetTime(LPC_RTC , RTC_TIMETYPE_MONTH , 11);
  RTC_SetTime(LPC_RTC , RTC_TIMETYPE_YEAR , 2019);
  RTC_SetTime(LPC_RTC , RTC_TIMETYPE_DAYOFMONTH , 28);

  // second-level granularity
  RTC_SetAlarmTime(LPC_RTC , RTC_TIMETYPE_SECOND , 1);

  RTC_CntIncrIntConfig(LPC_RTC , RTC_TIMETYPE_SECOND , RTC_ENABLE);
  RTC_AlarmIntConfig(LPC_RTC , RTC_TIMETYPE_SECOND , RTC_ENABLE);
}

void enable_rtc(uint8_t enable)
{
  if(enable)
    NVIC_EnableIRQ(RTC_IRQn);
  else
    NVIC_DisableIRQ(RTC_IRQn);
}
