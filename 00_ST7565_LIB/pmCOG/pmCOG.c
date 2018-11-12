/*
 * pmCOG.c
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../mac.h"

#include "pmCOG.h"

#ifdef LCD_ST7565
#include "../pmST7565/pmST7565.h"
#endif

#define LCD_BG_COLOR 0x00

#if USE_BUFF == 1

typedef struct{
	uint8_t col[LCD_WIDTH];
	uint8_t bg_color;
}TCOL;

TCOL cog_buffer[LCD_PAGE_NUM];
volatile uint8_t buffCur_X;
volatile uint8_t buffCur_Y;
#endif

#ifdef METROSTYLE_FONT
	#include "pmF_Metrostyle.h"
#endif


volatile uint8_t curX, curY, invert;

FONT_INFO current_font;
#ifdef FONT_TEST
#define FONT_NUM 4
FONT_INFO const * font_tab[FONT_NUM]={
		&Metrostyl8x8Font,
#ifdef METROSTYLE_12
		&Metrostyle10x12Font,
#else
		0,
#endif
#ifdef METROSTYLE_16
		&Metrostyle11x16Font,
#else
		0,
#endif
#ifdef METROSTYLE_24
		&Metrostyle16x24Font,
#else
		0,
#endif
};


uint8_t cogFontCharList(void);
#if USE_BUFF == 1
uint8_t cogFontCharListBuff(void);
#endif
#endif

void cogSetPos(uint8_t y, uint8_t x);
void setFotn(const FONT_INFO * const font);

void intToStr(char * buf, int16_t num){
	uint8_t i=0;
	char temp[6];
	if(num<0) {
		*buf++='-';
		num *=-1;
	}
	do {
		temp[i] = num % 10 + 48;
		num-= num % 10;
		if(num>9) i++;
	} while(num/=10);

	while(i){
		*buf++ =temp[i--];
	}
	*buf='\0';

}

void cogInit(void){
	#ifdef LCD_ST7565
		st7565_init();
	#endif

#if USE_BUFF == 1
	for(uint8_t i=0; i<LCD_PAGE_NUM;i++){
		cog_buffer[i].bg_color=LCD_BG_COLOR;
	}

	cogClr_buff();
#endif

	#ifdef METROSTYLE_FONT
		setFotn( &Metrostyl8x8Font );
	#endif
}


void cogClr(void){
	#ifdef LCD_ST7565
		st7565_clr();
	#endif
	curX=0;
	curY=0;

}

void cogInvertColor(uint8_t inv){
	if(inv) invert=1; else invert=0;
}

void cogGoTo(uint8_t y, uint8_t x){
	curX=x;
	curY=y;
	cogSetPos(curY,curX);


}
void cogSetPos(uint8_t y, uint8_t x){
	#ifdef LCD_ST7565
		st7565_setPos(y,x);
	#endif
}

uint8_t cogGetX(void){
	return curX;
}

uint8_t cogGetY(void){
	return curY;
}

uint8_t cogPutChar(char c){
	uint8_t fWidth;
	uint8_t x=0, y, line_num, bajt;

	line_num=current_font.heightFont/8;
	if (line_num*8<current_font.heightFont) line_num++;

	if(c==' ') {	// char space print
		fWidth=current_font.spacePixels;
		for (x=0;x<fWidth;x++){
			if(curX+x>=LCD_WIDTH) break;
			for(y=0;y<line_num;y++){
				if( (!x && !y) || line_num>1 ) cogSetPos(curY+y, curX+x);
				bajt=LCD_BG_COLOR;
				if(invert) bajt = ~bajt;
				if (curY+y<LCD_PAGE_NUM){
					#ifdef LCD_ST7565
						st7565_interface_write( DATA, bajt );
					#endif
				}

			}
		}
		curX+=x;
		return 1; // space char print ok
	}else{ // char print
		if(c>=current_font.startChar && c<=current_font.stopChar){
			fWidth = pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].widthF );
			if(curX+fWidth>=LCD_WIDTH){
				for(x=0;x<fWidth;x++){
					if(curX+x>=LCD_WIDTH) break;
					for(y=0;y<line_num;y++){
						if( (!x && !y) || line_num>1 ) cogSetPos(curY+y, curX+x);
						bajt=LCD_BG_COLOR;
						if(invert) bajt = ~bajt;
						if (curY+y<LCD_PAGE_NUM){
							#ifdef LCD_ST7565
								st7565_interface_write( DATA, bajt );
							#endif
						}
					}
				}
				curX+=x;
				return 0;	// page to short to print;
			}else{
				if(fWidth>0){
					uint8_t p=0;
					uint8_t * data = (uint8_t*)current_font.dat;
					data+= pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].map );
					for(x=0;x<fWidth+current_font.interCharPixels;x++){
						if(curX+x>=LCD_WIDTH) break;
						for(y=0;y<line_num;y++){
							if(x<fWidth) bajt = pgm_read_word( &data[ p++ ] ); else bajt=LCD_BG_COLOR;//cog_buffer[buffCur_Y].bg_color;
							if( (!x && !y) || line_num>1 ) cogSetPos(curY+y, curX+x);
							if(invert) bajt = ~bajt;
							if (curY+y<LCD_PAGE_NUM){
								#ifdef LCD_ST7565
									st7565_interface_write( DATA, bajt );
								#endif
							}
						}
					}
				}
			}
			curX+=x;
			return 1; // char print ok
		}else{
			return 2; // not defined char in font
		}
	}
}
uint8_t cogPutString(char * buf){
	return 1;
}

void setFotn(const FONT_INFO * const font){
	if(font){
		current_font.heightFont = pgm_read_word(&font->heightFont);
		   current_font.startChar = pgm_read_word(&font->startChar);
		    current_font.stopChar = pgm_read_word(&font->stopChar);
		 current_font.spacePixels = pgm_read_word(&font->spacePixels);
	current_font.interCharPixels = pgm_read_word(&font->interCharPixels);
			current_font.charInfo = (CHAR_INFO*) pgm_read_word(&font->charInfo);
				current_font.dat = (uint8_t*) pgm_read_word(&font->dat);
	}
}

#ifdef METROSTYLE_FONT
uint8_t useFontMetrostyle(uint8_t height){
	switch(height){
		case 8:
			setFotn( &Metrostyl8x8Font );
			return 1;
			break;

	#ifdef METROSTYLE_12
		case 12:
			setFotn( &Metrostyle10x12Font );
			return 1;
			break;
	#endif

	#ifdef METROSTYLE_16
		case 16:
			setFotn( &Metrostyle11x16Font );
			return 1;
			break;
	#endif

	#ifdef METROSTYLE_24
		case 24:
			setFotn( &Metrostyle16x24Font );
			return 1;
			break;
	#endif

		default:
			return 0;
	}
}
#endif

#ifdef FONT_TEST
void cogFontPrint(void){
	static uint8_t font;
	uint8_t flag;	// 1 - wyswietlono ostatin znak
	st7565_clr();
	cogGoTo(0,0);
	while(font_tab[font]==0){
		if(font<FONT_NUM-1) font++; else font=0;
	}
	setFotn( font_tab[font] );
	//setFotn( &Metrostyle10x12Font );

	flag=cogFontCharList();

	st7565_clr_to_end(curY,curX);

	if(flag){
		if(++font==FONT_NUM) font=0;
	}
}
uint8_t cogFontCharList(void){
	static uint8_t c;
	uint8_t flag=1, char_flag=1, n=0;
	if(!c){
		c=current_font.startChar;
	}

	n=current_font.heightFont/8;
	if(n*8<current_font.heightFont) n++;

	while(c<=current_font.stopChar){
		char_flag=cogPutChar(c);
		if(!char_flag){
			curX=0;
			curY+=n;
			if(curY<LCD_PAGE_NUM){

				char_flag=cogPutChar(c);
			}else{
				flag=0;
				break;
			}
		}
		c++;
	}
	if(char_flag){
		while(curX<LCD_WIDTH){
			char_flag=cogPutChar(' ');
		}
		curY+=n;
	}

	if(flag) c='\0';
	return flag;
}

	#if USE_BUFF == 1
	void cogFontPrintBuff(void){
		static uint8_t font;
		uint8_t flag=0;	// 1 - wyswietlono ostatin znak

		cogSetLocateBuf(0, 0);
		while(font_tab[font]==0){
			if(font<FONT_NUM-1) font++; else font=0;
		}
		setFotn( font_tab[font] );
		//setFotn( &Metrostyle10x12Font );

		flag=cogFontCharListBuff();

		//st7565_clr_to_end(curY,curX);
		cogClrPage_buff(buffCur_Y++, buffCur_X, LCD_WIDTH);
		while(buffCur_Y<LCD_PAGE_NUM){
			cogClrPage_buff(buffCur_Y++, 0, LCD_WIDTH);
		}
		cogSendBuff_ToDisplay();

		if(flag){
			if(++font==FONT_NUM) font=0;
		}
	}

	uint8_t cogFontCharListBuff(void){
		static uint8_t c;
		uint8_t flag=1, oldX,n=0;
		if(!c){
			c=current_font.startChar;
		}

		n=current_font.heightFont/8;
		if(n*8<current_font.heightFont) n++;

		while(c<=current_font.stopChar){
			oldX=buffCur_X;
			cogPutCharBuff(c++);
			if(buffCur_X>LCD_WIDTH-1){
				c--;
				buffCur_X=oldX;
				while(buffCur_X<LCD_WIDTH){
					cogPutCharBuff(' ');
				}

				if(buffCur_Y<LCD_PAGE_NUM-1){
					buffCur_X=0;

					buffCur_Y+=n;
					if (buffCur_Y>1 && buffCur_Y<3) cogInvertColor(1); else cogInvertColor(0);
					cogPutCharBuff(c++);
				}else{
					flag=0;
					break;
				}
			}
		}
		if(n>1 && buffCur_Y<LCD_PAGE_NUM-2){
			while(buffCur_X<LCD_WIDTH){
				cogPutCharBuff(' ');
			}
			buffCur_Y+=n;
			buffCur_X=0;
		}
		if(flag) c='\0';

		return flag;
	}
	#endif
#endif


#if USE_BUFF == 1
uint8_t cogGetBuffX(void){
	return buffCur_X;
}
uint8_t cogGetBuffY(void){
	return buffCur_Y;
}

void cogPageBgColor_buff(uint8_t page, uint8_t color){
	if(page<LCD_PAGE_NUM){
		cog_buffer[page].bg_color=color;
	}
}

void cogClrPage_buff(uint8_t page,uint8_t start, uint8_t stop){
	if(stop==0 || stop>LCD_WIDTH) stop=LCD_WIDTH;

	for(uint8_t i=start; i<stop;i++){
		cog_buffer[page].col[i]=cog_buffer[page].bg_color;
	}
}
void cogClr_buff(void){
	for(uint8_t i=0;i<LCD_PAGE_NUM;i++){
		cogClrPage_buff(i,0,LCD_WIDTH);
	}
}

void cogSendPageBuff_ToDisplay(uint8_t page, uint8_t start, uint8_t stop){
	if(start>LCD_WIDTH-1) start = LCD_WIDTH-1;
	if(stop==0 || stop>LCD_WIDTH) stop=LCD_WIDTH;
#ifdef LCD_ST7565
	st7565_setPos(page, start);
	for (uint8_t i=start;i<stop;i++){
		st7565_interface_write( DATA, cog_buffer[page].col[i] );
	}
#endif
}

void cogSendBuff_ToDisplay(void){
	for(uint8_t i=0;i<LCD_PAGE_NUM;i++){
		cogSendPageBuff_ToDisplay(i,0,LCD_WIDTH);
	}
}
void cogSetLocateBuf(uint8_t y, uint8_t x){
	buffCur_Y=y;
	buffCur_X=x;
}
uint8_t cogPutCharBuff(char c){
	uint8_t fWidth;
	uint8_t x=0, y, line_num, bajt;

	line_num=current_font.heightFont/8;
	if (line_num*8<current_font.heightFont) line_num++;

	if(c==' ') {
		fWidth=current_font.spacePixels;
		for (x=0;x<fWidth;x++){
			if(buffCur_X+x>=LCD_WIDTH) break;
			for(y=0;y<line_num;y++){
				bajt=cog_buffer[buffCur_Y].bg_color;
				if(invert) bajt = ~bajt;
				if (buffCur_Y+y<LCD_PAGE_NUM) cog_buffer[buffCur_Y+y].col[buffCur_X+x]=bajt;
			}
		}
		buffCur_X+=x;
		return 1; // space char print ok
	}else{
		if(c>=current_font.startChar && c<=current_font.stopChar){
			fWidth = pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].widthF );
			if(buffCur_X+fWidth>=LCD_WIDTH){
				for(x=0;x<fWidth;x++){
					if(buffCur_X+x>=LCD_WIDTH) break;
					for(y=0;y<line_num;y++){
						bajt=cog_buffer[buffCur_Y].bg_color;
						if(invert) bajt = ~bajt;
						if (buffCur_Y+y<LCD_PAGE_NUM) cog_buffer[buffCur_Y+y].col[buffCur_X+x]=bajt;
					}
				}
				buffCur_X+=x;
				return 0;	// page to short to print;
			}else{
				if(fWidth>0){
					uint8_t p=0;
					uint8_t * data = (uint8_t*)current_font.dat;
					data+= pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].map );
					for(x=0;x<fWidth+current_font.interCharPixels;x++){
						if(buffCur_X+x>=LCD_WIDTH) break;
						for(y=0;y<line_num;y++){
							if(x<fWidth) bajt = pgm_read_word( &data[ p++ ] ); else bajt=0x00;//cog_buffer[buffCur_Y].bg_color;
							if(invert) bajt = ~bajt;
							if (buffCur_Y+y<LCD_PAGE_NUM) cog_buffer[buffCur_Y+y].col[buffCur_X+x]=bajt;
						}
					}
				}
			}
			buffCur_X+=x;
			return 1; // char print ok
		}else{
			return 2; // not defined char in font
		}
	}
}
#endif
