/*
 * pmCOG.h
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 */

#ifndef PMCOG_PMCOG_H_
#define PMCOG_PMCOG_H_

#define METROSTYLE_FONT

#define FONT_TEST

void cogInit(void);
void cogClr(void);
void cogGoTo(uint8_t y, uint8_t x);
uint8_t cogGetX(void);
void cogPutChar(char c);

#ifdef METROSTYLE_FONT
uint8_t useFontMetrostyle(uint8_t height);
#endif

#ifdef FONT_TEST
	uint8_t cogFontCharList(void);
	void cogFontPrint(void);
#endif
#endif /* PMCOG_PMCOG_H_ */
