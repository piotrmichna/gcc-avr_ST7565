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

char teksta[]="dupa";
char tekstb[]="ok";
char tekstc[]="czcionka";
char * tekst[3]={ teksta, tekstb, tekstc };
char intBuf[6];
uint8_t n,len;

int main(void){
	DDR( LED1_PORT ) |= (1<<LED1_PIN);
	DDR( LED2_PORT ) |= (1<<LED2_PIN);
	DDR( OSC_TEST_PORT ) |= (1<<OSC_TEST_PIN);
	PORT(LED1_PORT) &= ~(1<<LED1_PIN);
	PORT(LED2_PORT) &= ~(1<<LED2_PIN);

	cogInit();
	useFontMetrostyle(12);

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
		if(n>2) n=0;
		len=cogGetStringWidth(tekst[n]);
		cogPutString(tekst[n++]);

		intToStr(intBuf, len);
		cogGoTo(4,5);
		cogPutString(intBuf);
		_delay_ms(2500);


	}
}
