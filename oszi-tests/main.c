/*
 * oszi-tests.c
 *
 * Created: 10.03.2018 16:22:15
 * Author : Victor
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>	
#include <avr/sleep.h>
#include <avr/wdt.h>

#define CONFIG_BYTE(byte,on,off) byte = ((byte & (0xFF^(off)))|on)
#define BIT(n) (1<<n)
#define DDR(x) DDR##x
#define PORT(x) PORT##x
#define PIN(x) PIN##x
#define CLK_PRECOUNT (0x0100-200)


unsigned char precount=0;
unsigned int seconds=0;


void clock_inc(){ // kleine ungenaue uhr die mitläuft
	if(++precount) return;
	precount=CLK_PRECOUNT;
	seconds++;
	if(seconds%2) PORT(B)=1<<5;
	else PORT(B)=0;
}

ISR (TIMER1_COMPA_vect)
{
	// reset timer1
	PORT(B)=1<<5;
	clock_inc();
}

ISR (TIMER1_COMPB_vect)
{
	PORT(C)&=0xF0;
	// servocontrol

}
void init(){
	DDR(B)=1<<5;
	CONFIG_BYTE(TCCR1A , BIT(COM1A1) | BIT(COM1B1) , BIT(WGM10) | BIT(WGM11));// FastPWM Mode mode TOP determined by ICR1 - non-inverting Compare Output mode
	CONFIG_BYTE(TCCR1B , BIT(WGM12) | BIT(CS10) , BIT(CS11) | BIT(CS12) | BIT(WGM13));    // set prescaler to 1, FastPWM Mode mode continued
	CONFIG_BYTE(TIFR1 , BIT(OCF1A) | BIT(OCF1B),0);
	CONFIG_BYTE(TIMSK0 , BIT(OCIE1A), 0);
	OCR1A = 5000;      // set count to 1500 us - 90 degree
	OCR1B = 1500;      // set count to 1500 us - 90 degree
	TCNT1 = 0;         // reset timer
	sei();
}


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {

    }
}

