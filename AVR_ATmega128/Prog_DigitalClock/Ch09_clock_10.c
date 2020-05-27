

// Ch09_clock_10.c  

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "LCD.h"  // LCD 관련 함수 헤더 파일

int main(void)
{
	lcd_init() ;  
	
	int i = 0 ;
	unsigned char *month_table[ ] = {"000","JAN","FEB","MAR",
									  "APR","MAY","JUN","JUL",
									  "AUG","SEP","OCT","NOV",
									  "DEC"};
									  
	unsigned char *day_table[ ] = {"SUN","MON","TUE","WED",
									"THU","FRI","SAT"};
	
	unsigned char month_date_table[ ] = {0,31,28,31,
										 30,31,30,31,
										 31,30,31,30,
										 31} ;	
	 
	int year = 2016 ;	int month = 2;	int date = 27 ;
	
	if ( ((year%4 == 0) && (year%100 != 0)) || (year%400 == 0) ) 
		month_date_table[2] = 29 ;
	
	
	int second = 0 ;
	int minute = 0 ;
	int hour = 10 ;
	int AMPM = 0 ;
	
	
	while(1) {
		
		
		lcd_position(12,1) ;  //(12,1) 좌표로 위치 이동 
		lcd_write_char(second/10+'0'); // second 10 자리
		lcd_write_char(second%10+'0'); // second 1 자리
		lcd_write_char('S'); // S 문자 출력
		_delay_ms(10);
		minute++ ; // minute 증가
		
		if(second >= 60) { second = 0; minute++ ;}
		
		lcd_position(8,1) ;  //(8,1) 좌표로 위치 이동 
		lcd_write_char(minute/10+'0'); // minute 10 자리
		lcd_write_char(minute%10+'0'); // minute 1 자리
		lcd_write_char('M'); // M 문자 출력
		
		if(minute >= 60) { minute = 0; hour++ ;}
		
		lcd_position(4,1) ;  //(4,1) 좌표로 위치 이동 
		lcd_write_char(hour/10+'0'); // hour 10 자리
		lcd_write_char(hour%10+'0'); // hour 1 자리
		lcd_write_char('H'); // H 문자 출력
		if(hour >= 13) { hour = 1 ;}
		if((hour == 12)&(minute == 0)&(second == 0)) { 
			AMPM++ ;
			if(AMPM>=2){ AMPM=0 ; date++; }
		}
		
		if(date > month_date_table[month] ) {
			date = 1;
			month++;
		}
		
		if(month >=13) {
			month = 1 ; year++; 
		}
		
		
		lcd_position(1,1) ;  //(1,1) 좌표로 위치 이동 
		if(!AMPM) lcd_write_char('A');
		else if (AMPM) lcd_write_char('P');
		lcd_write_char('M');
		
		
		
		lcd_position(1,0) ;  //(1,0) 좌표로 위치 이동 
		lcd_write_char(year/1000+'0'); // year 1000 자리
		lcd_write_char((year/100)%10+'0'); // year 100 자리
		lcd_write_char((year/10)%10+'0'); // year 10 자리
		lcd_write_char(year%10+'0'); // year 1 자리
		
		lcd_display_string(6,0, month_table[month]) ;
		
		lcd_position(10,0);  //(10,0) 좌표로 위치 이동 
		lcd_write_char(date/10+'0'); // date 10 자리
		lcd_write_char(date%10+'0'); // date 1 자리
		
		int total_date = (year-1)   // total_date를 구해야 '요일'을 표시할 수 있다!!!
		                 +(year-1)/4 	// 1월 1일을 기준으로 구한 뒤 아래 for문에서 월의 날짜들도 더하는 것이다
		                 -(year-1)/100 
						 + (year-1)/400 
						 + date ;
						 
		for( i=1;i<month;i++) 
			total_date += month_date_table[i] ; 
		
		lcd_display_string(13,0, day_table[total_date%7]) ;
		
		
		
				
	}
	
	return 0;
}
