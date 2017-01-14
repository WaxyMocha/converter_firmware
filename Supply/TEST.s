
/*
 * TEST.asm
 *
 * Created: 2017-01-11 10:35:24
 *  Author: Jackob
 */ 
.nolist
#include "avr/io.h"
.list

.org 6
	RJMP start

start:
	PUSH R16
	PUSH R17
	PUSH R18
	PUSH R19

	 LDS R16, EDMA_CH0_ADDRL ;EDMA_CH0_ADDRL"); //Load low byte of EDMA CH0 ADDR register
	LDS R17, [281] ;EDMA_CH0_ADDRH");//Load high byte of EDMA CH0 ADDR register

	LDS R18, [current] ;//Load low byte of current variable pointer
	LDS R19, [current+1] ;//Load high byte of current variable pointer

	LDS R16, [current] ;//Compare low byte of EDMA and current
	LDS R17, [current+1] ;//Compare high byte of EDMA and current
	BREQ current //"Jump" if equal to current label

	LDS R18, [input_voltage] ; //Load low byte of input_voltage variable pointer
	LDS R19, [input_voltage+1] ;//Load high byte of input_voltage variable pointer

	CP R16, R18 ;//Compare low byte of EDMA and input_voltage
	CPC R17, R19 ;//Compare high byte of EDMA and input_voltage
	BREQ input_voltage ;//"Jump" if equal input_voltage label

	LDS R18, [voltage] ;//Load low byte of voltage variable pointer
	LDS R19, [voltage+1] ;//Load high byte of voltage variable pointer

	CP R16, R18 ;//Compare low byte of EDMA and voltage
	CPC R17, R19 ;//Compare high byte of EDMA and voltage
	BREQ voltage ;//"Jump" if equal to voltage label
	
		// If EDMA_CH0_ADDR equal input_current (all other possibilities was checked) 
	LDI R16, (0x00<<3) ; ADC_CH_MUXPOS_PIN0_gc");//Load config to register
	RJMP end ;//Jump to end label

voltage: ;//voltage label
	LDS R18, [current] ;//Load low byte of current variable pointer
	LDS R19, [current+1] ;//Load high byte of current variable pointer
	LDI R16, (0x01<<3) ; ADC_CH_MUXPOS_PIN1_gc");//Load config to register
	RJMP end ;//Jump to end label

current: ;//current label
	LDS R18, [input_voltage] ;//Load low byte of input_voltage variable pointer
	LDS R19, [input_voltage+1] ;//Load high byte of input_voltage variable pointer
	LDI R16, (0x02<<3) ; ADC_CH_MUXPOS_PIN2_gc");//Load config to register
	RJMP end ;//Jump to end label

input_voltage: ;//input_voltage label
	LDS R18, [input_current] ;//Load low byte of input_current variable pointer
	LDS R19, [input_current+1] ;//Load high byte of input_current variable pointer
	LDI R16, (0x03<<3) ; ADC_CH_MUXPOS_PIN3_gc");//Load config to register
	
end: ;//end label
	STS [545], R16 ; ADCA_CH0_MUXCTRL");//load R16 register to memory

	STS [280], R18 ; EDMA_CH0_ADDRL");//load R18 register to memory
	STS [281], R19 ; EDMA_CH0_ADDRH");//load R19 register to memory

	POP R19
	POP R18
	POP R17
	POP R16
