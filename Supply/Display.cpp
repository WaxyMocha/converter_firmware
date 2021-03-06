/*
 * Display.cpp
 *
 * Created: 27.12.2016 02:08:33
 *  Author: Jakub
 * build on the basis of adafruit library 
 */ 
 #include <avr/pgmspace.h>
 #include <avr/interrupt.h>
 #include <stdlib.h>
 #include "include/Extern_variables.h"
 #include "include/Display.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

const uint8_t FreeSans9pt7bBitmaps[] PROGMEM = {
	0xFF, 0xFF, 0xF8, 0xC0, 0xDE, 0xF7, 0x20, 0x09, 0x86, 0x41, 0x91, 0xFF,
	0x13, 0x04, 0xC3, 0x20, 0xC8, 0xFF, 0x89, 0x82, 0x61, 0x90, 0x10, 0x1F,
	0x14, 0xDA, 0x3D, 0x1E, 0x83, 0x40, 0x78, 0x17, 0x08, 0xF4, 0x7A, 0x35,
	0x33, 0xF0, 0x40, 0x20, 0x38, 0x10, 0xEC, 0x20, 0xC6, 0x20, 0xC6, 0x40,
	0xC6, 0x40, 0x6C, 0x80, 0x39, 0x00, 0x01, 0x3C, 0x02, 0x77, 0x02, 0x63,
	0x04, 0x63, 0x04, 0x77, 0x08, 0x3C, 0x0E, 0x06, 0x60, 0xCC, 0x19, 0x81,
	0xE0, 0x18, 0x0F, 0x03, 0x36, 0xC2, 0xD8, 0x73, 0x06, 0x31, 0xE3, 0xC4,
	0xFE, 0x13, 0x26, 0x6C, 0xCC, 0xCC, 0xC4, 0x66, 0x23, 0x10, 0x8C, 0x46,
	0x63, 0x33, 0x33, 0x32, 0x66, 0x4C, 0x80, 0x25, 0x7E, 0xA5, 0x00, 0x30,
	0xC3, 0x3F, 0x30, 0xC3, 0x0C, 0xD6, 0xF0, 0xC0, 0x08, 0x44, 0x21, 0x10,
	0x84, 0x42, 0x11, 0x08, 0x00, 0x3C, 0x66, 0x42, 0xC3, 0xC3, 0xC3, 0xC3,
	0xC3, 0xC3, 0xC3, 0x42, 0x66, 0x3C, 0x11, 0x3F, 0x33, 0x33, 0x33, 0x33,
	0x30, 0x3E, 0x31, 0xB0, 0x78, 0x30, 0x18, 0x1C, 0x1C, 0x1C, 0x18, 0x18,
	0x10, 0x08, 0x07, 0xF8, 0x3C, 0x66, 0xC3, 0xC3, 0x03, 0x06, 0x1C, 0x07,
	0x03, 0xC3, 0xC3, 0x66, 0x3C, 0x0C, 0x18, 0x71, 0x62, 0xC9, 0xA3, 0x46,
	0xFE, 0x18, 0x30, 0x60, 0xC0, 0x7F, 0x20, 0x10, 0x08, 0x08, 0x07, 0xF3,
	0x8C, 0x03, 0x01, 0x80, 0xF0, 0x6C, 0x63, 0xE0, 0x1E, 0x31, 0x98, 0x78,
	0x0C, 0x06, 0xF3, 0x8D, 0x83, 0xC1, 0xE0, 0xD0, 0x6C, 0x63, 0xE0, 0xFF,
	0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x18, 0x18, 0x18, 0x10, 0x30, 0x30,
	0x3E, 0x31, 0xB0, 0x78, 0x3C, 0x1B, 0x18, 0xF8, 0xC6, 0xC1, 0xE0, 0xF0,
	0x6C, 0x63, 0xE0, 0x3C, 0x66, 0xC2, 0xC3, 0xC3, 0xC3, 0x67, 0x3B, 0x03,
	0x03, 0xC2, 0x66, 0x3C, 0xC0, 0x00, 0x30, 0xC0, 0x00, 0x00, 0x64, 0xA0,
	0x00, 0x81, 0xC7, 0x8E, 0x0C, 0x07, 0x80, 0x70, 0x0E, 0x01, 0x80, 0xFF,
	0x80, 0x00, 0x1F, 0xF0, 0x00, 0x70, 0x0E, 0x01, 0xC0, 0x18, 0x38, 0x71,
	0xC0, 0x80, 0x00, 0x3E, 0x31, 0xB0, 0x78, 0x30, 0x18, 0x18, 0x38, 0x18,
	0x18, 0x0C, 0x00, 0x00, 0x01, 0x80, 0x03, 0xF0, 0x06, 0x0E, 0x06, 0x01,
	0x86, 0x00, 0x66, 0x1D, 0xBB, 0x31, 0xCF, 0x18, 0xC7, 0x98, 0x63, 0xCC,
	0x31, 0xE6, 0x11, 0xB3, 0x99, 0xCC, 0xF7, 0x86, 0x00, 0x01, 0x80, 0x00,
	0x70, 0x40, 0x0F, 0xE0, 0x06, 0x00, 0xF0, 0x0F, 0x00, 0x90, 0x19, 0x81,
	0x98, 0x10, 0x83, 0x0C, 0x3F, 0xC2, 0x04, 0x60, 0x66, 0x06, 0xC0, 0x30,
	0xFF, 0x18, 0x33, 0x03, 0x60, 0x6C, 0x0D, 0x83, 0x3F, 0xC6, 0x06, 0xC0,
	0x78, 0x0F, 0x01, 0xE0, 0x6F, 0xF8, 0x1F, 0x86, 0x19, 0x81, 0xA0, 0x3C,
	0x01, 0x80, 0x30, 0x06, 0x00, 0xC0, 0x68, 0x0D, 0x83, 0x18, 0x61, 0xF0,
	0xFF, 0x18, 0x33, 0x03, 0x60, 0x3C, 0x07, 0x80, 0xF0, 0x1E, 0x03, 0xC0,
	0x78, 0x0F, 0x03, 0x60, 0xCF, 0xF0, 0xFF, 0xE0, 0x30, 0x18, 0x0C, 0x06,
	0x03, 0xFD, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0F, 0xF8, 0xFF, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xFE, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x0F, 0x83,
	0x0E, 0x60, 0x66, 0x03, 0xC0, 0x0C, 0x00, 0xC1, 0xFC, 0x03, 0xC0, 0x36,
	0x03, 0x60, 0x73, 0x0F, 0x0F, 0x10, 0xC0, 0x78, 0x0F, 0x01, 0xE0, 0x3C,
	0x07, 0x80, 0xFF, 0xFE, 0x03, 0xC0, 0x78, 0x0F, 0x01, 0xE0, 0x3C, 0x06,
	0xFF, 0xFF, 0xFF, 0xC0, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC1, 0x83, 0x07,
	0x8F, 0x1E, 0x27, 0x80, 0xC0, 0xD8, 0x33, 0x0C, 0x63, 0x0C, 0xC1, 0xB8,
	0x3F, 0x07, 0x30, 0xC3, 0x18, 0x63, 0x06, 0x60, 0x6C, 0x0C, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xE0,
	0x3F, 0x01, 0xFC, 0x1F, 0xE0, 0xFD, 0x05, 0xEC, 0x6F, 0x63, 0x79, 0x13,
	0xCD, 0x9E, 0x6C, 0xF1, 0x47, 0x8E, 0x3C, 0x71, 0x80, 0xE0, 0x7C, 0x0F,
	0xC1, 0xE8, 0x3D, 0x87, 0x98, 0xF1, 0x1E, 0x33, 0xC3, 0x78, 0x6F, 0x07,
	0xE0, 0x7C, 0x0E, 0x0F, 0x81, 0x83, 0x18, 0x0C, 0xC0, 0x6C, 0x01, 0xE0,
	0x0F, 0x00, 0x78, 0x03, 0xC0, 0x1B, 0x01, 0x98, 0x0C, 0x60, 0xC0, 0xF8,
	0x00, 0xFF, 0x30, 0x6C, 0x0F, 0x03, 0xC0, 0xF0, 0x6F, 0xF3, 0x00, 0xC0,
	0x30, 0x0C, 0x03, 0x00, 0xC0, 0x00, 0x0F, 0x81, 0x83, 0x18, 0x0C, 0xC0,
	0x6C, 0x01, 0xE0, 0x0F, 0x00, 0x78, 0x03, 0xC0, 0x1B, 0x01, 0x98, 0x6C,
	0x60, 0xC0, 0xFB, 0x00, 0x08, 0xFF, 0x8C, 0x0E, 0xC0, 0x6C, 0x06, 0xC0,
	0x6C, 0x0C, 0xFF, 0x8C, 0x0E, 0xC0, 0x6C, 0x06, 0xC0, 0x6C, 0x06, 0xC0,
	0x70, 0x3F, 0x18, 0x6C, 0x0F, 0x03, 0xC0, 0x1E, 0x01, 0xF0, 0x0E, 0x00,
	0xF0, 0x3C, 0x0D, 0x86, 0x3F, 0x00, 0xFF, 0x86, 0x03, 0x01, 0x80, 0xC0,
	0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x80, 0xC0, 0xC0, 0x78, 0x0F,
	0x01, 0xE0, 0x3C, 0x07, 0x80, 0xF0, 0x1E, 0x03, 0xC0, 0x78, 0x0F, 0x01,
	0xB0, 0x61, 0xF0, 0xC0, 0x6C, 0x0D, 0x81, 0x10, 0x63, 0x0C, 0x61, 0x04,
	0x60, 0xCC, 0x19, 0x01, 0x60, 0x3C, 0x07, 0x00, 0x60, 0xC1, 0x81, 0x30,
	0xE1, 0x98, 0x70, 0xCC, 0x28, 0x66, 0x26, 0x21, 0x13, 0x30, 0xC8, 0x98,
	0x6C, 0x4C, 0x14, 0x34, 0x0A, 0x1A, 0x07, 0x07, 0x03, 0x03, 0x80, 0x81,
	0x80, 0x60, 0x63, 0x0C, 0x30, 0xC1, 0x98, 0x0F, 0x00, 0xE0, 0x06, 0x00,
	0xF0, 0x19, 0x01, 0x98, 0x30, 0xC6, 0x0E, 0x60, 0x60, 0xC0, 0x36, 0x06,
	0x30, 0xC3, 0x0C, 0x19, 0x81, 0xD8, 0x0F, 0x00, 0x60, 0x06, 0x00, 0x60,
	0x06, 0x00, 0x60, 0x06, 0x00, 0xFF, 0xC0, 0x60, 0x30, 0x0C, 0x06, 0x03,
	0x01, 0xC0, 0x60, 0x30, 0x18, 0x06, 0x03, 0x00, 0xFF, 0xC0, 0xFB, 0x6D,
	0xB6, 0xDB, 0x6D, 0xB6, 0xE0, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84,
	0x10, 0x80, 0xED, 0xB6, 0xDB, 0x6D, 0xB6, 0xDB, 0xE0, 0x30, 0x60, 0xA2,
	0x44, 0xD8, 0xA1, 0x80, 0xFF, 0xC0, 0xC6, 0x30, 0x7E, 0x71, 0xB0, 0xC0,
	0x60, 0xF3, 0xDB, 0x0D, 0x86, 0xC7, 0x3D, 0xC0, 0xC0, 0x60, 0x30, 0x1B,
	0xCE, 0x36, 0x0F, 0x07, 0x83, 0xC1, 0xE0, 0xF0, 0x7C, 0x6D, 0xE0, 0x3C,
	0x66, 0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC3, 0x66, 0x3C, 0x03, 0x03, 0x03,
	0x3B, 0x67, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x67, 0x3B, 0x3C, 0x66,
	0xC3, 0xC3, 0xFF, 0xC0, 0xC0, 0xC3, 0x66, 0x3C, 0x36, 0x6F, 0x66, 0x66,
	0x66, 0x66, 0x60, 0x3B, 0x67, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x67,
	0x3B, 0x03, 0x03, 0xC6, 0x7C, 0xC0, 0xC0, 0xC0, 0xDE, 0xE3, 0xC3, 0xC3,
	0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xC0, 0x30, 0x03,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xE0, 0xC0, 0x60, 0x30, 0x18, 0x4C,
	0x46, 0x63, 0x61, 0xF0, 0xEC, 0x62, 0x31, 0x98, 0x6C, 0x30, 0xFF, 0xFF,
	0xFF, 0xC0, 0xDE, 0xF7, 0x1C, 0xF0, 0xC7, 0x86, 0x3C, 0x31, 0xE1, 0x8F,
	0x0C, 0x78, 0x63, 0xC3, 0x1E, 0x18, 0xC0, 0xDE, 0xE3, 0xC3, 0xC3, 0xC3,
	0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x3C, 0x66, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
	0xC3, 0x66, 0x3C, 0xDE, 0x71, 0xB0, 0x78, 0x3C, 0x1E, 0x0F, 0x07, 0x83,
	0xE3, 0x6F, 0x30, 0x18, 0x0C, 0x00, 0x3B, 0x67, 0xC3, 0xC3, 0xC3, 0xC3,
	0xC3, 0xC3, 0x67, 0x3B, 0x03, 0x03, 0x03, 0xDF, 0x31, 0x8C, 0x63, 0x18,
	0xC6, 0x00, 0x3E, 0xE3, 0xC0, 0xC0, 0xE0, 0x3C, 0x07, 0xC3, 0xE3, 0x7E,
	0x66, 0xF6, 0x66, 0x66, 0x66, 0x67, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
	0xC3, 0xC3, 0xC7, 0x7B, 0xC1, 0xA0, 0x98, 0xCC, 0x42, 0x21, 0xB0, 0xD0,
	0x28, 0x1C, 0x0C, 0x00, 0xC6, 0x1E, 0x38, 0x91, 0xC4, 0xCA, 0x66, 0xD3,
	0x16, 0xD0, 0xA6, 0x87, 0x1C, 0x38, 0xC0, 0xC6, 0x00, 0x43, 0x62, 0x36,
	0x1C, 0x18, 0x1C, 0x3C, 0x26, 0x62, 0x43, 0xC1, 0x21, 0x98, 0xCC, 0x42,
	0x61, 0xB0, 0xD0, 0x38, 0x1C, 0x0C, 0x06, 0x03, 0x01, 0x03, 0x00, 0xFE,
	0x0C, 0x30, 0xC1, 0x86, 0x18, 0x20, 0xC1, 0xFC, 0x36, 0x66, 0x66, 0x6E,
	0xCE, 0x66, 0x66, 0x66, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xC6, 0x66,
	0x66, 0x67, 0x37, 0x66, 0x66, 0x66, 0xC0, 0x61, 0x24, 0x38 
	};
uint8_t screen[512] = {
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0xEE,0x00,0x00,0x01,0xC7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x44,0x00,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x44,0x00,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x28,0x00,0x00,0x00,0xAA,0x33,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x28,0x00,0x00,0x00,0xAA,0x09,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x28,0x00,0x00,0x00,0xAA,0x38,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x10,0x00,0x00,0x00,0x92,0x49,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x10,0x00,0x00,0x01,0xD7,0x3F,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x10,0x00,0x00,0x01,0xC7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x10,0x00,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x28,0x00,0x00,0x00,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x28,0x00,0x00,0x00,0xAA,0x33,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x28,0x00,0x00,0x00,0xAA,0x09,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x7C,0x00,0x00,0x00,0xAA,0x38,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x44,0x00,0x00,0x00,0x92,0x49,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0xEE,0x00,0x00,0x01,0xD7,0x3F,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x67,0x00,0x00,
	 0x00,0x00,0x00,0x12,0x40,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0x00,0x04,
	 0x00,0x00,0x00,0x10,0x40,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x04,
	 0x00,0x00,0x00,0x08,0xE6,0x7D,0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0x33,0x6E,
	 0x00,0x00,0x00,0x04,0x41,0x24,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x2A,0x49,0x44,
	 0x00,0x00,0x00,0x02,0x47,0x24,0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x78,0x84,
	 0x00,0x00,0x00,0x12,0x49,0x24,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0x41,0x44,
	 0x00,0x00,0x00,0x1C,0x67,0xB3,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x72,0x3B,0x66,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 };
uint16_t screen_counter = 0;
bool screen_send = true;

uint8_t buffer[32];
uint8_t buffer_counter = 0;

uint8_t control = 0;

int coursor_x;
int coursor_y;

void command(uint8_t c)
{
	buffer_counter++;
	buffer[buffer_counter] = c;
}
void Display_init ()
{
	command(SSD1306_DISPLAYOFF);                    // 0xAE
	command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	command(0x80);                                  // the suggested ratio 0x80

	command(SSD1306_SETMULTIPLEX);                  // 0xA8
	command(SSD1306_LCDHEIGHT - 1);

	command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	command(0x0);                                   // no offset
	command(SSD1306_SETSTARTLINE | 0x0);            // line #0
	command(SSD1306_CHARGEPUMP); // 0x8D
	command(0x14);
	command(SSD1306_MEMORYMODE);                    // 0x20
	command(0x00);                                  // 0x0 act like ks0108
	command(SSD1306_SEGREMAP | 0x1);
	command(SSD1306_COMSCANDEC);
	command(SSD1306_SETCOMPINS);                    // 0xDA
	command(0x02);
	command(SSD1306_SETCONTRAST);                   // 0x81
	command(0x8F);
	command(SSD1306_SETPRECHARGE);
	command(0xF1);
	command(SSD1306_SETVCOMDETECT);                 // 0xDB
	command(0x40);
	command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
	command(SSD1306_NORMALDISPLAY);                 // 0xA6

	command(SSD1306_DEACTIVATE_SCROLL);

	command(SSD1306_DISPLAYON);//--turn on oled panel
}
void Display_invert(bool i)
{
	if (i) {
		command(SSD1306_INVERTDISPLAY);
	} else {
		command(SSD1306_NORMALDISPLAY);
	}
}
void Display_scroll (uint8_t start, uint8_t stop, bool right = true)
{
	if (right)
	{
		command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
	} 
	else
	{
		command(SSD1306_LEFT_HORIZONTAL_SCROLL);
	}
	command(0X00);
	command(start);
	command(0X00);
	command(stop);
	command(0X00);
	command(0XFF);
	command(SSD1306_ACTIVATE_SCROLL);
}
void Display_diagonal_scroll (uint8_t start, uint8_t stop, bool right = true)
{
	command(SSD1306_SET_VERTICAL_SCROLL_AREA);
	command(0X00);
	command(SSD1306_LCDHEIGHT);
	if (right)
	{
		command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	} 
	else
	{
		command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	}
	command(0X00);
	command(start);
	command(0X00);
	command(stop);
	command(0X01);
	command(SSD1306_ACTIVATE_SCROLL);
}
void Display_scroll_stop ()
{
	command(SSD1306_DEACTIVATE_SCROLL);
}
void Display_dim (bool dim)
{
	uint8_t contrast;

	if (dim) 
	{
		contrast = 0; // Dimmed display
	} 
	else 
	{
		contrast = 0xCF;
	}
	command(SSD1306_SETCONTRAST);
	command(contrast);
}
void Display_pixel(int x, int y, bool enable = true)
{
	if ((x < 0) || (x >= SSD1306_LCDWIDTH) || (y < 0) || (y >= SSD1306_LCDHEIGHT))
	{
		return;
	}
	if (enable)
	{
		screen[x + (y/8)*SSD1306_LCDWIDTH] |= (1<<(y%8));
	} 
	else
	{
		screen[x + (y/8)*SSD1306_LCDWIDTH] &= ~(1<<(y%8));
	}
}
void Display_send ()
{
	screen_send = true;
	control++;
	TWIC.MASTER.DATA = SSD1306_I2C_ADDRESS;// start twi transfer
}

void Display_line(int x0, int y0, int x1, int y1, bool enable)
{
	int steep = abs(y1 - y0) > abs(x1-x0);
	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}
	int dx, dy;
	dx = x1-x0;
	dy = abs(y1 - y0);

	int err = dx / 2;
	int ystep;

	if (y0 < y1)
	{
		ystep = -1;
	} 
	else
	{
		ystep = 1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			Display_pixel(y0, x0, enable);
		} 
		else
		{
			Display_pixel(x0, y0, enable);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}
void Display_veritcal_line(int x, int y, int h, bool enable)
{
	Display_line(x, y, x, y+h-1, enable);
}
void Display_horizontal_line(int x, int y, int w, bool enable)
{
	Display_line(x, y, x+w-1, y, enable);
}
void Display_bitmap_P (int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, bool enable = true)
{
	int byteWidth = (w+7)/8;

	for (int i = 0; i<h; i++)
	{
		for (int j = 0; j<w; j++)
		{
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))
			{
				Display_pixel(x+j, y+i, enable);
			}
		}
	}
}
void Display_fill_screen (bool enable)
{
	Display_fill_rect(0, 0, 128, 32, enable);
}

void Display_char(int x, int y, char c, bool enable, bool background, uint8_t size)
{
	if ((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT) || ((x+6*size-1) < 0) || ((y+8*size-1) < 0)) return;

	uint8_t line;

	for (int8_t i=0; i<6; i++)
	{
		if (i == 5)
		{
			line = 0x0;
		}
		else
		{
			line = pgm_read_byte(FreeSans9pt7bBitmaps+(c*5)+1);
		}
		for (int8_t j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (size == 1)
				{
					Display_pixel(x+i, y+j, enable);
				}
				else
				{
					Display_fill_rect(x+(i*size), y+(j*size), size, size, enable);
				}
			}
			else if (background != enable)
			{
				if (size == 1)
				{
					Display_pixel(x+i, y+j, background);
				}
				else
				{
					Display_fill_rect(x+i*size, y+j*size, size, size, background);
				}
			}
			line >>= 1;
		}
	}
}
void Display_text (int x, int y, char * str, uint8_t txt_size, bool eanable, bool background)
{
	coursor_x = x;
	coursor_y = y;
	while (*str)
	{
		Display_char(coursor_x, coursor_y, *str++, eanable, background, txt_size);
		coursor_x += txt_size*6;
	}
}
void Display_int (int x, int y, int data, uint8_t txt_size, bool enable, bool background)
{
	char buf[16];
	Display_text(x, y, itoa(data, buf, 10), txt_size, enable, background);
}

void Display_fill_rect (int x, int y, int w, int h, bool enable)
{
	for (int16_t i = x; i<x+w; i++)
	{
		Display_veritcal_line(i, y, h, enable);
	}
}
void Display_rect (int x, int y, int w, int h, bool enable)
{
	Display_horizontal_line(x, y, w, enable);
	Display_horizontal_line(x, y+h-1, w, enable);
	Display_veritcal_line(x, y, h, enable);
	Display_veritcal_line(x+w-1, y, h, enable);
}