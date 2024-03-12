/*
 * ports.c
 *
 * Created: 27/02/2024 06:34:52 p. m.
 *  Author: tonoc
 */ 
#include <avr/io.h>


void init_ports(void)
{
	DDRB = 0xFF;
	DDRD |= (1 << PD2) | (1 << PD3);
}