/*
 * main.c
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 *
 *      cup: Atmega328P
 *      F_CPU 14.7456 Mhz
 *      https://www.youtube.com/watch?v=3Vc89HU6Q80
 */


#include <avr/io.h>
#include <util/delay.h>

#include "mac.h"

#include "pmCOG/pmCOG.h"

char tekst[]="dupa";

int main(void){
	DDR( LED1_PORT ) |= (1<<LED1_PIN);
	DDR( LED2_PORT ) |= (1<<LED2_PIN);
	DDR( OSC_TEST_PORT ) |= (1<<OSC_TEST_PIN);
	PORT(LED1_PORT) &= ~(1<<LED1_PIN);
	PORT(LED2_PORT) &= ~(1<<LED2_PIN);

	cogInit();

	while(1){
		LED2_TOG;

//		#ifdef FONT_TEST
//			#if USE_BUFF == 0
//				cogFontPrint();
//			#else
//				cogFontPrintBuff();
//			#endif
//		#endif
		cogClr();
		cogGoTo(0,5);
		cogPutString(tekst);
		_delay_ms(2500);


	}
}
