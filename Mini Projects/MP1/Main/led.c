
#include "lpc17xx_gpio.h"
#include "led.h"

void init_leds(void)
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

void enable_all_leds(uint8_t enable)
{
  enable_led(LED_0 , enable);
  enable_led(LED_1 , enable);
  enable_led(LED_2 , enable);
  enable_led(LED_3 , enable);
}
