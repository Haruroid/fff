/*
 * FONTX2.h
 *
 *  Created on: 2018/08/31
 *      Author: haru
 */

#ifndef ILI9488_INC_FONTX2_H_
#define ILI9488_INC_FONTX2_H_
#include "ILI9488.h"

#define USE_CHAN_FF

#define READ32(dat,pointer) (uint32_t)(dat[pointer + 3] << 24 | dat[pointer + 2] << 16 | dat[pointer + 1] << 8 | dat[pointer])
#define READ16(dat,pointer) (uint16_t)(dat[pointer+1] << 8 | dat[pointer])

#define LCD_drawDot(x,y,color) ILI9488_drawDot(x,y,color)

void fx_writechar(uint16_t x, uint16_t y, uint8_t c, uint16_t color,
		uint16_t bg, uint8_t *font);
#ifdef USE_CHAN_FF
#include "ff.h"
#define FX_BUF 512
#define ZENKAKU_FONT "zenkaku16x.FNT"
#define ZENKAKU_WIDTH 16
#define HANKAKU_FONT "hankaku.FNT"
#define HANKAKU_WIDTH 8
void fx_writecharFF(uint16_t x, uint16_t y, uint16_t c, uint16_t color, uint16_t bg, FIL *font);
void fx_writestrFF(uint16_t x, uint16_t y, char *str, uint16_t color, uint16_t bg);
#endif
#endif /* ILI9488_INC_FONTX2_H_ */
