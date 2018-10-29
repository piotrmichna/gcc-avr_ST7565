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


#if USE_BUFF == 1

typedef struct{
	uint8_t col[LCD_WIDTH];
	uint8_t bg_color;
}TCOL;

#define LCD_BG_COLOR 0x00

TCOL cog_buffer[LCD_PAGE_NUM];
uint8_t buffCur_X;
uint8_t buffCur_Y;
#endif

#ifdef METROSTYLE_FONT
	#include "pmF_Metrostyle.h"
#endif


volatile uint8_t curX, curY,invert;

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
#endif

void cogSetPos(uint8_t y, uint8_t x);
void setFotn(const FONT_INFO * const font);


void cogInit(void){
	#ifdef LCD_ST7565
		st7565_init();
	#endif

#if USE_BUFF == 1
	for(uint8_t i=0; i<LCD_PAGE_NUM;i++){
		cog_buffer[i].bg_color=LCD_BG_COLOR;
	}

	st7565_Clr_buff();
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

void cogPutLineX(uint8_t ypix, uint8_t xl, uint8_t xr){
	uint8_t bajt=0x00, line, bit;

	line=ypix/8;
	cogGoTo(line, xl);
	bit= ypix-(line*8);
	bajt= (1<<bit);

	while(xl<xr && xl<LCD_COL_NUM){
		#ifdef LCD_ST7565
			st7565_interface_write( DATA, bajt );
		#endif
		curX++;
	}
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

void cogPutChar(char c){
	uint8_t fWidth = pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].widthF );

	uint8_t * data = (uint8_t*)current_font.dat;
	data+= pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].map );

	uint8_t x=0, y, ny, bajt, p=0;
	ny=current_font.heightFont/8;

	if (ny*8<current_font.heightFont) ny++;
	if(c!=' ' && fWidth>0){

		for(x=0;x<fWidth+current_font.interCharPixels;x++){
			if(ny==1){			// font na jednej stronie

				if(!x) cogSetPos(curY, curX);

				if(x<fWidth) bajt = pgm_read_word( &data[ p++ ] ); else bajt=0;
				if(invert) bajt = ~bajt;

				#ifdef LCD_ST7565

					st7565_interface_write( DATA, bajt );
				#endif
			}else{				// font na wielu stronach
				for(y=0;y<ny;y++){
					cogSetPos(curY+y, curX+x);

					if(x<fWidth) bajt = pgm_read_word( &data[ p++ ] ); else bajt=0;
					if(invert) bajt = ~bajt;
					#ifdef LCD_ST7565
						st7565_interface_write( DATA, bajt );
					#endif
				}
			}
		}
	}
	if(c==' '){
		for(x=0;x<current_font.spacePixels;x++){
			if(ny==1){
				if(!x)cogSetPos(curY, curX);

				bajt = 0x00;
				if(invert) bajt = ~bajt;
				#ifdef LCD_ST7565
					st7565_interface_write( DATA, bajt );
				#endif
			}else{
				for(y=0;y<ny;y++){
					cogSetPos(curY+y, curX+x);

					bajt = 0x00;
					if(invert) bajt = ~bajt;
					#ifdef LCD_ST7565
						st7565_interface_write( DATA, bajt );
					#endif
				}
			}
		}
	}

curX+=x;
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
	uint8_t flag=1, oldX,n=0;
	if(!c){
		c=current_font.startChar;
	}

	n=current_font.heightFont/8;
	if(n*8<current_font.heightFont) n++;

	while(c<=current_font.stopChar){
		oldX=curX;
		cogPutChar(c++);
		if(curX>LCD_WIDTH-1){
			c--;
			curX=oldX;
			while(curX<LCD_WIDTH){
				cogPutChar(' ');
			}

			if(curY<LCD_PAGE_NUM-1){
				curX=0;

				curY+=n;
				if (curY>1 && curY<3) cogInvertColor(1); else cogInvertColor(0);
				cogPutChar(c++);
			}else{
				flag=0;
				break;
			}
		}
	}
	if(n>1 && curY<LCD_PAGE_NUM-2){
		while(curX<LCD_WIDTH){
			cogPutChar(' ');
		}
		curY+=n;
		curX=0;
	}
	if(flag) c='\0';

	return flag;
}
#endif


#if USE_BUFF == 1

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
void cogPutCharBuff(char c){
	uint8_t fWidth = pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].widthF );

	uint8_t * data = (uint8_t*)current_font.dat;
	data+= pgm_read_word( &current_font.charInfo[ c - current_font.startChar ].map );

	uint8_t x=0, y, ny, bajt, p=0;
	ny=current_font.heightFont/8;

	if (ny*8<current_font.heightFont) ny++;
	if(c!=' ' && fWidth>0){
		for(x=0;x<fWidth+current_font.interCharPixels;x++){
			if(ny==1){			// font na jednej stronie
				if(x<fWidth) bajt = pgm_read_word( &data[ p++ ] ); else bajt=cog_buffer[buffCur_Y].bg_color;
				if(invert) bajt = ~bajt;

				cog_buffer[buffCur_Y].col[buffCur_X+x]=bajt;

			}else{				// font na wielu stronach
				for(y=0;y<ny;y++){

					if(x<fWidth) bajt = pgm_read_word( &data[ p++ ] ); else bajt=cog_buffer[buffCur_Y].bg_color;
					if(invert) bajt = ~bajt;

					cog_buffer[buffCur_Y+y].col[buffCur_X+x]=bajt;
				}
			}
		}
	}
	if(c==' '){
		for(x=0;x<current_font.spacePixels;x++){
			if(ny==1){
				bajt=cog_buffer[buffCur_Y].bg_color;
				if(invert) bajt = ~bajt;

				cog_buffer[buffCur_Y].col[buffCur_X+x]=bajt;

			}else{
				for(y=0;y<ny;y++){
					bajt=cog_buffer[buffCur_Y].bg_color;
					if(invert) bajt = ~bajt;

					cog_buffer[buffCur_Y+y].col[buffCur_X+x]=bajt;
				}
			}
		}
	}
	curX+=x;
}
#endif
