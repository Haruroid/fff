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

uint16_t ILI9488_readData(){
	uint16_t tmp = LCD_DATA;
	return tmp;
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

void ILI9488_readArea(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1) {
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

	ILI9488_writeCmd(0x2E);
	ILI9488_readData();
}

void ILI9488_sendColor(uint16_t rgb565) {
	ILI9488_writeData(rgb565>>8);
	ILI9488_writeData(rgb565 & 0x00FF);
}

void ILI9488_drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
		uint16_t color) {
	ILI9488_setArea(x, y, x + width, y + height);
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
		}
}

void ILI9488_putc(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
	for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
		uint8_t line = font[c * 5 + i];
		for (uint8_t rep = 0; rep < 8; rep++) {
			(line & 0x01) ?
					ILI9488_drawDot(x + i, y + rep, color) :
					ILI9488_drawDot(x + i, y + rep, bg);
			line >>= 1;
		}
	}
}

void ILI9488_putc_f(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
	ILI9488_setArea(x, y, x + 4, y + 6);
	for (int i = 0; i < 5; i++) { // Char bitmap = 5 columns
		uint8_t line = font[c * 5 + i];
		uint8_t rep = 8;
		while (rep--) {
			ILI9488_sendColor((line & 0x80) ? color : bg);
			line <<= 1;
		}
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

void ILI9488_readGRAM(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t* rgb565){
	ILI9488_readArea(x,y,x + width,y + height);
	uint16_t tmp[2];
	for(uint16_t rep = 0;rep < width * height; rep++){
		tmp[0] = ILI9488_readData(); //b*5 + g*3
		tmp[1] = ILI9488_readData(); //g*3 + r*5
		char s[255];
		sprintf(s,"[0]:%04x [1]:%04x",tmp[0],tmp[1]);
		ILI9488_puts(200,0,0x0000,0xffff,s);
		rgb565[rep] = (tmp[0] & 0b0000000011111000) >> 3 | (tmp[0] & 0b0000000000000111) << 8 | (tmp[1] & 0b0000000000011111) << 11 | (tmp[1] & 0b0000000011100000) << 3;
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
	ILI9488_writeData(0b01001000);
//	ILI9488_writeData(0x00);

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

