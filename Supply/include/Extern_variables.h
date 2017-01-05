/*
 * Extern_variables.h
 *
 * Created: 28.12.2016 10:33:04
 *  Author: Jakub
 */ 


#ifndef EXTERN_VARIABLES_H_
#define EXTERN_VARIABLES_H_

extern volatile bool reCalc;

extern uint16_t voltage;
extern uint16_t current;
extern uint16_t input_voltage;
extern uint16_t input_current;

extern uint16_t voltage_max;
extern uint16_t current_max;
extern bool src;

extern const uint8_t first[];
extern const uint8_t second[];
extern const uint8_t source[];
extern const uint8_t voltage_source[];
extern const uint8_t current_source[];
extern const uint8_t ok[];

extern uint8_t screen[512];
extern uint16_t screen_counter;
extern bool screen_send;

extern uint8_t buffer[32];
extern uint8_t buffer_counter;

extern uint8_t control;

extern int coursor_x;
extern int coursor_y;

#endif /* EXTERN_VARIABLES_H_ */