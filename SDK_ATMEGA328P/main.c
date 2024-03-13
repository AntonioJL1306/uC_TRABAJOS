/*
 * SDK_ATMEGA328P.c
 *
 * Created: 27/02/2024 06:32:11 p. m.
 * Author : tonoc
 */ 
#include <avr/io.h>
#include "ports.h"
#include "leds.h"
#include "i2c.h"
#include "lcd_i2c.h"
int main(void)
{
	/* Replace with your application code */
	init_ports();
	led_on_off();
	init_i2c();
	lcd_i2c_init();
	lcd_i2c_clr();
	lcd_i2c_desl("NO TENGO NADA PINCHE GOBIERNO MALO");
	LCD_clr();
	lcd_i2c_init();
	lcd_i2c_cmd(0x80);
	lcd_i2c_write_string("LOADING...");
	while (1)
	{
		lcd_i2c_movimiento_carcter();
	}
}


