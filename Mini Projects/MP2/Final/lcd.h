void init_lcd(void);
void clear_lcd(void);
void write_lcd_pos(uint8_t character , uint8_t row , uint8_t column);
uint8_t ascii_to_r(char letter);
void print_string(char* string , uint8_t row);

#define ROW_1 0
#define ROW_2 1
