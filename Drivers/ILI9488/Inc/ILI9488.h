/*
 * ILI9488.h
 *
 *  Created on: 2018/07/05
 *      Author: Haru
 */

#ifndef ILI9488_H_
#define ILI9488_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "string.h"
#include "font5x7.h"
#include "stdbool.h"
#include "stdlib.h"

#define FSMC_Ax	16
#define FSMC_NEx	1

#define FSMC_ADDRESS  (0x60000000 + ((FSMC_NEx-1) << 26))
#define LCD_CMD_ADDR  (FSMC_ADDRESS)

#define LCD_DATA_ADDR (FSMC_ADDRESS | 1 << (FSMC_Ax + 1))
#define LCD_CMD       (*((volatile uint16_t*) LCD_CMD_ADDR))
#define LCD_DATA      (*((volatile uint16_t*) LCD_DATA_ADDR))

static const uint16_t LCD_X = 480;
static const uint16_t LCD_Y = 320;

#define YF(a) (LCD_Y - a)

void ILI9488_writeCmd(uint16_t cmd);
void ILI9488_writeData(uint16_t data);
void ILI9488_setArea(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1);
void ILI9488_sendColor(uint16_t rgb565);
void ILI9488_drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint16_t color);
void ILI9488_drawDot(uint16_t x,uint16_t y,uint16_t color);
void ILI9488_fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint16_t color);
void ILI9488_putc(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void ILI9488_putc_f(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void ILI9488_puts(uint16_t x, uint16_t y, uint16_t color,uint16_t bg, char* str);
void ILI9488_puts_f(uint16_t x, uint16_t y, uint16_t color,uint16_t bg, char* str);
void ILI9488_init();

#endif /* ILI9488_H_ */
