#include "lpc17xx_uart.h"
#include "serial.h"
#include "adc.h"

void main(void)
{
  uint32_t AdcData , last;
  clear_serial();

  debug_to_serial("Hello ADC\n\r");

  init_adc();

  last = 0;
  AdcData = 0;
  while(1)
  {
    AdcData = read_adc();
    if(AdcData != last)
      debug_to_serial("%u\n\r" , AdcData);
    last = AdcData;
  }

  while(1);
}
