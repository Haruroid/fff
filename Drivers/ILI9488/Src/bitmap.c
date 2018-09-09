/*
 * bitmap.c
 *
 *  Created on: 2018/08/28
 *      Author: Haru
 */

#include "bitmap.h"

void drawBitmap(uint16_t x, uint16_t y, uint8_t* bitmap) {
	if (bitmap[0] != 'B' || bitmap[1] != 'M') //check BM header
		return;
	if (READ16(bitmap, 14) != 40) //check Windows Bitmap
		return;
	if (READ16(bitmap, 26) != 1) //check plane
		return;
	if (READ16(bitmap, 28) != 24) //check color bit
		return;
	if (READ32(bitmap, 30) != 0) //check compression
		return;

	uint32_t offset, width, height;
	offset = READ32(bitmap, 10);
	width = abs(READ32(bitmap, 18));
	height = abs(READ32(bitmap, 22));
	if (width > X_MAX || height > Y_MAX)
		return;
	uint32_t pointer = 0;
	for (uint32_t for_y = 0; for_y < height; for_y++) {
		for (uint32_t for_x = x; for_x < x + width; for_x++) {
			uint8_t B = bitmap[offset + pointer * 3];
			uint8_t G = bitmap[offset + pointer * 3 + 1];
			uint8_t R = bitmap[offset + pointer * 3 + 2];
			Bitmap_drawDot(for_x, y + height - for_y, RGB565(R,G,B));
			if (for_x - x == width && (width % 4) != 0)
				pointer = +(width % 4) + 1;
			else
				pointer += 1;
		}
	}
}

#ifdef USE_CHAN_FF
void drawBitmapFF(uint16_t x, uint16_t y, FIL *bmp) {
	uint8_t bmpbuf[BMP_BUF];
	UINT bw;
	f_lseek(bmp, 0);
	f_read(bmp, bmpbuf, BMP_BUF, &bw);
	if (bmpbuf[0] != 'B' || bmpbuf[1] != 'M') //check BM header
		return;
	if (READ16(bmpbuf, 14) != 40) //check Windows bitmap
		return;
	if (READ16(bmpbuf, 26) != 1) //check plane
		return;
	if (READ16(bmpbuf, 28) != 24) //check color bit
		return;
	if (READ32(bmpbuf, 30) != 0) //check compression
		return;

	uint32_t imgsize, offset, width, height, pointer, bufcnt, gcounter;
	uint16_t cnt_x, cnt_y;
	offset = READ32(bmpbuf, 10);
	imgsize = READ32(bmpbuf,2) - offset;
	width = abs(READ32(bmpbuf, 18));
	height = abs(READ32(bmpbuf, 22));
	if (width > X_MAX || height > Y_MAX)
		return;
	bufcnt = imgsize / BMP_BUF;
	cnt_x = 0;
	cnt_y = 0;
	gcounter = 0;
	uint8_t bmp_R = 0;
	uint8_t bmp_G = 0;
	uint8_t bmp_B = 0;
	for (uint32_t cntlp = 0; cntlp <= bufcnt; cntlp++) {
		f_lseek(bmp, offset + (cntlp * BMP_BUF));
		f_read(bmp, bmpbuf, BMP_BUF, &bw);
		pointer = 0;
		uint32_t pointermax =
				(cntlp == bufcnt) ? ((imgsize % BMP_BUF)) : (BMP_BUF);
		while (pointer < pointermax) {
			switch (gcounter % 3) {
			case 0:
				bmp_B = bmpbuf[pointer];
				pointer++;
				break;
			case 1:
				bmp_G = bmpbuf[pointer];
				pointer++;
				break;
			case 2:
				bmp_R = bmpbuf[pointer];
				Bitmap_drawDot(cnt_x + x, y + height - cnt_y, RGB565(bmp_R,bmp_G,bmp_B));
				if (cnt_x == width - 1) {
					if ((width % 4) != 0)
						pointer += (width % 4) + 1;
					else
						pointer++;
					cnt_x = 0;
					cnt_y++;
				} else {
					pointer++;
					cnt_x++;
				}
				break;
			default:
				break;
			}
			gcounter++;
		}
	}
}

#endif
