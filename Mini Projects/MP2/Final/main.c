#include "lpc17xx_uart.h"		// Central include files
#include "lpc17xx_pinsel.h"
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "stdio.h"
#include "string.h"
#include "serial.h"
#include "i2c.h"
#include "lcd.h"
#include "keypad.h"
#include "lpc17xx_rtc.h"
#include "rtc.h"

void stage1(void);
void stage2(void);
void stage3(void);

uint8_t delay = 0;

void RTC_IRQHandler(void)
{
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
  if(RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
    RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);

  delay = 0;
}

void sleeper(void)
{
  enable_rtc(ENABLE);
  delay = 1;
  while(delay)
    ;
  enable_rtc(DISABLE);
}

void main(void)
{
	clear_serial();

	clear_lcd();

	init_rtc();

	stage1();

	// wait one second
	sleeper();

	stage2();

	// wait one second
	sleeper();

	stage3();

	// shouldn't reach here !
	debug_to_serial("exit\n\r");
	while(1);
}

void stage1(void)
{
  uint32_t count , address;
  uint8_t send[1] = {0};
  uint8_t receive[0] = {};
  Status result;

  I2C_M_SETUP_Type setup =
  {
    .tx_data = send,
    .tx_length = 1,
    .tx_count = 0,
    .rx_data = receive,
    .rx_length = 0,
    .rx_count = 0,
    .retransmissions_max = 3,
    .retransmissions_count = 0,
    .status = 0
  };

  count = 0;
  for(address = 0 ; address < 128 ; address++)
  {
    setup.sl_addr7bit = address;
    result = write_i2c(&setup);
    if (result)
    {
      debug_to_serial("Device found at address 0x%02x\n\r" , address);
      count++;
    }
  }

  debug_to_serial("%d devices connected to i2c bus.\n\r" , count);
}

void stage2(void)
{
  print_string("Hello" , ROW_1);

  // wait one second
  sleeper();

  clear_lcd();

  // wait one second
  sleeper();

  print_string("World" , ROW_2);

  // wait one second
  sleeper();

  clear_lcd();
}

void stage3(void)
{
  uint8_t value , last , pos , row;
  last = 0x00;
  pos = 0;
  row = ROW_1;
  while(1)
  {
    if((value = check_keypad()) != 0x00)
    {
      if(last != value)
      {
	write_lcd_pos(value , row , pos);
	pos = (pos + 1) & 0x0F;
	if(!pos)
	  row = row == ROW_1 ? ROW_2 : ROW_1;
      }
    }
    last = value;
  }
}
