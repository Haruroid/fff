/*
 * vkeyboard.c
 *
 *  Created on: 2018/09/07
 *      Author: haru
 */
#include "vkeyboard.h"

uint8_t line_table[4][2][14] = {
		{{'1','2','3','4','5','6','7','8','9' ,'0','-','^' ,'\\','<'},{'!','\"','#','$','%','&','\'','(',')',' ','=','~','|','<'}},
		{{' ','q','w','e','r','t','y','u','i' ,'o','p','@' ,'[' ,'E'},{' ','Q' ,'W','E','R','T','Y' ,'U','I','O','P','@','{','E'}},
		{{'S','a','s','d','f','g','h','j','k' ,'l',';',':' ,']' ,'S'},{'S','A' ,'S','D','F','G','H' ,'J','K','L','+','*','}','S'}},
		{{' ','z','x','c','v','b','n','m',',' ,'.','/','\\',' ' ,' '},{' ','Z' ,'X','C','V','B','N' ,'M','<','>','?','_',' ',' '}}
};

void show_qwerty(bool isShift) { 	//this is 4line qwerty-like keyboard.
	uint16_t x_base = LCD_W / 14, y_base = LCD_H / 6;

	LCD_drawRect(0, y_base*2, LCD_W, LCD_H, 0xffff);
	for (uint8_t i = 2; i < 6; i++) // vertical flame
		LCD_drawRect(0, y_base * i, LCD_W, 1, 0x0000);
	for (uint8_t i = 0; i < 14; i++) //horizontal flame
		LCD_drawRect(i * x_base, y_base * 2, 1, LCD_H, 0x0000);
	char ch[2];
	uint16_t x;
	uint16_t y;
	for (uint8_t i = 0; i < 4; i++) {
		x = 0;
		y = (y_base * (2 + i)) + 2;
		for (uint8_t c = 0; c < 14; c++) {
			x = (x_base * c) + 2;
			sprintf(ch, "%c", line_table[i][isShift][c]);
			LCD_PUTS(x, y, ch);
		}
	}
}

void vkb_getstr(char* str) {
	uint16_t x_base = LCD_W / 14, y_base = LCD_H / 6;
	bool isShift = false;
	show_qwerty(isShift);
	uint16_t tx,ty,p = 0;
	while(1){
		tx = TS_getAxisX();
		ty = TS_getAxisY();
		if(tx != 9999){
			tx /= x_base;
			ty /= y_base;
			ty -= 2;
			if(ty == 0 && tx == 13 && p > 0){
				p--;
				str[p] = '\0';
				LCD_drawRect(0,0,LCD_W,16,BG_COLOR);
				LCD_PUTS(0,0,str);
				HAL_Delay(10);
			}else if( ty == 2 && (tx == 0 || tx == 13)){
				isShift = !isShift;
				show_qwerty(isShift);
			}else if(ty == 1 && tx == 13){
				break;
			}else{
				str[p] = line_table[ty][isShift][tx];
				p++;
				str[p] = '\0';
				LCD_PUTS(0,0,str);
				HAL_Delay(100);
			}
		}
	}
}
