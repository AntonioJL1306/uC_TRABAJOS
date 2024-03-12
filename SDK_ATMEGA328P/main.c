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
	lcd_i2c_desl("Vives acentuando dia a dia mis defectos,resaltando lo que no me sale bien comparandome con todos Que debia ser como este y como aquel");
	lcd_i2c_clr();
	
	char i;
	
	unsigned char Character1[8] = { 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00 };  /* Custom char set for alphanumeric LCD Module */
	unsigned char Character2[8] = { 0x04, 0x02, 0x1E, 0x0F, 0x0F, 0x1E, 0x02, 0x04 };
	unsigned char Character3[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	lcd_i2c_init();
	
	
	LCD_Custom_Char(0, Character1);  /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);  /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character3);
	LCD_Custom_Char(3, Character3);
	LCD_Custom_Char(4, Character3);
	LCD_Custom_Char(5, Character3);
	LCD_Custom_Char(6, Character3);
	LCD_Custom_Char(7, Character1);  /* Build Character6 at position 7 */

	lcd_i2c_cmd(0x80);		/*cursor at home position */
	lcd_i2c_write_string("LOADING");
	lcd_i2c_cmd(0xC0);
	
	for(i=0;i<8;i++)		/* function will send data 1 to 8 to lcd */
	{
		lcd_i2c_data(i);		/* char at 'i'th position will display on lcd */
		lcd_i2c_data(' ');		/* space between each custom char. */
	}
	while (1)
	{
	}
}


