/*
 * oszi-tests.c
 *
 * Created: 10.03.2018 16:22:15
 * Author : Victor
 */ 

#include <avr/io.h>
#define DDR(x) DDR##x
#define PORT(x) PORT##x
#define PIN(x) PIN##x

int waste_time(int i){
	return i*i&i;
}

int main(void)
{
	DDR(B)=1<<5;
    /* Replace with your application code */
    while (1) 
    {
		for(int i = 0; i < 8000; i++){
			for(int i = 0; i < 100; i++){
				//waste_time(i);
			}
			
		}
		PORT(B)=1<<5;
		for(int i = 0; i < 8000; i++){
			for(int i = 0; i < 100; i++){
				//waste_time(i);
			}
			
		}
		PORT(B)=0;
    }
}

