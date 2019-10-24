
#include "lpc17xx_gpio.h"
#include "led.h"

void setup_leds(void)
{
  GPIO_SetDir(LED_PORT , 0xFFFFFFFF , DIR_OUTPUT);
}

void enable_led(uint32_t led , uint8_t enable)
{
  if(enable)
    GPIO_SetValue(LED_PORT , led);
  else
    GPIO_ClearValue(LED_PORT , led);
}

void clear_all_leds(void)
{
  enable_led(LED_0 , 0);
  enable_led(LED_1 , 0);
  enable_led(LED_2 , 0);
  enable_led(LED_3 , 0);
}
