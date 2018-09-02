/*
 * ILI9488.c
 *
 *  Created on: 2018/07/05
 *      Author: Haru
 */
#include "ILI9488.h"

void ILI9488_writeCmd(uint16_t cmd) {
	LCD_CMD = cmd;
}

void ILI9488_writeData(uint16_t data) {
	LCD_DATA = data;
}

void ILI9488_setArea(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1) {
	ILI9488_writeCmd(0x2A);
	ILI9488_writeData((y >> 8));
	ILI9488_writeData((y & 0x00FF));
	ILI9488_writeData((y1 >> 8));
	ILI9488_writeData((y1 & 0x00FF));

	ILI9488_writeCmd(0x2B);
	ILI9488_writeData((x >> 8));
	ILI9488_writeData((x & 0x00FF));
	ILI9488_writeData((x1 >> 8));
	ILI9488_writeData((x1 & 0x00FF));

	ILI9488_writeCmd(0x2C);
}

void ILI9488_sendColor(uint16_t rgb565){
	//RGB565 to BGR565
	ILI9488_writeData(((rgb565 & 0b0000011111111111) | (rgb565 & 0b0000000000011111) << 11) >> 8);
	ILI9488_writeData(((rgb565 & 0b1111111111100000) | (rgb565 & 0b1111100000000000) >> 11 ) & 0x00FF);
}

void ILI9488_drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
		uint16_t color) {
	ILI9488_setArea(x, y, x + width , y + height);
	for (int i = 0; i < width * height; i++)
		ILI9488_sendColor(color);
}

void ILI9488_drawDot(uint16_t x, uint16_t y, uint16_t color) {
	ILI9488_drawRect(x, y, 1, 1, color);
}

void ILI9488_fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
		uint16_t color) {
	for (int i = x; i < x + width; i++)
		for (int j = y; j < y + height; j++) {
			ILI9488_drawDot(i, j, color);
//			HAL_Delay(1);
		}
}

void ILI9488_putc(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
	for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
		uint8_t line = font[c * 5 + i];
		(line & 0x01) ?
				ILI9488_drawDot(x + i, y + 7, color) :
				ILI9488_drawDot(x + i, y + 7, bg);
		(line & 0x02) ?
				ILI9488_drawDot(x + i, y + 6, color) :
				ILI9488_drawDot(x + i, y + 6, bg);
		(line & 0x04) ?
				ILI9488_drawDot(x + i, y + 5, color) :
				ILI9488_drawDot(x + i, y + 5, bg);
		(line & 0x08) ?
				ILI9488_drawDot(x + i, y + 4, color) :
				ILI9488_drawDot(x + i, y + 4, bg);
		(line & 0x10) ?
				ILI9488_drawDot(x + i, y + 3, color) :
				ILI9488_drawDot(x + i, y + 3, bg);
		(line & 0x20) ?
				ILI9488_drawDot(x + i, y + 2, color) :
				ILI9488_drawDot(x + i, y + 2, bg);
		(line & 0x40) ?
				ILI9488_drawDot(x + i, y + 1, color) :
				ILI9488_drawDot(x + i, y + 1, bg);
		(line & 0x80) ?
				ILI9488_drawDot(x + i, y + 0, color) :
				ILI9488_drawDot(x + i, y + 0, bg);
		//}
	}
}

void ILI9488_putc_f(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
	ILI9488_setArea(x, y, x + 4, y + 6);
	for (int i = 0; i < 5; i++) { // Char bitmap = 5 columns
		uint8_t line = font[c * 5 + i];
		ILI9488_sendColor((line & 0x80) ? color : bg);
		ILI9488_sendColor((line & 0x40) ? color : bg);
		ILI9488_sendColor((line & 0x20) ? color : bg);
		ILI9488_sendColor((line & 0x10) ? color : bg);
		ILI9488_sendColor((line & 0x08) ? color : bg);
		ILI9488_sendColor((line & 0x04) ? color : bg);
		ILI9488_sendColor((line & 0x02) ? color : bg);
		ILI9488_sendColor((line & 0x01) ? color : bg);
	}
}

void ILI9488_puts(uint16_t x, uint16_t y, uint16_t color, uint16_t bg,
		char* str) {
	for (int i = 0; i < strlen(str); i++) {
		ILI9488_putc(x + i * 6, y, str[i], color, bg);
	}
}

void ILI9488_puts_f(uint16_t x, uint16_t y, uint16_t color, uint16_t bg,
		char* str) {
	for (int i = 0; i < strlen(str); i++) {
		ILI9488_putc_f(x + i * 6, y, str[i], color, bg);
	}
}

void ILI9488_init() {
	ILI9488_writeCmd(0x01); //software reset
	HAL_Delay(50);
	ILI9488_writeCmd(0x11); //exit sleep
	HAL_Delay(50);

	ILI9488_writeCmd(0xE0);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x0E);
	ILI9488_writeData(0x15);
	ILI9488_writeData(0x06);
	ILI9488_writeData(0x13);
	ILI9488_writeData(0x09);
	ILI9488_writeData(0x3A);
	ILI9488_writeData(0xAC);
	ILI9488_writeData(0x4F);
	ILI9488_writeData(0x05);
	ILI9488_writeData(0x0D);
	ILI9488_writeData(0x0B);
	ILI9488_writeData(0x33);
	ILI9488_writeData(0x3B);
	ILI9488_writeData(0x0F);

	ILI9488_writeCmd(0xE1);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x0E);
	ILI9488_writeData(0x16);
	ILI9488_writeData(0x05);
	ILI9488_writeData(0x13);
	ILI9488_writeData(0x08);
	ILI9488_writeData(0x3B);
	ILI9488_writeData(0x9A);
	ILI9488_writeData(0x50);
	ILI9488_writeData(0x0A);
	ILI9488_writeData(0x13);
	ILI9488_writeData(0x0F);
	ILI9488_writeData(0x31);
	ILI9488_writeData(0x36);
	ILI9488_writeData(0x0F);

	ILI9488_writeCmd(0xC0);
	ILI9488_writeData(0x10);
	ILI9488_writeData(0x10);

	ILI9488_writeCmd(0xC1);
	ILI9488_writeData(0x44);

	ILI9488_writeCmd(0xC5);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x10);
	ILI9488_writeData(0x80);

	ILI9488_writeCmd(0x36);
	ILI9488_writeData(0x00);

	ILI9488_writeCmd(0x3A); /* Color Pixel Format */
	ILI9488_writeData(0x05);

	ILI9488_writeCmd(0xB0); /* Interface Mode Control */
	ILI9488_writeData(0x00);

	ILI9488_writeCmd(0xB1); /* Frame rate 70Hz */
	ILI9488_writeData(0xB0);
	ILI9488_writeData(0x11);

	ILI9488_writeCmd(0xB4);
	ILI9488_writeData(0x02);

	ILI9488_writeCmd(0xB6);
	ILI9488_writeData(0x02);
	ILI9488_writeData(0x22);

	ILI9488_writeCmd(0xB7);
	ILI9488_writeData(0xC6);

	ILI9488_writeCmd(0xBE);
	ILI9488_writeData(0x00);
	ILI9488_writeData(0x04);

	ILI9488_writeCmd(0xE9);
	ILI9488_writeData(0x00);

	ILI9488_writeCmd(0xF7);
	ILI9488_writeData(0xA9);
	ILI9488_writeData(0x51);
	ILI9488_writeData(0x2C);
	ILI9488_writeData(0x82);

	ILI9488_writeCmd(0x11);
	HAL_Delay(120);

//	ILI9488_writeCmd(0x21); /* display on */
	ILI9488_writeCmd(0x29);
	ILI9488_drawRect(0, 0, 480, 320, 0x0000);
}

