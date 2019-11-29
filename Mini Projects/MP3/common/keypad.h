
uint8_t check_column(uint8_t column);

/*
 * LUT[ROW][COLUMN]
 * Where ROW is bottow to top,
 * and COLUMN is left to right,
 * with zero-based indexing (as it's C).
 */
static const uint8_t LUT[4][4] =
{
	{
		0xAA , // *
		0xB0 , // 0
		0xA3 , // #
		0xC4 , // D
	} ,
	{
		0xB7 , // 7
		0xB8 , // 8
		0xB9 , // 9
		0xC3 , // C
	} ,
	{
		0xB4 , // 4
		0xB5 , // 5
		0xB6 , // 6
		0xC2 , // B
	} ,
	{
		0xB1 , // 1
		0xB2 , // 2
		0xB3 , // 3
		0xC1 , // A
	}
};

uint8_t check_keypad(void);
