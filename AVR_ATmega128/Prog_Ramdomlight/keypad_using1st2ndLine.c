#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"  // LCD 관련 함수 헤더 파일

int main(void)
{
	DDRA = 0xff ;
	DDRB = 0xff ;
	DDRC = 0xf0 ; // 0b11110000 출출출출 입입입입
	
	PORTA = 0xff ; // LED Off
	PORTB = 0x00 ; // 7 Segment Off
	
	lcd_init();
	
	unsigned char Seg_table[16]={ 0x3F,0x06,0x5B,0x4F,
	                              0x66,0x6D,0x7D,0x07,
								  0x00,0x00,0x00,0x00,
								  0x00,0x00,0x00,0x00 }; //0~7까지만 출력
	
	unsigned char key_check[2] = { 0x70, 0xe0} ; //0b01110000, 0b11100000, 0b11010000, 0b10110000 0->1432
	unsigned char value = 0 ;
	unsigned char inp = 0, key = 0 ;
	int i = 0 ;
	
	unsigned char numOfClick = 0;
	
	while(1) {
	
		for(i=0; i<2; i++) {
			PORTC = key_check[i] ;
			value = PINC ; // PORTC로부터 입력을 받음
			value &= 0x0f ;
			
			if(value != 0x0f) {
				inp = (~value)&0x0f ; // value 반전하고 하위 4비트 추출 
				
				if(inp == 0x01){ key = 0 + 4*i ; _delay_ms(250); numOfClick++; } // 200이어도 가끔 숫자가 더 증가되었는데
				if(inp == 0x02){ key = 1 + 4*i ; _delay_ms(250); numOfClick++; } // 250으로 하니 안정적으로 1씩 증가됨
				if(inp == 0x04){ key = 2 + 4*i ; _delay_ms(250); numOfClick++; } 
				if(inp == 0x08){ key = 3 + 4*i ; _delay_ms(250); numOfClick++; } 
				
				PORTB = Seg_table[key] ;
				
				lcd_init();
				lcd_position(14,0);
				if(numOfClick>=10)
				{
					lcd_write_char( numOfClick/10 + '0');
					lcd_write_char( numOfClick%10 + '0');
				}
				else
					lcd_write_char( numOfClick + '0');
			}
		}
		
	}
	
	return 0;
}