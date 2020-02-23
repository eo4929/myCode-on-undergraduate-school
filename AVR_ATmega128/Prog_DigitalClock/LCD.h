
/* LCD 사용 포트를 지정한다. */
#define LCD_DCTRL	(DDRG)
#define LCD_DDATA	(DDRD)
#define LCD_CTRL	(PORTG)
#define LCD_DATA	(PORTD)

#define LCD_RS 		(PG0)
#define LCD_RW 		(PG1)
#define LCD_E 		(PG2)
#define LCD_DB0 	(PD0)
#define LCD_DB1 	(PD1)
#define LCD_DB2 	(PD2)
#define LCD_DB3 	(PD3)
#define LCD_DB4 	(PD4)
#define LCD_DB5 	(PD5)
#define LCD_DB6 	(PD6)
#define LCD_DB7 	(PD7)


void lcd_control(unsigned char data) 
{
	LCD_CTRL = LCD_CTRL & ~_BV(LCD_RW);
	LCD_CTRL = LCD_CTRL & ~_BV(LCD_RS);
	LCD_DATA = data;
	LCD_CTRL = LCD_CTRL | _BV(LCD_E);
	_delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~_BV(LCD_E);
} 


void lcd_init(void) 
{ 
	LCD_DDATA = 0xFF;
	LCD_DCTRL = 0xFF;
	LCD_DATA = 0;
	LCD_CTRL = 0;
	_delay_ms(1);

	lcd_control(0x38);  // Function Set (8bit, 2line, 5 x 7 dot 
	lcd_control(0x0C);  // Display ON, Cursor OFF
	lcd_control(0x06);  // Entry Mode Set (increment, not shift)
	lcd_control(0x01);  // Clear Display  	
	lcd_control(0x00);  // Dummy
	
}


void lcd_write_char(unsigned char data) 
{
	LCD_CTRL = LCD_CTRL & ~_BV(LCD_RW);
	LCD_CTRL = LCD_CTRL | _BV(LCD_RS);
	LCD_DATA = data;
	LCD_CTRL = LCD_CTRL | _BV(LCD_E);
	_delay_ms(1);
	LCD_CTRL = LCD_CTRL & ~_BV(LCD_E);
} 

void lcd_position(unsigned char x, unsigned char y) 
{
	uint8_t location=0; 
	if(y>0x01) 
		y=0x01; 
	if(x>0x0f)
		x=0x0f;
	
	if(y == 0)
		location = x + 0x80; 
	else
		location = x + 0xC0; 
	lcd_control(location); 
}


void lcd_display_string(uint8_t x, char y, char *string) 
{
	lcd_position(x,y);

	while(*string != '\0')
	{
		lcd_write_char(*string);
		string++;
	}
}


