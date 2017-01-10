/*
 * Make_screen.cpp
 *
 * Created: 28.12.2016 10:59:36
 *  Author: Jakub
 */ 
#include <avr/io.h>
#include <stdio.h>
#include "include/Make_screen.h"
#include "include/Extern_variables.h"
#include "include/Display.h"


void make_screen ()
{
	float voltage_to_display = (voltage/4096)*15;
	float current_to_display = (current/4096);
	char temp [4];
	
	//TODO: add clear screen
	
	if (page == 0)
	{
		Display_char(0, 0, 'V', true, false, 1);
		Display_char(0, 10, 'A', true, false, 1);
		Display_char(0, 21, 'W', true, false, 1);
		
		Display_text(40, 0, "Max", 1, true, false);
		Display_text(40, 10, "Max", 1, true, false);
		Display_text(104, 21, "Next", 1, true, false);
	}
	else if (page == 1)
	{
		Display_text(0, 0, "UVLO", 1, true, false);
		Display_text(0, 10, "Effectivity", 1, true, false);
		Display_text(0, 21, "Back", 1, true, false);
		Display_text(104, 21, "Next", 1, true, false);

		Display_char(80, 10, '%', true, false, 1);
	}
	else
	{
		Display_text(0, 0, "Screen off", 1, true, false);
		Display_text(0, 10, "Switching", 1, true, false);
		Display_text(0, 21, "Back", 1, true, false);
		Display_text(70, 10, "kHz", 1, true, false);
	}

	if (page == 0)
	{
		sprintf(temp, "%f", voltage_to_display);
		Display_text(15, 0, temp, 1, true, false);

		sprintf(temp, "%f", current_to_display);
		Display_text(15, 10, temp, 1, true, false);
		
		sprintf(temp, "%f", voltage_to_display*current_to_display);
		Display_text(15, 10, temp, 1, true, false);
		
		sprintf(temp, "%f", voltage_to_display);
		Display_text(15, 21, temp, 1, true, false);
		
		sprintf(temp, "%f", voltage_max/10);
		Display_text(65, 0, temp, 1, true, false);
		
		sprintf(temp, "%f", voltage_to_display/100);
		Display_text(65, 10, temp, 1, true, false);
	}
	else if (page == 1)
	{
		sprintf(temp, "%f", UVLO);
		Display_text(57, 0, temp, 1, true, false);

		float temp2 = (voltage*current)/(input_voltage*input_current);
		sprintf(temp, "%f", temp2);
		Display_text(15, 10, temp, 1, true, false);
	}
	else
	{
	}
}