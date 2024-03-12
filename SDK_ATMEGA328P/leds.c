/*
 * leds.c
 *
 * Created: 27/02/2024 06:51:06 p. m.
 *  Author: tonoc
 */ 
#include <avr/io.h>
#include <avr/delay.h>
void led_on_off(void)
{
	
	for (int i=0;i <= 10;i++)
	{
		PORTB ^= 1 << PINB5;
		_delay_ms(500);
	}
}