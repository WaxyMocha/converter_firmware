/*
 * Logic.cpp
 *
 * Created: 28.12.2016 10:19:53
 *  Author: Jakub
 */ 

#include <avr/io.h>
#include "include/Extern_variables.h"
#include "include/Logic.h"

uint16_t voltage = 0;
uint16_t current = 0;
uint16_t input_voltage = 0;
uint16_t input_current = 0;
volatile bool button = false;

uint16_t voltage_max = 0;
uint16_t current_max = 0;
bool src = true;//true - voltage, false - current
uint8_t select = 0;//0 - max voltage, 1- max current, 2 - voltage source, 3 - current source, 4 - next
bool selected = false;
uint8_t page = 0;

void PWM_control ()
{
	if (src)
	{
		if (((voltage_max/150)*4096) > voltage && (current_max/100)*4096 < current)
		{
			TCD5.CCA++;
			TCD5.CCB++;
		}
		else
		{
			TCD5.CCA--;
			TCD5.CCB--;
		}
	}
	else
	{
		if ((current_max/100)*4096 > current && ((voltage_max/150)*4096) < voltage)
		{
			TCD5.CCA++;
			TCD5.CCB++;
		}
		else
		{
			TCD5.CCA--;
			TCD5.CCB--;
		}
	}
}
void Encoder_logic ()
{
	if (TCC5.CNT != 0)
	{
		if (TCC5.CNT>>15)//greater than 2^8
		{
			if (selected && (select == 0 || select == 1) && page == 0)
			{
				if (select == 0)
				{
					voltage_max -= (TCC5.CNT/4);
					if (voltage_max < 0)
					{
						voltage_max = 0;
					}
				}
				else
				{
					current_max -= (TCC5.CNT/4);
					if (current_max < 100)
					{
						current_max = 0;
					}
				}
			}
			else
			{
				for (uint8_t i = ((~((TCC5.CNT/4))+1)%5); i < 1; i--)
				{
					if (select == 0)
					{
						select = 4;
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
			if (selected && (select == 0 || select == 1) && page == 0)
			{
				if (select == 0)
				{
					voltage_max += (TCC5.CNT/4);
					if (voltage_max > 150)
					{
						voltage_max = 150;
					}
				}
				else
				{
					current_max += (TCC5.CNT/4);
					if (current_max > 100)
					{
						current_max = 100;
					}
				}
			}
			else
			{
				select += (TCC5.CNT/4)%5;
			}
		}
		TCC5.CNT = 0;
	}
	if (button)
	{
		button = false;
		if (select == 2)
		{
			src = true;
		}
		else if (select == 3)
		{
			src = false;
		}
		else if (select == 4)
		{
			page = 1;
		}
		else
		{
			selected = true;
		}
	}
}