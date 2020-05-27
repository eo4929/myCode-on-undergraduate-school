#define F_CPU 16000000UL
#define ADCSRA  _SFR_IO8(0x06)
#define SFIOR   _SFR_IO8(0x30)

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"  // LCD 관련 함수 헤더 파일
#include <stdlib.h>


float temp; 
int speed = 999;
unsigned char spd_1 = 9;
unsigned char spd_2 = 9;
unsigned char spd_3 = 9;

unsigned char NumbersOfCom[5] = {0,};
unsigned char NumbersOfUser[5] = {0,};

int idx_com=0;
int idx_user=0;
unsigned char clearBit=0;

void init_NumbersOfCom(){
	int i=0;
	for(i=0; i<5; i++){
		NumbersOfCom[i] = 0;
	}
	idx_com=0;
}

void init_NumbersOfUser(){
	int i=0;
	for(i=0; i<5; i++){
		NumbersOfUser[i] = 0;
	}
	idx_user=0;
}

void push_NumbersOfCom(unsigned char val){
	NumbersOfCom[idx_com] = val;
	idx_com++;
}

void push_NumbersOfUser(unsigned char val){
	NumbersOfUser[idx_user] = val;
	idx_user++;
}

void pop_and_print_NumbersOfCom(){
	unsigned char temp[5]={0,};
	int i;
	
	for(i=0; i<5; i++){
		temp[i] = NumbersOfCom[i];
		NumbersOfCom[i] = 0;
	}
	idx_com=0;
	
	lcd_display_string(0,0,"com: ");
	lcd_position(5,0);
	for(i=0; i<5; i++){
		lcd_write_char(temp[i]+'0');
		lcd_write_char(' ');
	}
}

void pop_and_print_NumbersOfUser(){
	unsigned char temp[5]={0,};
	int i;
	
	for(i=0; i<5; i++){
		temp[i] = NumbersOfUser[i];
		NumbersOfUser[i] = 0;
	}
	idx_user=0;
	
	lcd_display_string(0,1,"you: ");
	lcd_position(5,1);
	for(i=0; i<5; i++){
		lcd_write_char(temp[i]+'0');
		lcd_write_char(' ');
	}
}

void checkClear(){
	int i=0;
	clearBit=1;
	for(i=0; i<5; i++){
		if(NumbersOfCom[i] != NumbersOfUser[i]){
			clearBit=0;
		}
	}
}


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
	unsigned char randomLed = random() % 8;
	int i_port = 0;
 
	PORTA = 0xFF ;
	PORTB = 0x00 ;
	
	unsigned char Seg_table[16]={ 0x3F,0x06,0x5B,0x4F,
	                              0x66,0x6D,0x7D,0x07,
								  0x00,0x00,0x00,0x00,
								  0x00,0x00,0x00,0x00 }; //0~7까지만 출력
	
	unsigned char key_check[2] = { 0x70, 0xe0} ; //0b01110000, 0b11100000, 0b11010000, 0b10110000 0->1432
	unsigned char value = 0 ;
	unsigned char inp = 0, key = 0 ;
	int i = 0 ; unsigned char numOfClick = 0;
	
	int generating_flag = 0;
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
			if(generating_flag == 0){
				lcd_init();
				lcd_display_string(0,0,"remember Nums!");
				_delay_ms(2000);
				
				for(i_port = 0; i_port<5; i_port++){
					lcd_display_string(0,0,"remember Nums!");
			
					randomLed = random() % 8;
					PORTA = 0xFF;
					_delay_ms(speed);
					PORTA = Led_table[randomLed];
					push_NumbersOfCom(randomLed);
			
					lcd_position(0,1);
					lcd_write_char(spd_1+'0');
					lcd_write_char(spd_2+'0');
					lcd_write_char(spd_3+'0');
					lcd_write_char('m');
					lcd_write_char('s');
			
					lcd_position(10,1);
					lcd_write_char('c');
					lcd_write_char('n');
					lcd_write_char('t');
					lcd_write_char(':');
					lcd_position(14,1);
					lcd_write_char( i_port + 1 + '0');
					
					_delay_ms(300);
				}
				
				_delay_ms(1000);
				lcd_init();
				lcd_display_string(0,0,"click buttons!");
				PORTA = 0xFF;
				
				numOfClick=0;
				generating_flag = 1;
			}
		
			for(i=0; i<2; i++) {
				PORTC = key_check[i] ;
				value = PINC ; // PORTC로부터 입력을 받음
				value &= 0x0f ;
			
				if(value != 0x0f) {
					inp = (~value)&0x0f ;
					
					if(inp == 0x01){ key = 0 + 4*i ; _delay_ms(250); numOfClick++; }
					if(inp == 0x02){ key = 1 + 4*i ; _delay_ms(250); numOfClick++; } 
					if(inp == 0x04){ key = 2 + 4*i ; _delay_ms(250); numOfClick++; } 
					if(inp == 0x08){ key = 3 + 4*i ; _delay_ms(250); numOfClick++; } 
				
					PORTB = Seg_table[key] ;
					lcd_init();
					lcd_display_string(0,0,"Cheer up!");
				
					lcd_position(10,1);
					lcd_write_char('c');
					lcd_write_char('n');
					lcd_write_char('t');
					lcd_write_char(':');
					lcd_position(14,1);
					lcd_write_char( numOfClick + '0');
					
					push_NumbersOfUser(key);
				}
			}
			
			if(numOfClick >= 5){
				_delay_ms(2000);
				checkClear();
			
				lcd_init();
				pop_and_print_NumbersOfCom();
				pop_and_print_NumbersOfUser();
				
				if(clearBit == 1){ _delay_ms(5000); lcd_init(); lcd_display_string(0,0,"You're genius!"); return;}
				if(clearBit == 0){ _delay_ms(5000); lcd_init(); lcd_display_string(0,0,"Try again!"); _delay_ms(5000); generating_flag = 0;}
			}
			
		}
		
		temp = ADCH;	//LED로 표시하기위해 ADCL의 내용을 변수에 넣어둠.
		
		speed=(temp*999)/255;
		spd_1 = speed/100 ;
		spd_2 = (speed/10)%10 ;
		spd_3 = speed%10 ;
		
		lcd_init();
		
		init_NumbersOfCom();
		init_NumbersOfUser();
		
		generating_flag = 0;
	}

	return 0;
}

