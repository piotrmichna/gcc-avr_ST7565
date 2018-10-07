/*
 * pmST7565.h
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 *
 *      cup: Atmega328P
 *      F_CPU 14.7456 Mhz
 */

#ifndef PMST7565_PMST7565_H_
#define PMST7565_PMST7565_H_

#define LCD_ST7565

#define INTERFACE 0		// 0-PARALE 1-SERIAL
// optional pin config
#define USE_RST 1	// 0 - pullup pin RST to VCC
#define USE_RD 1	// 0 - pullup pin RD to Vcc
#define USE_WR 1	// 0 - pullup pin WR to GND

// ---------------------- PARALE ------------------
#if INTERFACE == 0
	#define D0_PIN PD0
	#define D1_PIN PD1
	#define D2_PIN PD2
	#define D3_PIN PD3
	#define D4_PIN PD4
	#define D5_PIN PD5
	#define D6_PIN PD6
	#define D7_PIN PD7

	#define D0_PORT D
	#define D1_PORT D
	#define D2_PORT D
	#define D3_PORT D
	#define D4_PORT D
	#define D5_PORT D
	#define D6_PORT D
	#define D7_PORT D

	//CS Port
	#define CS_PIN		PC3
	#define CS_PORT		C

	// A0 Port
	#define A0_PIN   	PC4
	#define A0_PORT  	C

	//Reset Port
	#if USE_RST == 1
		#define RST_PIN  	PC0
		#define RST_PORT 	C
	#endif

	// WR Port
	#if USE_WR == 1
		#define WR_PIN  	PC2
		#define WR_PORT 	C
	#endif

	// RD Port
	#if USE_RD == 1
		#define RD_PIN  	PC1
		#define RD_PORT 	C
	#endif
#endif

// ---------------------- SERIAL ------------------
#if INTERFACE == 1
	//SPI Port
	#define SI_PIN		PD7
	#define SCK_PIN		PD6
	#define CS_PIN		PC3

	#define SI_PORT		D
	#define SCK_PORT	D
	#define CS_PORT		C

	// A0 Port
	#define A0_PIN   	PC4
	#define A0_PORT  	C

	//Reset Port
	#if USE_RST == 1
		#define RST_PIN  	PC0
		#define RST_PORT 	C
	#endif

	// WR Port
	#if USE_WR == 1
		#define WR_PIN  	PC2
		#define WR_PORT 	C
	#endif

	// RD Port
	#if USE_RD == 1
		#define RD_PIN  	PC1
		#define RD_PORT 	C
	#endif

#endif


//write to lc-display command or data register
#define CMD		1
#define DATA	0

#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define LCD_PAGE_NUM 8
#define LCD_COL_NUM 132

//Befehlstabelle EA DOGM128-6 Seite 5
// (1) Display ON/OFF
#define DISPLAY_ON       			0xAF  //LCD_DISPLAY_ON
#define DISPLAY_OFF      			0xAE  //LCD_DISPLAY_OFF

// (2) Display start line set
#define DISPLAY_START_LINE_SET		0x40

// (3) Page address set
#define DISPLAY_PAGE_ADDRESS		0xB0

// (4) Column address set upper bit
#define DISPLAY_COL_ADDRESS_MSB		0x10
// (4) Column address set lower bit
#define DISPLAY_COL_ADDRESS_LSB		0x00

// (8) ADC select
#define DISPLAY_BOTTOMVIEW			0xA0
#define DISPLAY_TOPVIEW				0xA1

// (9) Display normale/reverse
#define DISPLAY_NORMAL   			0xA6
#define DISPLAY_REVERSE				0xA7

// (10) Display all points ON/OFF
#define DISPLAY_SHOW_NORMAL			0xA4
#define DISPLAY_SHOW_ALL_POINTS		0xA5

// (11) LCD bias set
#define DISPLAY_BIAS_1_9			0xA2
#define DISPLAY_BIAS_1_7			0xA3

// (14) RESET
#define DISPLAY_RESET_CMD			0xE2

// (15) Common output mode select
#define DISPLAY_SCAN_DIR_NORMAL		0xC0
#define DISPLAY_SCAN_DIR_REVERSE	0xC8

// (16) Power control set
#define DISPLAY_POWER_CONTROL		0x28
#define DISPLAY_POWER_LOW_POWER		0x2F
#define DISPLAY_POWER_WIDE_RANGE	0x2E
#define DISPLAY_POWER_LOW_VOLTAGE	0x2B

// (17) V0 voltage regulator internal resistor ratio set
#define DISPLAY_VOLTAGE          	0x25

// (18) Electronic volume mode set
#define DISPLAY_VOLUME_MODE_SET   	0x81
// (18) Register
#define DISPLAY_VOLUME_MODE	    	0x21

// (19) Static indicator ON/OFF
#define DISPLAY_INDICATOR_ON       	0xAD
#define DISPLAY_INDICATOR_OFF      	0xAC
// (19) Static indicator register set
#define DISPLAY_INDICATOR_MODE_OFF 	0x00
#define DISPLAY_INDICATOR_MODE_1HZ 	0x01
#define DISPLAY_INDICATOR_MODE_2HZ 	0x10
#define DISPLAY_INDICATOR_MODE_ON  	0x11

// (20) Booster ratio set
#define DISPLAY_BOOSTER_SET      	0xF8
#define DISPLAY_BOOSTER_234      	0x00
#define DISPLAY_BOOSTER_5        	0x01
#define DISPLAY_BOOSTER_6        	0x03

// (22) NOP
#define LCD_NOP              		0xE3

void st7565_init(void);
void st7565_clr(void);
void st7565_clr_to_end(uint8_t y, uint8_t x);
void st7565_setPos(uint8_t y, uint8_t x);
void st7565_interface_write( uint8_t comand, uint8_t data );

#endif /* PMST7565_PMST7565_H_ */
