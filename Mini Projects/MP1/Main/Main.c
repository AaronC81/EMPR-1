
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"
#include "led.h"

volatile uint32_t msTicks = 0;

void SysTick_Handler(void)
{
  int returncode;
  static char dec[3];
  static char hex[3];
  static char bin[4];
  static uint8_t count = 0;

  msTicks++;

  if((msTicks % 10) == 0 && count < 16)
  {
    if(count == 0)
      write_usb_serial_blocking("Starting count\n\r",16);

    bin[4] = "xxxx";

    enable_led(LED_0 , count & 1);
    bin[3] = (count & 1) ? '1' : '0' ;

    enable_led(LED_1 , count & 2);
    bin[2] = (count & 2) ? '1' : '0' ;

    enable_led(LED_2 , count & 4);
    bin[1] = (count & 4) ? '1' : '0';

    enable_led(LED_3 , count & 8);
    bin[0] = (count & 8) ? '1' : '0';

    returncode = sprintf(dec , "%02d " , count);
    write_usb_serial_blocking(dec , returncode);
    returncode = sprintf(hex , "%02X " , count);
    write_usb_serial_blocking(hex , returncode);
    write_usb_serial_blocking(bin , 4);
    write_usb_serial_blocking("\n\r" , 2);

    count++;

    if(count == 16)
      write_usb_serial_blocking("Finished count\n\r\n\r",18);
  }
}

// Entry point for the program
void main(void)
{
  uint32_t returnCode;

  serial_init();
  write_usb_serial_blocking("Serial initialised\n\r\n\r",22);

  init_leds();

  returnCode = SysTick_Config(SystemCoreClock / 10);

  if(returnCode)
    write_usb_serial_blocking("Error setting up error handling.\n\r",34);

  while(1);
}
