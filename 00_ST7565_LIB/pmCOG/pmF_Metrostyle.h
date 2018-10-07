/*
 * pmF_Metrostyle.h
 *
 *  Created on: 2 paü 2018
 *      Author: Piotr Michna
 *      poczta@piotrmichna.pl
 */

#ifndef PMFONT_PMF_METROSTYLE_H_
#define PMFONT_PMF_METROSTYLE_H_


#define METROSTYLE_PL
#define METROSTYLE_12
#define METROSTYLE_16
#define METROSTYLE_24

#include "pmFONT_def.h"

extern const uint8_t Metrostyl8x8Dat[];
extern const CHAR_INFO Metrostyl8x8Map[];
extern const FONT_INFO Metrostyl8x8Font;

#ifdef METROSTYLE_12
extern const uint8_t Metrostyle10x12Dat[];
extern const CHAR_INFO Metrostyle10x12Map[];
extern const FONT_INFO Metrostyle10x12Font;
#endif

#ifdef METROSTYLE_16
extern const uint8_t Metrostyle11x16Dat[];
extern const CHAR_INFO Metrostyle11x16Map[];
extern const FONT_INFO Metrostyle11x16Font;
// font '/'(47) = 0
// font ';'(59) = '∞'(176)
// font '<'(60) = 'C'(67)
#endif

#ifdef METROSTYLE_24
extern const uint8_t Metrostyle16x24Dat[];
extern const CHAR_INFO Metrostyle16x24Map[];
extern const FONT_INFO Metrostyle16x24Font;
// font '/'(47) = 0
// font ';'(59) = '∞'(176)
// font '<'(60) = 'C'(67)
#endif

#endif /* PMFONT_PMF_METROSTYLE_H_ */
