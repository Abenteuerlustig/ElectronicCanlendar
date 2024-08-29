/*
 * fonts.h
 *
 *  Created on: 2023年6月26日
 *      Author: dell
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

typedef struct {
    unsigned int FontWidth ;
    unsigned int FontHeight;
} FontDef_t;

extern FontDef_t fontone;
extern unsigned int DATA[];
extern unsigned int hanzi[];

void font(void);

#endif /* INC_FONTS_H_ */
