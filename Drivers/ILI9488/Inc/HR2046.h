/*
 * HR2046.h
 *
 *  Created on: 2018/09/03
 *      Author: haru
 */

#ifndef ILI9488_INC_HR2046_H_
#define ILI9488_INC_HR2046_H_

#include "stm32f4xx_hal.h"

#define TS_CS_Pin GPIO_PIN_1
#define TS_CS_GPIO_Port GPIOC

#define NOTOUCH 9999

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t z;
}TSR_t;

#define TS_W_MAX 3940
#define TS_W_MIN 192
#define TS_H_MAX 3840
#define TS_H_MIN 129

#define TS_Z 10

#define LCD_W 479
#define LCD_H 319

void HR2046_init();
TSR_t HR2046_read();
TSR_t HR2046_adjust();
TSR_t HR2046_getAxis();
#endif /* ILI9488_INC_HR2046_H_ */
