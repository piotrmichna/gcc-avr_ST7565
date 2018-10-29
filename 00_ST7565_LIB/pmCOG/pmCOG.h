/*
 * pmCOG.h
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 */

#ifndef PMCOG_PMCOG_H_
#define PMCOG_PMCOG_H_


#define LCD_ST7565

#define METROSTYLE_FONT

#define FONT_TEST

#define USE_BUFF 1

void cogInit(void);
void cogClr(void);
void cogInvertColor(uint8_t inv);
void cogGoTo(uint8_t y, uint8_t x);
uint8_t cogGetX(void);
void cogPutChar(char c);
void cogPutLineX(uint8_t ypix, uint8_t xl, uint8_t xr);

#ifdef METROSTYLE_FONT
uint8_t useFontMetrostyle(uint8_t height);
#endif

#ifdef FONT_TEST
	uint8_t cogFontCharList(void);
	void cogFontPrint(void);
#endif

#if USE_BUFF == 1

void cogPageBgColor_buff(uint8_t page, uint8_t color);
void cogClrPage_buff(uint8_t page,uint8_t start, uint8_t stop);
void cogClr_buff(void);

void cogSendPageBuff_ToDisplay(uint8_t page, uint8_t start, uint8_t stop);
void cogSendBuff_ToDisplay(void);

#endif

#endif /* PMCOG_PMCOG_H_ */
