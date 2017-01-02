/*
 * Make_screen.cpp
 *
 * Created: 28.12.2016 10:59:36
 *  Author: Jakub
 */ 
#include <avr/io.h>
#include "include/Make_screen.h"
#include "include/Extern_variables.h"
#include "include/Display.h"

float voltage_to_display = 0;
float current_to_display = 0;

void make_screen ()
{
	voltage_to_display = (voltage/4096)*15;
	current_to_display = (current/4096);
	Display_fill_rect(9, 1, 28, 18, false); // clear
	Display_fill_rect(53, 1, 72, 18, false);


}