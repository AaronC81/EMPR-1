#include "lpc17xx_uart.h"
#include "serial.h"
#include "adc.h"
#include "dac.h"

void main(void)
{
  uint32_t AdcData , last , dac_value;

  clear_serial();

  // DAC
  debug_to_serial("Hello DAC\n\r");

  init_dac();

  // ADC
  debug_to_serial("Hello ADC\n\r");

  init_adc();

  last = 0;
  AdcData = 0;
  while(1)
  {
    AdcData = read_adc();
    if(AdcData != last)
    {
      //	debug_to_serial("%u\n\r" , AdcData);
      dac_value = AdcData >> 2;
      write_dac(dac_value);
    }
    last = AdcData;
  }

  while(1);
}
