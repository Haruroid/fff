/*
 * bitmap.h
 *
 *  Created on: 2018/08/28
 *      Author: Haru
 */

#ifndef BITMAP_INC_BITMAP_H_
#define BITMAP_INC_BITMAP_H_

#include "ILI9488.h"

#define USE_CHAN_FF

#define X_MAX 480
#define Y_MAX 320

#define READ32(dat,pointer) (uint32_t)(dat[pointer + 3] << 24 | dat[pointer + 2] << 16 | dat[pointer + 1] << 8 | dat[pointer])
#define READ16(dat,pointer) (uint16_t)(dat[pointer+1] << 8 | dat[pointer])

#define Bitmap_drawDot(x,y,color) ILI9488_drawDot(x,y,color)

#define RGB565(r,g,b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

void drawBitmap(uint16_t x, uint16_t y, uint8_t* bitmap);

#ifdef USE_CHAN_FF
#define BMP_BUF 1024 //must divide with 4
#include "ff.h"
void drawBitmapFF(uint16_t x,uint16_t y,FIL *bmp);
#endif

#endif /* BITMAP_INC_BITMAP_H_ */
