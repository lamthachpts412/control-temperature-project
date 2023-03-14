/*
 * delay_ms.c
 *
 * Created: 4/16/2021 4:33:51 PM
 * Author : lamth
 */ 
#include "my_delay.h"
#define F_CPU 8000000UL
#include <util/delay.h>

void delay_10us(int n)
{
	while(n--)
	{
		_delay_us(10);
	}
}

void delay_ms(int n) 
{
	while(n--) 
	{
		_delay_ms(1);
	}
}

