/*
 * Logic.cpp
 *
 * Created: 28.12.2016 10:19:53
 *  Author: Jakub
 */ 

#include <avr/io.h>
#include "include/Extern_variables.h"
#include "include/Logic.h"

void PWM_mode(bool mode);

uint16_t voltage = 0;
uint16_t current = 0;
uint16_t input_voltage = 1396;
uint16_t input_current = 100;

float voltage_max = 120;
float current_max = 10;
bool src = false;//true - voltage, false - current
uint8_t select = 0;//0 - max voltage, 1- max current, 2 - next
bool selected = false;
uint8_t page = 0;

void PWM_control ()
{
	float voltage_temp = ((voltage_max*4096)/150);
	float current_temp = ((current_max*4096)/100);
	
	if (TCD5.CCA == 0xFF && (TCD5.CTRLE & ~TC5_CCBMODE0_bm))
	{
		PWM_mode(true);
	}
	else if (TCD5.CCB == 0x0 && (TCD5.CTRLE & ~TC5_CCAMODE0_bm))
	{
		PWM_mode(false);
	}
	if (voltage_temp > voltage && current_temp > current)
	{
		if ((TCD5.CTRLE & TC5_CCAMODE0_bm) && TCD5.CCA < 0xFF)
		{
			TCD5.CCA++;
		}
		if ((TCD5.CTRLE & TC5_CCBMODE0_bm) && TCD5.CCB < 0xFF)
		{
			TCD5.CCB++;
		}
	}
	else
	{
		if ((TCD5.CTRLE & TC5_CCAMODE0_bm) && TCD5.CCA > 0x0)
		{
			TCD5.CCA--;
		}
		else if ((TCD5.CTRLE & TC5_CCBMODE0_bm) && TCD5.CCB > 0x0)
		{
			TCD5.CCB--;
		}
	}
}
void PWM_mode(bool mode) //0- buck 1-boost
{
	if (!mode)
	{
		TCD5.CCA = 0;
		TCD5.CTRLE |= TC5_CCAMODE0_bm;
		PORTD.PIN4CTRL &= ~PORT_OPC_PULLUP_gc;
		
		TCD5.CTRLE &= ~TC5_CCBMODE0_bm;
		PORTD.PIN5CTRL |= PORT_OPC_PULLDOWN_gc;
	} 
	else
	{
		TCD5.CCB = 0;
		TCD5.CTRLE |= TC5_CCBMODE0_bm;
		PORTD.PIN5CTRL &= ~PORT_OPC_PULLDOWN_gc;
		
		TCD5.CTRLE &= ~TC5_CCAMODE0_bm;
		PORTD.PIN4CTRL |= PORT_OPC_PULLUP_gc;
	}
}
void Encoder_logic ()
{
	uint16_t CNT = TCC5.CNT;
	if (CNT>>15)//greater than 2^8 == below 0
	{
		if (selected && page == 0)
		{
			if (select == 0)
			{
				voltage_max -= (CNT/4);
				if (voltage_max < 0)
				{
					voltage_max = 0;
				}
			}
			else
			{
				current_max -= (CNT/4);
				if (current_max < 0)
				{
					current_max = 0;
				}
			}
		}
		else
		{
			for (uint8_t i = ((~((CNT/4))+1)%5); i < 1; i--)
			{
				if (select == 0)
				{
					select = 2;
				}
				else
				{
					select--;
				}
			}
		}
	}
	else
	{
		if (selected && page == 0)
		{
			if (select == 0)
			{
				voltage_max += (CNT/4);
				if (voltage_max > 150)
				{
					voltage_max = 150;
				}
			}
			else
			{
				current_max += (CNT/4);
				if (current_max > 100)
				{
					current_max = 100;
				}
			}
		}
		else
		{
			for (uint8_t i = ((~((CNT/4))+1)%5); i < 1; i--)
			{
				if (select == 2)
				{
					select = 0;
				}
				else
				{
					select++;
				}
			}
		}
	}
	if (PORTC.INTFLAGS & PORT_INT3IF_bm)//interrupt flag for switch
	{
		PORTC.INTFLAGS = 0;
		if (select == 2)
		{
			page = 1;
		}
		else
		{
			selected = true;
		}
	}
}