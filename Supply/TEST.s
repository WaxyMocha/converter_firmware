
/*
 * TEST.s
 *
 * Created: 2017-01-11 10:35:24
 *  Author: Jackob
 */ 
 IN R16, EDMA_CH0_ADDRL
 IN R17, EDMA_CH0_ADDRH

 LDS R18, [voltage]
 LDS R19, [voltage+1]

 CP R16, R18
 CPC R17, R19
 BREQ voltage

 LDS R18, [current]
 LDS R19, [curent+1]

 CP R16, R18
 CPC R17, R19
 BREQ current

 LDS R18, [input_voltage]
 LDS R19, [input_voltage+1]

 CP R16, R18
 CPC R17, R19
 BREQ input_voltage
 
 LDS R16, [voltage]
 LDS R17, [voltage+1]
 OUT EDMA_CH0_ADDRL, R16
 OUT EDMA_CH0_ADDRH, R7
 LDS R16, ADC_CH_MUXPOS_PIN0
 OUT ADCA_CH0_MUXCTRL, R16
 RJMP end

 voltage:
 LDS R16, [current]
 LDS R17, [current+1]
 OUT EDMA_CH0_ADDRL, R16
 OUT EDMA_CH0_ADDRH, R7
 LDS R16, ADC_CH_MUXPOS_PIN0
 OUT ADCA_CH0_MUXCTRL, R16
 RJMP end

 current:
 LDS R16, [input_voltage]
 LDS R17, [input_voltage+1]
 OUT EDMA_CH0_ADDRL, R16
 OUT EDMA_CH0_ADDRH, R7
 LDS R16, ADC_CH_MUXPOS_PIN0
 OUT ADCA_CH0_MUXCTRL, R16
 RJMP end

 input_voltage:
 LDS R16, [input_current]
 LDS R17, [input_current+1]
 OUT EDMA_CH0_ADDRL, R16
 OUT EDMA_CH0_ADDRH, R7
 LDS R16, ADC_CH_MUXPOS_PIN0
 OUT ADCA_CH0_MUXCTRL, R16
 
 end:
 RETI
