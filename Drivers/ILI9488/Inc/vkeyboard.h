/*
 * vkeyboard.h
 *
 *  Created on: 2018/09/07
 *      Author: haru
 */

#ifndef ILI9488_INC_VKEYBOARD_H_
#define ILI9488_INC_VKEYBOARD_H_

#include "ILI9488.h"
#include "FONTX2.h"
#include "HR2046.h"

#define BG_COLOR 0xffff
#define FR_COLOR 0x0000

#define LCD_PUTS(x,y,str) fx_writestrFF(x,y,str,FR_COLOR,BG_COLOR)
#define LCD_drawRect(x,y,w,h,color) ILI9488_fillRect(x,y,w,h,color)
#define LCD_W 479
#define LCD_H 319

#define TS_getAxisX() HR2046_getAxis().x
#define TS_getAxisY() HR2046_getAxis().y

void vkb_getstr(char* str);
#endif /* ILI9488_INC_VKEYBOARD_H_ */
