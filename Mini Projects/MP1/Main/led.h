
#define DIR_INPUT 0
#define DIR_OUTPUT 1
#define LED_PORT 1

#define LED_0 (1 << 18)
#define LED_1 (1 << 20)
#define LED_2 (1 << 21)
#define LED_3 (1 << 23)

void init_leds(void);

void enable_led(uint32_t led , uint8_t enable);

void enable_all_leds(uint8_t enable);
