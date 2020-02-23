#define F_CPU 16000000UL
#define ADCSRA  _SFR_IO8(0x06)
#define SFIOR   _SFR_IO8(0x30)

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"  // LCD 관련 함수 헤더 파일
#include <stdlib.h>


float temp; 
int speed = 0;
unsigned char spd_1 = 0;
unsigned char spd_2 = 0;
unsigned char spd_3 = 0;

//포트초기화 함수 정의
void initPort()
{
	DDRA = 0xff;	//PortA를 출력으로 사용
	DDRB = 0xff;	//PortB를 출력으로 사용
	DDRC = 0xf0;	//PortC를 출력으로 사용
	DDRD = 0xff;	//PortD를 출력으로 사용		
	DDRF = 0x00;	//PortF를 입력으로 사용
	                // -> PortF0를 A/D 입력으로 사용 (Step 1)
}

void start()
{
	int i=0;
	for(i=0; i<3; i++)
	{
		lcd_display_string(0,0,"adjust level");
		lcd_display_string(0,1,"using a/d");
		PORTA = 0x00;
		_delay_ms(2000);
		
		PORTA = 0xFF;
		lcd_init();
		_delay_ms(2000);
	}
	
	lcd_init();
}

int main(void)
{
	initPort( ) ;		//포트 초기화
	lcd_init( ) ;       // LCD 초기화
	
	unsigned char Led_table[8] = { 0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111,
									0b10111111, 0b01111111 };
	int randomLed = random() % 8;
 
	PORTA = 0xFF ;
	PORTB = 0x00 ; // 7 Segment Off
	
	unsigned char Seg_table[16]={ 0x3F,0x06,0x5B,0x4F,
	                              0x66,0x6D,0x7D,0x07,
								  0x00,0x00,0x00,0x00,
								  0x00,0x00,0x00,0x00 }; //0~7까지만 출력
	
	unsigned char key_check[2] = { 0x70, 0xe0} ; //0b01110000, 0b11100000, 0b11010000, 0b10110000 0->1432
	unsigned char value = 0 ;
	unsigned char inp = 0, key = 0 ;
	int i = 0 ; unsigned char numOfClick = 0;
	
	ADMUX  = 0x60;	//Vref=AVcc, ADC0의 단일전압모드, 좌로조정
	ADCSRA = 0xa5;	//ADC enable, 프리스케일러 32, 단일변환모드, auto trigger
	SFIOR &= 0x1f;	//free running 모드
	ADCSRA |= 0x40; //ADC 시작
	
	start();
	
	while(1) {
		_delay_us(200);	//샘플링 기간
		ADCSRA |= 0x10; //clear ADIF
		while((ADCSRA & 0x10) == 0x00)	//ADIF=1일 때까지 지연
		{
			randomLed = random() % 8;
			_delay_ms(speed); // 원인 발견!  이 코드 넣는 순간 제대로 인식 못함 -> 인터럽트 기능이 없으니 이 딜레이 동안 버튼 누르면 인식을 못한다;;
			PORTA = Led_table[randomLed]; // 게임 번경하자: 컴퓨터가 LED 로 뿌려주면 그에 맞는 순서와 숫자 기억했다가 키패드로 눌러 맞추는 게임으로
			
			for(i=0; i<2; i++) {
				PORTC = key_check[i] ;
				value = PINC ; // PORTC로부터 입력을 받음
				value &= 0x0f ;
			
				if(value != 0x0f) { // 에러 부분: value가 키를 누른 것과 상관없이 무조건 0x0f가 아니게 되어서 키패드 안눌러도 이 if문에 들어가버림 
					inp = (~value)&0x0f ; // 에러 부분:
					
					if(inp == 0x01){ key = 0 + 4*i ; _delay_ms(250); numOfClick++; }
					if(inp == 0x02){ key = 1 + 4*i ; _delay_ms(250); numOfClick++; } 
					if(inp == 0x04){ key = 2 + 4*i ; _delay_ms(250); numOfClick++; } 
					if(inp == 0x08){ key = 3 + 4*i ; _delay_ms(250); numOfClick++; } 
				
					PORTB = Seg_table[key] ;
				
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
		
		temp = ADCH;	//LED로 표시하기위해 ADCL의 내용을 변수에 넣어둠.
				
		speed=(temp*999)/255;
		spd_1 = speed/100 ;
		spd_2 = (speed/10)%10 ;
		spd_3 = speed%10 ;
		
		lcd_position(3,1);
		lcd_write_char(spd_1+'0');
		lcd_write_char('.');
		lcd_write_char(spd_2+'0');
		lcd_write_char(spd_3+'0');
		lcd_write_char('S');
		lcd_write_char('p');
		lcd_write_char('d');
	} 
}

