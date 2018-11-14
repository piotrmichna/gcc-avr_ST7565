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

void intToStr(char * buf, int16_t num);

void cogInit(void);
void cogClr(void);
void cogInvertColor(uint8_t inv);
void cogGoTo(uint8_t y, uint8_t x);
uint8_t cogGetX(void);
uint8_t cogGetY(void);
uint8_t cogPutChar(char c);
uint8_t cogPutString(char * str);

uint8_t cogGetCharWidth(char c);
uint8_t cogGetStringWidth(char * str);
#ifdef METROSTYLE_FONT
uint8_t useFontMetrostyle(uint8_t height);
#endif

#ifdef FONT_TEST
	void cogFontPrint(void);
	#if USE_BUFF == 1
		void cogFontPrintBuff(void);
	#endif
#endif

#if USE_BUFF == 1

void cogPageBgColor_buff(uint8_t page, uint8_t color);
void cogClrPage_buff(uint8_t page,uint8_t start, uint8_t stop);
void cogClr_buff(void);

void cogSendPageBuff_ToDisplay(uint8_t page, uint8_t start, uint8_t stop);
void cogSendBuff_ToDisplay(void);
void cogSetLocateBuf(uint8_t y, uint8_t x);
uint8_t cogPutCharBuff(char c);
uint8_t cogGetBuffY(void);
uint8_t cogGetBuffX(void);
#endif

#endif /* PMCOG_PMCOG_H_ */
