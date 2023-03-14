/*
 * ntc.c
 *
 * Created: 4/8/2021 8:12:07 PM
 * Author : lamth
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "adc.h"


const int sample_number = 50;			//default is 10
const float Beta		= 3950.0;
const float adc_max		= 1023.0;		//ADC 10bit
const float R_balance	= 98500.0;		//98.5k Ohm
const float R_o			= 100000.0;		//100k Ohm, Resistor of Thermistor at T=25 degree Celsius
const float T_o			= 298.15;		//T=25 degree Celsius <=> T=298.15 degree Kelvin
	

void NTC_Init(uint8_t channel)
{
	ADC_Init(channel);
}

double NTC_Read()
{
	double	R_thermistor	= 0;
	double	T_thermistor_K	= 0;
	double	T_thermistor_C	= 0;
	double  adc_average		= 0;
	int		adc_sample[sample_number];
	
	for(int i=0; i<sample_number; i++)
	{
		adc_sample[i] = ADC_Read();
		_delay_ms(2);						//default is 10ms
	}
	
	for(int i=0; i<sample_number; i++)
	{
		adc_average += adc_sample[i];
	}
	adc_average /= sample_number;
	
	R_thermistor = R_balance * ( (adc_max/adc_average) - 1 );
	
	T_thermistor_K = 1 / ( (1/T_o) + ( (1/Beta) * log(R_thermistor/R_o) ) );
	
	T_thermistor_C = T_thermistor_K - 273.15;
	
	return T_thermistor_C;
}