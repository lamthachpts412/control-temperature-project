#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>


#include "lcd.h"
#include "ntc.h"
#include "my_delay.h"


/*-------------------------------------------------Declare variables----------------------------------------------------*/

volatile uint8_t button_was_pressed1 = 0;
volatile uint8_t flag_main = 0;


//Variables for PID
int pid_output;

uint8_t setpoint = 120;
double temperature_now;

double error_now, error_previous=0, error_integral=0, error_derivative;


uint8_t k_proportional = 18; 
uint8_t k_integral = 2; 
uint8_t k_derivative = 0;
/*-------------------------------------------------Declare variables----------------------------------------------------*/


/*---------------------------------------------------MAIN ROUNTINE------------------------------------------------------*/
int main(void)
{
	
	
	//Configure for the I/O ports
	DDRB |=  (1<<0);											//Setup PB0 as Output (Control TRIACC
	DDRC &= ~(1<<0);											//Setup PC0 as Input  (Read temperature)
	DDRC &= ~(1<<2);											//Setup PC2 as Input  (ON/OFF main routine)
	DDRD &= ~(1<<2);											//Setup PD2 as Input  (Detect zero-crossing points)

	PORTB |= (1<<3);											//Use pull-up resistor at PB3
	PORTB |= (1<<4);											//Use pull-up resistor at PB4
	PORTC |= (1<<2);											//Use pull-up resistor at PC2

	
	
	
	//Use external interrupt at PD2 (Detect zero-crossing)
	//EIMSK  |= (1<<INT0);										//Enable external interrupt at PD2 (INT0)
	EICRA  |= (1<<ISC01);										//Use falling edge of INT0 to generate an interrupt
	
	//Use pin change interrupt at PC2 (ON/OFF system)
	PCICR  |= (1 << PCIE1);										//Enable pin change interrupt: PCINT14 to PCINT8
	PCMSK1 |= (1 << PCINT10);									//Enable pin change interrupt at PC2
	
	sei();														//Enable global interrupt
	
	//Initialization for all functions
	NTC_Init(0);												//Select PC0 as Analog Input to read temperature
	LCD_Init();
	
	while (1)
	{
		
		if(flag_main == 1)
		{
			LCD_On();
			EIMSK  |= (1 << INT0);									//Enable external interrupt at PD2 (INT0)
		}
		
		else if(flag_main == 0)
		{
			LCD_Off();
			EIMSK  &= ~(1 << INT0);									//Disable external interrupt at PD2 (INT0)
		}
		
		temperature_now = NTC_Read();								//Read temperature from NTC sensor
		
		/*---------------------------------------------------PID--------------------------------------------------------*/
		
		error_now		= (double)setpoint - temperature_now;
		
		error_integral	= error_integral + error_now;
		
		if(error_integral > 100)
			error_integral = 100;
		else if(error_integral < -100)
			error_integral = -100;
		
		error_derivative	= error_now - error_previous;
		error_previous		= error_now;
		
		pid_output			= (k_proportional*error_now) + (k_integral*error_integral) + (k_derivative*error_derivative);
		
		/*---------------------------------------------------PID--------------------------------------------------------*/
		
		
		if(pid_output > 500)											//Limit error compensation in range 0 - 60
		pid_output = 500;												//Limit error compensation in range 0 - 60
		else if(pid_output < 0)											//Limit error compensation in range 0 - 60
		pid_output = 0;													//Limit error compensation in range 0 - 60
		
		//Print data on LCD
		LCD_Clear();
		
		LCD_Set_Cursor(1,0);
		LCD_Puts("S:");
		LCD_Set_Cursor(3,0);
		LCD_Printf("%d", setpoint);
		
		LCD_Set_Cursor(7,0);
		LCD_Puts("PID:");
		LCD_Set_Cursor(11,0);
		LCD_Printf("%d", pid_output);
		
		LCD_Set_Cursor(1,1);
		LCD_Puts("TEMP:");
		LCD_Set_Cursor(7,1);
		LCD_Printf("%.2lf", temperature_now);
	}
}
/*---------------------------------------------------MAIN ROUNTINE------------------------------------------------------*/


/*--------------------------------Detect zero-crossing points and control TRIAC routine---------------------------------*/
ISR (INT0_vect)
{
	delay_10us(800 - pid_output);
	
	if(pid_output != 0)
	{
		PORTB |= (1<<PINB0);
		_delay_us(500);
		PORTB &= ~(1<<PINB0);
	}
}
/*--------------------------------Detect zero-crossing points and control TRIAC routine---------------------------------*/


/*----------------------------------------------ON/OFF system routine---------------------------------------------------*/
ISR(PCINT1_vect)
{
	
	if(bit_is_clear(PINC,2))								//If button go to LOW...
	{
		_delay_us(1000);
		if(bit_is_clear(PINC,2))
		{
			if (button_was_pressed1 == 0)					//and the last status of button is HIGH
			{
				
				flag_main = !flag_main;
				
				button_was_pressed1 = 1;					//Setup this to prevent bouncing
				
			}
		}
	}
	
	else
	{
		button_was_pressed1 = 0;
	}
}
/*----------------------------------------------ON/OFF system routine---------------------------------------------------*/




