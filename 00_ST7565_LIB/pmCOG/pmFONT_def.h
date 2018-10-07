/*
 * pmFONT_def.h
 *
 *  Created on: 28 wrz 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 */

#ifndef PMFONT_PMFONT_DEF_H_
#define PMFONT_PMFONT_DEF_H_


// This structure describes a single character's display information
typedef struct {
	const uint8_t widthF;	// width, in bits (or pixels), of the character
	const uint16_t map;	// map of the character's bitmap, in bytes, into the the FONT_INFO's dat array
} CHAR_INFO;

// Describes a single font
typedef struct {
	uint8_t heightFont;	// Font height in pixels
	uint8_t startChar;	// the first character in the font
	uint8_t stopChar;	// the last character in the font
	uint8_t interCharPixels;	// number of pixels of interspace between characters
	uint8_t spacePixels;	// number of pixels of space character
	const CHAR_INFO * charInfo;	// pointer to array of char information
	const uint8_t * dat;	// pointer to generated array of character
} FONT_INFO;


#endif /* PMFONT_PMFONT_DEF_H_ */
