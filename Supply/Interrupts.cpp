/*
 * Interrupts.cpp
 *
 * Created: 28.12.2016 10:39:43
 *  Author: Jakub
 */ 
#include <avr/interrupt.h>
#include "include/Interrupts.h"
#include "include/Extern_variables.h"
#include "include/Display.h"

ISR (TWIC_TWIM_vect)
{
	uint8_t to_send = 0;
	if (control == 0)
	{
		control++;
		to_send = SSD1306_I2C_ADDRESS; //addres
	}
	else if (control == 1)
	{
		control++;
		to_send = 0x00;
	}
	else
	{
		control = 0; //command was send
		if (screen_send)//initialized
		{
			if (screen_counter < 512)
			{
				screen_counter++;
				to_send = screen[screen_counter];
			}
			else
			{
				screen_counter = 0;
				screen_send = false;
			}
		}
		else if (buffer_counter != 0)//command
		{
			to_send = buffer[0];
			buffer_counter--;
			for (uint8_t i = 0; i < buffer_counter; i++)//fifo
			{
				buffer[i] = buffer[i+1];
			}
		}
	}
	TWIC.MASTER.DATA = to_send;
}
ISR(EDMA_CH0_vect)
{
	if (EDMA.CH0.ADDR == (uint16_t)&voltage)
	{
		EDMA.CH0.ADDR = (uint16_t)&current;
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
	}
	else if (EDMA.CH0.ADDR == (uint16_t)&current)
	{
		EDMA.CH0.ADDR = (uint16_t)&input_voltage;
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
	}
	else if (EDMA.CH0.ADDR == (uint16_t)&input_voltage)
	{
		EDMA.CH0.ADDR = (uint16_t)&input_current;
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
	}
	else
	{
		EDMA.CH0.ADDR = (uint16_t)&voltage;
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	}
	ADCA.CTRLA |= ADC_START_bm;
}
ISR (PORTC_INT_vect)
{
	button = true;
}