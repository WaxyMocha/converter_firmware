/*
 * Supply.cpp
 *
 * Created: 26.12.2016 19:44:35
 * Author : Jackob
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "include/Interrupts.h"
#include "include/Extern_variables.h"
#include "include/Display.h"
#include "include/Logic.h"
#include "include/Make_screen.h"

void Init ();

volatile bool reCalc = true;

int main(void)
{
    Init();
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
			make_screen ();
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
ISR (TWIC_TWIM_vect);
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
}
ISR (PORTC_INT_vect);