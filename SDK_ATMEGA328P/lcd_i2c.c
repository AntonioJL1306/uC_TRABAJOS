/*
 * lcd_i2c.c
 *
 * Created: 6/29/2021 7:38:17 PM
 *  Author: jlb
 */
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "lcd_i2c.h"

//first column address
char Columna_1 [2] = {0 , 64};	

/*	4 bits mode (pag.46 44780 datasheet) */
void lcd_i2c_reset(void)
{
	//unsigned char data;
	start();	//START--> I2C
	write_i2c(PCF8574_ADDR);
	
	write_i2c(0xFF); //wait for 15ms (at least)
	_delay_ms(20);
	
	write_i2c(0x30+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	write_i2c(0x30);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(10);			//wait for 4.1ms (ate least)
	
	write_i2c(0x30+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	write_i2c(0x30);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(1);			//wait for 100 us (at least)
	
	write_i2c(0x30+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=1
	write_i2c(0x30);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 & E=0
	_delay_ms(1);			//wait for 100 us (at least)
	
	write_i2c(0x20+LCD_EN); //write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 & E=1
	write_i2c(0x20);		//write Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 & E=0
	_delay_ms(1);			//wait for 100 us (at least)
	
	stop();	//STOP--> I2C
}

/*	write command to LCD	*/
void lcd_i2c_cmd (char cmd)
{
	unsigned char data;
	start();	//START--> I2C
	write_i2c(PCF8574_ADDR);
	//I2C address
	
	data=(cmd & 0xF0)|LCD_EN|LUZ_FONDO;
	write_i2c(data);

	data=(cmd & 0xF0)|LUZ_FONDO;
	write_i2c(data);

	data=((cmd << 4) & 0xF0)|LCD_EN|LUZ_FONDO;
	write_i2c(data);

	data=((cmd << 4) & (0xF0|LUZ_FONDO));
	write_i2c(data);
	
	stop();	//STOP--> I2C
	_delay_ms(2);
	_delay_ms(2);
}

/*	LCD	initialization */
void lcd_i2c_init (void)
{
	init_i2c(); //****init I2C****

	lcd_i2c_reset();		// Reset LCD.
	
	lcd_i2c_cmd(0x2C);		// Function Set: 4 bits mode, 2 lines, 5�10 resolution
	//lcd_i2c_cmd(0x0F);	// Display control: LCD on, cursor on, blinking
	lcd_i2c_cmd(0x0C);		// cursor off
	lcd_i2c_cmd(0x06);		// Entry mode: Increment.
	lcd_i2c_cmd(0x80);		// points to DDRAM.
	lcd_i2c_cmd(0xC0);
	lcd_i2c_cmd(0x40);
}

/*	display character	*/
void lcd_i2c_data (unsigned char dat)
{
	unsigned char data;
	start();	//START--> I2C
	write_i2c(PCF8574_ADDR);

	data=((dat & 0xF0)|LCD_EN|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=((dat & 0xF0)|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=(((dat << 4) & 0xF0)|LCD_EN|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=(((dat << 4) & 0xF0)|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	stop();	// STOP--> I2C
	_delay_ms(2);
	_delay_ms(2);
}

/* set cursor on specified column&row	*/
void lcd_i2c_col_row(uint8_t x, uint8_t y)
{
	lcd_i2c_cmd(0x80 + Columna_1[y-1]+(x-1));
}

/*	clear screen	*/
void lcd_i2c_clr(void)
{
	lcd_i2c_cmd(0x01); //clear LCD (0000 0001)
	_delay_ms(2);
}

/*	write character string	*/
void lcd_i2c_write_string(char *a)
{
	unsigned int i;
	for(i=0;a[i]!=0;i++)
	lcd_i2c_data(a[i]);
}

/*	Write integer value */
void lcd_i2c_write_int(int value,unsigned int field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(value)
		{
			str[i]=value%10;
			value=value/10;
			i--;
		}
	if(field_length==-1)
	while(str[j]==0) j++;
	else  
		j=5-field_length;
	if(value<0) lcd_i2c_data('-');
	for(i=j;i<5;i++)
		{
			lcd_i2c_data(48+str[i]);
		}
}
void lcd_i2c_desl(const char * text){
	int pos=0;
	while (1){
	for (int i=0; text[i] !='\0'; i++){
		if (pos==16){
			lcd_i2c_cmd(0xC0);
	}
		else if (pos==32){
			lcd_i2c_cmd(0x01); 
			_delay_ms(100);
			lcd_i2c_cmd(0x80);
			pos=0;
	}

	lcd_i2c_data(text[i]);
	_delay_ms(200);
	pos++;
		}
		return 0;	
	}
}

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
    unsigned char i;
    if(loc<8)
    {
     lcd_i2c_cmd (0x40 + (loc*8));  /* Command 0x40 and onwards forces 
                                       the device to point CGRAM address */
       for(i=0;i<8;i++)  /* Write 8 byte for generation of 1 character */
           lcd_i2c_data(msg[i]);      
    }   
}
void LCD_clr(void)
{
	lcd_i2c_cmd(0x01); //clear LCD (0000 0001)
	_delay_ms(10);
}

void lcd_i2c_movimiento_carcter(void){
	char i;
	unsigned char Character2[8] = { 0x04, 0x02, 0x1E, 0x0F, 0x0F, 0x1E, 0x02, 0x04 };
	unsigned char Character1[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char Character3[8] = { 0x00, 0x00, 0x00, 0x04, 0x1F, 0x0E, 0x0E, 0x0A };
	unsigned char Character4[8] = { 0x04, 0x08, 0x0F, 0x1E, 0x1E, 0x0F, 0x08, 0x04 };
	unsigned char Character5[8] = { 0x0A, 0x0E, 0x0E, 0x1F, 0x04, 0x00, 0x00, 0x00 };
	LCD_Custom_Char(0, Character1);
	LCD_Custom_Char(1, Character1);
	LCD_Custom_Char(2, Character1);
	LCD_Custom_Char(3, Character2);
	_delay_ms(2000);
	LCD_Custom_Char(3, Character3);
	_delay_ms(2000);
	LCD_Custom_Char(3, Character4);
	_delay_ms(2000);
	LCD_Custom_Char(3, Character5);
	_delay_ms(2000);
	LCD_Custom_Char(4, Character1);
	LCD_Custom_Char(5, Character1);
	LCD_Custom_Char(6, Character1);
	LCD_Custom_Char(7, Character1);
	lcd_i2c_cmd(0xC0);
	for (int i=0;i<8; i++){
		lcd_i2c_data(i);		/* char at 'i'th position will display on lcd */
		lcd_i2c_data(' ');		/* space between each custom char. */
	}
	}
