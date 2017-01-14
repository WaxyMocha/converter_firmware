/*
 * Supply.cpp
 *
 * Created: 26.12.2016 19:44:35
 * Author : Jackob
 */ 

#include <avr/io.h>
//#include <avr/iox32e5.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "include/Interrupts.h"
#include "include/Extern_variables.h"
#include "include/Display.h"
#include "include/Logic.h"
#include "include/Make_screen.h"
//#include "TEST.asm"

void Init ();
void start();

volatile bool reCalc = true;

int main(void)
{
    Init();
    /*
	asm(".nolist \n \t");
	//asm(".include \"TEST.asm\" \n \t");
	asm(".list \n \t");
	asm("LDS R16, [280] \n \t;EDMA_CH0_ADDRL"); //Load low byte of EDMA CH0 ADDR register
	asm("LDS R17, [281] \n  \t;EDMA_CH0_ADDRH");//Load high byte of EDMA CH0 ADDR register

	asm("LDS R18, [current] \n \t");//Load low byte of current variable pointer
	asm("LDS R19, [current+1] \n \t");//Load high byte of current variable pointer

	asm("LDS R16, [current] \n \t");//Compare low byte of EDMA and current
	asm("LDS R17, [current+1] \n \t");//Compare high byte of EDMA and current
	asm("BREQ current \n \t");//"Jump" if equal to current label

	asm("LDS R18, [input_voltage] \n \t"); //Load low byte of input_voltage variable pointer
	asm("LDS R19, [input_voltage+1] \n \t");//Load high byte of input_voltage variable pointer

	asm("CP R16, R18 \n \t");//Compare low byte of EDMA and input_voltage
	asm("CPC R17, R19 \n \t");//Compare high byte of EDMA and input_voltage
	asm("BREQ input_voltage \n \t");//"Jump" if equal input_voltage label

	asm("LDS R18, [voltage] \n \t");//Load low byte of voltage variable pointer
	asm("LDS R19, [voltage+1] \n \t");//Load high byte of voltage variable pointer

	asm("CP R16, R18 \n \t");//Compare low byte of EDMA and voltage
	asm("CPC R17, R19 \n \t");//Compare high byte of EDMA and voltage
	asm("BREQ voltage \n \t");//"Jump" if equal to voltage label
	
	// If EDMA_CH0_ADDR equal input_current (all other possibilities was checked) 
	asm("LDI R16, (0x00<<3) \n \t; ADC_CH_MUXPOS_PIN0_gc");//Load config to register
	asm("RJMP end \n \t");//Jump to end label

	asm("voltage: \n \t");//voltage label
	asm("LDS R18, [current] \n \t");//Load low byte of current variable pointer
	asm("LDS R19, [current+1] \n \t");//Load high byte of current variable pointer
	asm("LDI R16, (0x01<<3) \n \t; ADC_CH_MUXPOS_PIN1_gc");//Load config to register
	asm("RJMP end \n \t");//Jump to end label

	asm("current: \n \t");//current label
	asm("LDS R18, [input_voltage] \n \t");//Load low byte of input_voltage variable pointer
	asm("LDS R19, [input_voltage+1] \n \t");//Load high byte of input_voltage variable pointer
	asm("LDI R16, (0x02<<3) \n \t; ADC_CH_MUXPOS_PIN2_gc");//Load config to register
	asm("RJMP end \n \t");//Jump to end label

	asm("input_voltage: \n \t");//input_voltage label
	asm("LDS R18, [input_current] \n \t");//Load low byte of input_current variable pointer
	asm("LDS R19, [input_current+1] \n \t");//Load high byte of input_current variable pointer
	asm("LDI R16, (0x03<<3) \n \t; ADC_CH_MUXPOS_PIN3_gc");//Load config to register
	
	asm("end: \n");//end label
	asm("STS [545], R16 \n \t; ADCA_CH0_MUXCTRL");//load R16 register to memory

	asm("STS [280], R18 \n \t; EDMA_CH0_ADDRL");//load R18 register to memory
	asm("STS [281], R19 \n \t; EDMA_CH0_ADDRH");//load R19 register to memory
	*/
	bool reMake = true;
    while (1) 
    {
		if (reCalc)
		{
			//reMake = true;
			//reCalc = false;
			PWM_control ();
		}
		if (TCC5.CNT != 0)
		{
			reMake = true;
			TCC5.CNT = 0;
			Encoder_logic ();
		}
		if (reMake)
		{
			reMake = false;
			//make_screen ();
		}

    }
}

void Init ()
{
	PMIC.CTRL = PMIC_RREN_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;

 	//PORTC.INTCTRL = PORT_INTLVL_LO_gc; //Need to check if flag will be set
 	PORTC.INTMASK |= PIN2_bm;
	PORTC.PIN0CTRL = PORT_ISC_LEVEL_gc;
	PORTC.PIN1CTRL = PORT_ISC_LEVEL_gc;
	PORTC.PIN3CTRL = PORT_OPC_WIREDAND_gc;
	PORTC.PIN4CTRL = PORT_OPC_WIREDAND_gc;

	PORTD.DIRSET |= PIN4_bm;
	PORTD.DIRSET |= PIN5_bm;
	PORTD.DIRSET |= PIN7_bm;
	PORTD.PIN4CTRL |= PORT_OPC_PULLUP_gc | PORT_INVEN_bm;
	PORTD.PIN5CTRL |= PORT_OPC_PULLDOWN_gc;
	PORTD.PIN7CTRL |= PORT_OPC_PULLDOWN_gc;

	TCC5.CTRLA = TC5_CLKSEL3_bm;
	TCC5.CTRLD = TC5_EVACT2_bm;

	TCD5.CTRLB = 5;
	TCD5.CTRLC |= TC5_CMPA_bm;
	TCD5.CTRLE |= TC5_CCAMODE0_bm |  TC5_CCBMODE0_bm;
	TCD5.CTRLA = TC5_CLKSEL2_bm; //div1
	TCD5.PER = 255;

	ADCA.CTRLA |= ADC_ENABLE_bm;
	ADCA.REFCTRL |= ADC_BANDGAP_bm;
	ADCA.PRESCALER = ADC_PRESCALER0_bm | ADC_PRESCALER1_bm;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE0_bm;
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH0.AVGCTRL = ADC_CH_SAMPNUM1_bm | ADC_CH_SAMPNUM3_bm;

	EDMA.CTRL |= EDMA_PRIMODE_RR123_gc;
	EDMA.INTFLAGS |= EDMA_CH0TRNFIF_bm;
	EDMA.CH0.CTRLA |= EDMA_CH_BURSTLEN_bm;
	EDMA.CH0.CTRLB |= EDMA_CH_TRNINTLVL0_bm;
	EDMA.CH0.ADDRCTRL |= EDMA_CH_RELOAD0_bm;
	EDMA.CH0.TRIGSRC |= EDMA_CH_TRIGSRC_ADCA_CH0_gc;
	EDMA.CH0.ADDR = (uint16_t)&voltage;
	EDMA.CH0.CTRLA |= EDMA_CH_ENABLE_bm;
	EDMA.CTRL |= EDMA_ENABLE_bm;

	EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN0_gc;
	EVSYS.CH0CTRL |= EVSYS_QDEN_bm | EVSYS_DIGFILT0_bm | EVSYS_DIGFILT1_bm | EVSYS_DIGFILT2_bm;

	TWIC.MASTER.BAUD = 35;//400 kHz
	TWIC.MASTER.ADDR = SSD1306_I2C_ADDRESS;
	TWIC.MASTER.CTRLA = TWI_MASTER_INTLVL_LO_gc | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
	TWIC.MASTER.ADDR = SSD1306_I2C_ADDRESS;

	RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
	
	CLK.CTRL = CLK_SCLKSEL0_bm;
	CLK.RTCCTRL |= CLK_RTCSRC1_bm | CLK_RTCEN_bm;

	PR.PRGEN = PR_XCL_bm;
	PR.PRPA = PR_DAC_bm | PR_AC_bm;
	PR.PRPC = PR_USART0_bm | PR_SPI_bm | PR_HIRES_bm;
	PR.PRPD = PR_USART0_bm;

	OSC.CTRL = OSC_RC32MEN_bm;
	while (OSC.STATUS & !OSC_RC32MRDY_bm);

	Display_init();

	sei();

	ADCA.CTRLA |= ADC_START_bm;
}
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
/*
ISR(EDMA_CH0_vect)
{
	if (EDMA.CH0.ADDR == (uint16_t)&voltage)
	{
		reCalc = true;
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
}*/