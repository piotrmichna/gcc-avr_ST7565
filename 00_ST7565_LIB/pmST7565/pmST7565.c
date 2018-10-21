/*
 * pmST7565.c
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 *
 *      uc: Atmega328P
 *      F_CPU 14.7456 Mhz
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../mac.h"
#include "pmST7565.h"


#if USE_BUFF == 1
#define BUFF_PAGE_NUM (LCD_HEIGHT/8)
TCOL st7565_buffer[BUFF_PAGE_NUM];
#endif

void st7565_init(void){
	DDR( CS_PORT ) |= (1<<CS_PIN);
	DDR( A0_PORT ) |= (1<<A0_PIN);

	PORT(CS_PORT) |= (1<<CS_PIN);
	PORT(A0_PORT) |= (1<<A0_PIN);

	#if USE_RST == 1
		DDR( RST_PORT ) |= (1<<RST_PIN);
		PORT(RST_PORT) |= (1<<RST_PIN);
	#endif
	#if USE_WR == 1
		DDR( WR_PORT ) |= (1<<WR_PIN);
		PORT(WR_PORT) &=~(1<<WR_PIN);
	#endif

	#if USE_RD == 1
		DDR( RD_PORT ) |= (1<<RD_PIN);
		PORT(RD_PORT) |= (1<<RD_PIN);
	#endif

// ---------------------- PARALE ------------------
	#if INTERFACE == 0
		DDR( D0_PORT ) |= (1<<D0_PIN);
		DDR( D1_PORT ) |= (1<<D1_PIN);
		DDR( D2_PORT ) |= (1<<D2_PIN);
		DDR( D3_PORT ) |= (1<<D3_PIN);
		DDR( D4_PORT ) |= (1<<D4_PIN);
		DDR( D5_PORT ) |= (1<<D5_PIN);
		DDR( D6_PORT ) |= (1<<D6_PIN);
		DDR( D7_PORT ) |= (1<<D7_PIN);

		PORT( D0_PORT ) |= (1<<D0_PIN);
		PORT( D1_PORT ) |= (1<<D1_PIN);
		PORT( D2_PORT ) |= (1<<D2_PIN);
		PORT( D3_PORT ) |= (1<<D3_PIN);
		PORT( D4_PORT ) |= (1<<D4_PIN);
		PORT( D5_PORT ) |= (1<<D5_PIN);
		PORT( D6_PORT ) |= (1<<D6_PIN);
		PORT( D7_PORT ) |= (1<<D7_PIN);
	#endif
// ------------------------------------------------

// ---------------------- SERIAL ------------------
	#if INTERFACE == 1
		DDR( SI_PORT ) |= (1<<SI_PIN);
		DDR( SCK_PORT ) |= (1<<SCK_PIN);

		PORT(SCK_PORT) &= ~(1<<SCK_PIN);
	#endif
// ------------------------------------------------

	#if USE_RST == 1
		//Reset the Display Controller
		PORT(RST_PORT) &= ~(1<<RST_PIN);
		_delay_ms(50);
		PORT(RST_PORT) |= (1<<RST_PIN);
	#endif

	// (15) Common output mode select
	st7565_interface_write(CMD,DISPLAY_SCAN_DIR_REVERSE);

	// (8) ADC Select
	st7565_interface_write(CMD,DISPLAY_BOTTOMVIEW);

	// (16) Power control set
	st7565_interface_write(CMD,DISPLAY_POWER_LOW_POWER);

	// (18) Electronic volume mode set
	st7565_interface_write(CMD,DISPLAY_VOLUME_MODE_SET);
	st7565_interface_write(CMD,DISPLAY_VOLUME_MODE);

	// (17) V0 voltage regulator internal resistor ratio set
	st7565_interface_write(CMD,DISPLAY_VOLTAGE);

	//(11) LCD Bias set
	st7565_interface_write(CMD,DISPLAY_BIAS_1_9);

	// (9) Display normale/reverse
	st7565_interface_write(CMD,DISPLAY_NORMAL);

	// (19) Static indicator ON/OFF
	st7565_interface_write(CMD,DISPLAY_INDICATOR_OFF);
	st7565_interface_write(CMD,DISPLAY_INDICATOR_MODE_OFF);

	// (20) Booster ratio set
	st7565_interface_write(CMD,DISPLAY_BOOSTER_SET);
	st7565_interface_write(CMD,DISPLAY_BOOSTER_234);

	// (2) Display ON/OFF
	st7565_interface_write(CMD,DISPLAY_START_LINE_SET);

	st7565_clr_to_end(0,0);

	// (1) Display ON/OFF
	st7565_interface_write(CMD,DISPLAY_ON);

#if USE_BUFF == 1
	for(uint8_t i=0; i<BUFF_PAGE_NUM;i++){
		st7565_buffer[i].bg_color=LCD_BG_COLOR;
	}

	st7565_Clr_buff();

#endif

}

void st7565_setPos(uint8_t y, uint8_t x){
	st7565_interface_write(CMD,DISPLAY_PAGE_ADDRESS | ((y) & 0x0F));
	st7565_interface_write(CMD,DISPLAY_COL_ADDRESS_MSB | ((x>>4) & 0x0F));
	st7565_interface_write(CMD,DISPLAY_COL_ADDRESS_LSB | ((x) & 0x0F));
}

void st7565_clr(void){
	uint8_t page, col;

	for(page=0;page<LCD_PAGE_NUM;page++){
		st7565_setPos(page,0);
		for (col=0;col<LCD_COL_NUM;col++){
			//st7565_setPos(page,col);
			st7565_interface_write(DATA,0x00);
		}
	}
}

void st7565_clr_to_end(uint8_t y, uint8_t x){
	uint8_t page;

	for(page=y;page<LCD_PAGE_NUM;page++){
		if (page!=y) x=0;
		st7565_setPos(page,x);
		while(x++<LCD_COL_NUM){
			st7565_interface_write(DATA,0x00);
		}
	}
}

void st7565_interface_write( uint8_t comand, uint8_t data ){
	//OSC_TEST_TOG;
	if(comand == 0) PORT( A0_PORT) |= (1<<A0_PIN); else PORT( A0_PORT) &= ~(1<<A0_PIN);
	#if USE_RD == 1
		PORT(RD_PORT) |= (1<<RD_PIN);
		asm volatile ("nop");
	#endif
	PORT(CS_PORT) &= ~(1<<CS_PIN);

	#if USE_WR == 1
		PORT(WR_PORT) &= ~(1<<WR_PIN);
	#endif

// ---------------------- PARALE ------------------
	#if INTERFACE == 0
		PORT(D0_PORT) = data;
	#endif
// ------------------------------------------------

// ---------------------- SERIAL ------------------
	#if INTERFACE == 1
		uint8_t cnt = 0x80;
		while(cnt){
			if( data & cnt) PORT( SI_PORT) |= (1<<SI_PIN); else PORT( SI_PORT) &= ~(1<<SI_PIN);
			PORT(SCK_PORT) |= (1<<SCK_PIN);
			PORT(SCK_PORT) &= ~(1<<SCK_PIN);

			cnt >>=1;
		}
	#endif
// ------------------------------------------------

	#if USE_WR == 1
		PORT(WR_PORT) |= (1<<WR_PIN);
	#endif

	PORT(CS_PORT) |= (1<<CS_PIN);

	#if USE_RD == 1
		PORT(RD_PORT) &= ~(1<<RD_PIN);
		asm volatile ("nop");
	#endif
	//OSC_TEST_TOG;
}


#if USE_BUFF == 1

void st7565_PageBgColor_buff(uint8_t page, uint8_t color){
	if(page<BUFF_PAGE_NUM){
		st7565_buffer[page].bg_color=color;
	}
}

void st7565_ClrPage_buff(uint8_t page,uint8_t start, uint8_t stop){
	if(stop==0 || stop>LCD_WIDTH) stop=LCD_WIDTH;

	for(uint8_t i=start; i<stop;i++){
		st7565_buffer[page].col[i]=st7565_buffer[page].bg_color;
	}
}
void st7565_Clr_buff(void){
	for(uint8_t i=0;i<BUFF_PAGE_NUM;i++){
		st7565_ClrPage_buff(i,0,LCD_WIDTH);
	}
}

void st7565_SendPageBuff_ToDisplay(uint8_t page, uint8_t start, uint8_t stop){
	if(start>LCD_WIDTH-1) start = LCD_WIDTH-1;
	if(stop==0 || stop>LCD_WIDTH) stop=LCD_WIDTH;

	st7565_setPos(page, start);
	for (uint8_t i=start;i<stop;i++){
		st7565_interface_write( DATA, st7565_buffer[page].col[i] );
	}
}

void st7565_SendBuff_ToDisplay(void){
	for(uint8_t i=0;i<BUFF_PAGE_NUM;i++){
		st7565_SendPageBuff_ToDisplay(i,0,LCD_WIDTH);
	}

}
#endif
