
void clear_lcd(void);
void write_lcd_pos(uint8_t character , uint8_t row , uint8_t column);
uint8_t ascii_to_r(char letter);
void print_string(char* string , uint8_t row);

static const uint32_t lcd_init_bytes[10] =
{0x34 , 0x0c , 0x06 , 0x35 , 0x04 , 0x10 , 0x42 , 0x9f , 0x34 , 0x02};

#define ROW_1 0
#define ROW_2 1
