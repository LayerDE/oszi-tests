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
#include <avr/pgmspace.h>	// for constant arrays

#define F_CPU 8000000UL
#define F_CPU_MHz (F_CPU/1000000UL)
#define CONFIG_BYTE(byte,on,off) byte = ((byte & (0xFF^(off)))|on)
#define BIT(n) (1<<n)
#define DDR(x) DDR##x
#define PORT(x) PORT##x
#define PIN(x) PIN##x
#define CLK_PRECOUNT (0x0100-200)

unsigned const char SERVO_PIN[4] PROGMEM={0x01,0x02,0x04,0x08};

#define SERVO_COUNT 4
#define PRESCALER 8
#define SERVO_PERIODE (unsigned int)20000/SERVO_COUNT*F_CPU_MHz/PRESCALER
#define SERVO_CALC(servo,int16) (servo_mid_ram[servo]*F_CPU_MHz/PRESCALER+int16)

int servo_buffer[4];
unsigned char precount=0;
unsigned int seconds=0;


void clock_inc(){ // kleine ungenaue uhr die mitläuft
	if(++precount) return;
	precount=CLK_PRECOUNT;
	seconds++;
	if(seconds%2) PORT(B)=1<<5;
	else PORT(B)=0;
}

unsigned char servo_index = 0; // index of the servo who gets signal now

unsigned int servo_mid_ram[SERVO_COUNT]={1500,1500,1500,1500};
unsigned int servo_min_ram[SERVO_COUNT];
unsigned int servo_max_ram[SERVO_COUNT];

void sleep(){
	set_sleep_mode(SLEEP_MODE_IDLE);//setup IDLE mode. only 1/4 powerconsumption
	sleep_mode();//goto sleep
}

ISR (TIMER1_COMPA_vect)
{
	PORT(C)|=SERVO_PIN[servo_index];
	OCR1B=servo_buffer[servo_index++];
	servo_index&=0x0F;
	// reset timer1
	clock_inc();
}

ISR (TIMER1_COMPB_vect)
{
	PORT(C)&=0xF0;
	// servocontrol

}
void init(){
	DDR(B)=1<<5;
	DDR(C) = 0x0F; // Servo out
	CONFIG_BYTE(TCCR1A , 0 , BIT(WGM10) | BIT(WGM11) | BIT(COM1A1) | BIT(COM1B1));// CTC Mode mode TOP determined by OCR1A //
	CONFIG_BYTE(TCCR1B , BIT(WGM12) | BIT(CS11) , BIT(CS10) | BIT(CS12) | BIT(WGM13));    // set prescaler to 1/8
	CONFIG_BYTE(TIFR1 , BIT(OCF1A) | BIT(OCF1B),0);
	CONFIG_BYTE(TIMSK1 , BIT(OCIE1A)|BIT(OCIE1B), 0);
	OCR1A = 5000;      // set count to 1500 us - 90 degree
	OCR1B = 1500;      // set count to 1500 us - 90 degree
	TCNT1 = 0;         // reset timer
	sei();
}


int main(void)
{
	init();
    /* Replace with your application code */
    while (1) 
    {
		sleep();
    }
}

