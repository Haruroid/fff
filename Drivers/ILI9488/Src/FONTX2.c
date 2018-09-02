/*
 * FONTX2.c
 *
 *  Created on: 2018/08/31
 *      Author: haru
 */

#include "FONTX2.h"
//Thank a lot =>http://elm-chan.org/docs/dosv/fontx.html
//				https://sakura87.net/archives/2671
//				https://qiita.com/meketen/items/a66c6953b3cff380d9b8

void fx_writechar(uint16_t x, uint16_t y, uint8_t c, uint16_t color,
		uint16_t bg, uint8_t *font) {
	uint8_t fon_width = font[14];
	uint8_t fon_height = font[15];
	if (font[16] == 0) { //ANK
		if ((c >= 0x81 && c <= 0x9F) || (c >= 0xe0 && c <= 0xfc)) //check mismatch character
			return;

		uint32_t offset = 17 + c * ((fon_width + 7) / 8) * fon_height;
		for (uint8_t for_y = 0; for_y < fon_height; for_y++) {
			uint8_t line = font[offset + fon_height - for_y];
			for (uint8_t for_x = (fon_width > 8) ? 7 : fon_width - 1; for_x > 0;
					for_x--, line >>= 1) {
				if (line & 1)
					LCD_drawDot(x + for_x, y + for_y, color);
				else
					LCD_drawDot(x + for_x, y + for_y, bg);
			}
		}
	} //i won't write code for s-jis because it is useless =)
}

#ifdef USE_CHAN_FF
void fx_writecharFF(uint16_t x, uint16_t y, uint16_t c, uint16_t color,
		uint16_t bg, FIL *font) {
	uint8_t fxbuf[FX_BUF];
	UINT br = 0;
	f_lseek(font, 0);
	f_read(font, fxbuf, FX_BUF, &br);

	uint8_t fon_width = fxbuf[14];
	uint8_t fon_height = fxbuf[15];
	if (fxbuf[16] == 0) { //ANK
		uint32_t offset = 17 + c * ((fon_width + 7) / 8) * fon_height;
		f_lseek(font, offset);
		f_read(font, fxbuf, fon_width * fon_height, &br);
		for (uint8_t for_y = 0; for_y < fon_height; for_y++) {
			uint8_t line = fxbuf[fon_height - for_y];
			for (uint8_t for_x = (fon_width > 8) ? 7 : fon_width - 1; for_x > 0;
					for_x--, line >>= 1) {
				if (line & 1)
					LCD_drawDot(x + for_x, y + for_y, color);
				else
					LCD_drawDot(x + for_x, y + for_y, bg);
			}
		}
	} else if (fxbuf[16] == 1) { //S-JIS
		uint8_t codeblock = fxbuf[17];
		uint8_t width_byte = (fon_width + 7) / 8;
		uint32_t offset = 0;
		uint16_t cc = 0;
		for (uint8_t cbc = 0; cbc < codeblock; cbc++) {	//search character on code block table
			f_lseek(font, 18 + cbc * 4);
			f_read(font, fxbuf, 4, &br);
			uint16_t cs = fxbuf[0] | fxbuf[1] << 8;
			uint16_t ce = fxbuf[2] | fxbuf[3] << 8;
			if (c >= cs && c <= ce) {
				cc += c - cs;
				offset = 18 + 4 * codeblock + cc * width_byte * fon_height;
				break;
			}
			cc += ce - cs + 1;
		}
		if (offset == 0) { //no character in selected font
			return;
		}
		f_lseek(font, offset);
		f_read(font, fxbuf, width_byte * fon_height, &br);
		uint16_t fx;
		for (uint16_t fy = 0; fy < fon_height; fy++) {
			fx = fon_width;
			uint8_t xb = width_byte;
			while (xb--) {
				uint8_t line = fxbuf[(fon_height - fy - 1) * width_byte + xb];
				uint8_t fxx;
				if ((fx % 8) != 0) {
					fxx = fx % 8;
					line >>= 8 - fxx;
				} else {
					fxx = 8;
				}
				while (fxx--) {
					if (line & 1)
						LCD_drawDot(x + fx, y + fy, color);
					else
						LCD_drawDot(x + fx, y + fy, bg);
					line >>= 1;
					fx--;
				}
			}
		}
	}
}

void fx_writestrFF(uint16_t x, uint16_t y, char *str, uint16_t color,
		uint16_t bg) {
	FIL font;
	uint8_t len = strlen(str);
	uint16_t pointer = 0;
	while (len--)
		if ((str[pointer] >= 0x80 && str[pointer] <= 0x9F)
				|| (str[pointer] >= 0xe0 && str[pointer] <= 0xfc)) {
			if (f_open(&font, ZENKAKU_FONT, FA_READ | FA_OPEN_EXISTING)
					!= FR_OK)
				return;
			fx_writecharFF(x, y, str[pointer] << 8 | str[pointer + 1], color,
					bg, &font);
			f_close(&font);
			pointer += 2;
			x += ZENKAKU_WIDTH + 1;
			len--;
		} else {
			if (f_open(&font, HANKAKU_FONT, FA_READ | FA_OPEN_EXISTING)
					!= FR_OK)
				return;
			fx_writecharFF(x, y, str[pointer], color, bg, &font);
			f_close(&font);
			pointer += 1;
			x += HANKAKU_WIDTH + 1;
		}
}
#endif
