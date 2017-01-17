/*
 * Logic.cpp
 *
 * Created: 28.12.2016 10:19:53
 *  Author: Jakub
 */ 

#include <avr/io.h>
#include "include/Extern_variables.h"
#include "include/Logic.h"
#include "include/Display.h"

void PWM_mode(bool mode);

uint16_t voltage = 0;
uint16_t current = 0;
uint16_t input_voltage = 1396;
uint16_t input_current = 100;

float voltage_max = 120;
float current_max = 10;
float UVLO = 0;
bool src = false;//true - voltage, false - current
bool selected = false;
bool active = false;
uint8_t select = 0;
uint8_t page = 0;
uint8_t coordinates [3][6][3] = {
	{
		{
			64, 0, 21 //max_voltage
		},
		{
			64, 10, 21//max_current
		},
		{
			104, 21, 23//NEXT
		}
	},
	{
		{
			56, 0, 21//UVLO
		},
		{
			0, 21, 23//BACK
		},
		{
			104, 21, 23//NEXT
		}
	},
	{
		{
			0, 0, 48//screen_off
		},
		{
			0, 21, 23//BACK
		}
	}
};
uint16_t rtc = 0;

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
	if (CNT != 0)//if user spin encoder
	{
		if (!active)//selection is off
		{
			active = true;//set flag on
			rtc = RTC.CNT;//get rtc value
			Display_rect(coordinates[page][select][0], coordinates[page][select][1], coordinates[page][select][2], 9, true);//draw selection
		}
		else//selection is on
		{
			Display_fill_rect(coordinates[page][select][0], coordinates[page][select][1], coordinates[page][select][2], 9, false);//clear old selection
			rtc = RTC.CNT;
		}
	}
	else
	{
		if (RTC.CNT > rtc + 5)//If the elapsed 5 seconds, deselect
		{
			active = false;
			Display_fill_rect(coordinates[page][select][0], coordinates[page][select][1], coordinates[page][select][2], 9, true);
		}
	}
	
	if (CNT>>15)//greater than 2^8 == below 0
	{
		if (selected)
		{
			if (page == 0)
			{
				if (select == 0)
				{
					voltage_max -= (CNT/4);//one spin increment or decrement counter by 4 
					if (voltage_max < 0)
					{
						voltage_max = 0;
					}
				}
				else if (select == 1)
				{
					current_max -= (CNT/4);
					if (current_max < 0)
					{
						current_max = 0;
					}
				}
			} 
			else if (page == 1)
			{
				if (select == 0)
				{
					UVLO -= (CNT/4);
					if (UVLO < 0)
					{
						UVLO = 0;
					}
				}
			}
		}
		else
		{
			if (page == 0)
			{
				for (uint8_t i = (((~(CNT/4))+1)%3); i < 1; i--)//2->1->0->2
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
			else if (page == 1)
			{
				for (uint8_t i = (((~(CNT/4))+1)%3); i < 1; i--)
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
			else
			{
				for (uint8_t i = (((~(CNT/4))+1)%2); i < 1; i--)
				{
					if (select == 0)
					{
						select = 1;
					}
					else
					{
						select--;
					}
				}
			}
		}
	}
	else
	{
		if (selected)
		{
			if (page == 0)
			{
				if (select == 0)
				{
					voltage_max += (CNT/4);
					if (voltage_max > 150)
					{
						voltage_max = 150;
					}
				}
				else if (select == 1)
				{
					current_max += (CNT/4);
					if (current_max > 100)
					{
						current_max = 100;
					}
				}
			}
			else if (page == 1)
			{
				if (select == 0)
				{
					UVLO += (CNT/4);
					if (UVLO > 1500)
					{
						UVLO = 150;
					}
				}
			}
		}
		else
		{
			if (page == 0)
			{
				for (uint8_t i = ((((CNT/4)))%3); i < 1; i++)//0->1->2->0
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
			else if (page == 1)
			{
				for (uint8_t i = ((((CNT/4)))%3); i < 1; i++)
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
			else
			{
				for (uint8_t i = ((((CNT/4)))%2); i < 1; i++)
				{
					if (select == 1)
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
	}
	if (PORTC.INTFLAGS & PORT_INT3IF_bm)//interrupt flag for switch
	{
		PORTC.INTFLAGS = 0;
		if ( (page == 0 && (select == 0 || select == 1)) || (page == 1 && select == 0) )//page 0 and max_voltage or max_current OR page 1 and UVLO
		{
			if (selected)
			{
				selected = false;//select
			}
			else
			{
				selected = true;//deselect
			}
		}
		else
		{
			if (page == 0 && select == 2)//NEXT
			{
				page = 1;
			} 
			else if (page == 1)
			{
				if (select == 1)//BACK
				{
					page = 0;
				} 
				else if (select == 2)//NEXT
				{
					page = 2;
				}
			}
			else if (page == 2 && select == 1)//BACK
			{
				page = 1;
			}
		}
	}
}