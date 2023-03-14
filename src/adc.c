/*
 * adc.c
 *
 * Created: 4/8/2021 10:39:22 AM
 * Author : lamth
 */ 

//Default:	-Vref		= AVcc (5V)
//			-Prescaler	= 128
//			-Save result value in right-justified
	
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

uint16_t result_value;

void ADC_Init(uint8_t channel)
{
	//Select Channel
	if(channel == 0)
		ADMUX = 0x00;					//Select channel 0
	else if(channel == 1)
		ADMUX = (1<<MUX0);				//Select channel 1
	else if(channel == 2)
		ADMUX = (1<<MUX1);				//Select channel 2
	else if(channel == 3)
		ADMUX = (1<<MUX1) | (1<<MUX0);	//Select channel 3
	else if(channel == 4)
		ADMUX = (1<<MUX2);				//Select channel 4
	else if(channel == 5)
		ADMUX = (1<<MUX2) | (1<<MUX0);	//Select channel 5
	else
		ADMUX |= 0x00;					//Don't change anything to ADMUX
		
	//Select Vref = AVcc
	ADMUX |= (1<<REFS0);
	
	//Select prescaler = 128
	ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	//Enable ADC
	ADCSRA |= (1<<ADEN);
	
}

int ADC_Read()
{
	int result_Value;
	ADCSRA |= (1<<ADSC); //Start conversion
	while( (ADCSRA & (1<<ADIF))  ==0 ); //Wait for conversion to finish
	result_Value = ADC;
	return result_Value;	
}


