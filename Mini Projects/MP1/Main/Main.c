
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"
#include "led.h"

volatile uint32_t msTicks = 0;

void int_to_4bit_bin(char* bin, uint32_t num)
{
  bin[3] = num & (1) ? '1' : '0';
  bin[2] = num & (1 << 1) ? '1' : '0';
  bin[1] = num & (1 << 2) ? '1' : '0';
  bin[0] = num & (1 << 3) ? '1' : '0';
}

void SysTick_Handler(void)
{
  int returncode;
  static char dec[3];
  static char hex[3];
  static char bin[4];
  static uint8_t count = 0;

  msTicks++;

  if((msTicks % 10) == 0)
  {
    if(count == 0)
      write_usb_serial_blocking("Starting count\n\r",16);

    clear_all_leds();

    if(count & 1)
      enable_led(LED_0 , 1);
    if(count & 2)
      enable_led(LED_1 , 1);
    if(count & 4)
      enable_led(LED_2 , 1);
    if(count & 8)
      enable_led(LED_3 , 1);

    returncode = sprintf(dec , "%02d " , count);
    write_usb_serial_blocking(dec , returncode);
    returncode = sprintf(hex , "%02X " , count);
    write_usb_serial_blocking(hex , returncode);
    bin[4] = "xxxx";
    int_to_4bit_bin(bin , count);
    write_usb_serial_blocking(bin , 4);
    write_usb_serial_blocking("\n\r" , 2);
    count = (count + 1) & 0xF ;

    if(count == 0)
      write_usb_serial_blocking("Finished count\n\r\n\r",18);
  }
}

// Entry point for the program
void main(void)
{
  uint32_t returnCode;

  serial_init();
  write_usb_serial_blocking("Serial initialised\n\r\n\r",22);

  setup_leds();

  returnCode = SysTick_Config(SystemCoreClock / 10);

  if(returnCode)
    write_usb_serial_blocking("Error setting up error handling.\n\r",34);

  while(1);
}
