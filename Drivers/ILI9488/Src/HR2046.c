/*
 * HR2046.c
 *
 *  Created on: 2018/09/03
 *      Author: haru
 */
//Caution: too many workaround. please adjust your environment
#include "HR2046.h"

SPI_HandleTypeDef tsspi;

#define TS_CS_H HAL_GPIO_WritePin(TS_CS_GPIO_Port,TS_CS_Pin,GPIO_PIN_SET)
#define TS_CS_L HAL_GPIO_WritePin(TS_CS_GPIO_Port,TS_CS_Pin,GPIO_PIN_RESET)

void TS_SPI_init() {

	/* SPI2 parameter configuration*/
	tsspi.Instance = SPI2;
	tsspi.Init.Mode = SPI_MODE_MASTER;
	tsspi.Init.Direction = SPI_DIRECTION_2LINES;
	tsspi.Init.DataSize = SPI_DATASIZE_8BIT;
	tsspi.Init.CLKPolarity = SPI_POLARITY_LOW;
	tsspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	tsspi.Init.NSS = SPI_NSS_SOFT;
	tsspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	tsspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	tsspi.Init.TIMode = SPI_TIMODE_DISABLE;
	tsspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	tsspi.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&tsspi) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = TS_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TS_CS_GPIO_Port, &GPIO_InitStruct);
}

void HR2046_init() {
	TS_SPI_init();
	TS_CS_H;
}

uint16_t HR2046_SPI_read(uint8_t cmd) {
	TS_CS_L;
	uint8_t receive[2];
	while (HAL_SPI_GetState(&tsspi) == HAL_SPI_STATE_BUSY);
	HAL_SPI_Transmit(&tsspi, &cmd, 1, 100);
	while (HAL_SPI_GetState(&tsspi) == HAL_SPI_STATE_BUSY);
	HAL_SPI_Receive(&tsspi, receive, 2, 100);
	TS_CS_H;
	return receive[0] << 5 | receive[1] >> 3;
}

TSR_t HR2046_read() {
	TSR_t ret;
	ret.x = HR2046_SPI_read(0b10010011); //x:measure
	ret.y = HR2046_SPI_read(0b11010011); //y:measure
	ret.z = HR2046_SPI_read(0b10110011); //z2:measure
	//omitting pressure detection because it isn't necessary
	return ret;
}

TSR_t HR2046_adjust() {
	TSR_t ret = HR2046_read();
	if (ret.z > TS_Z) {
		if (ret.x > TS_W_MAX)
			ret.x = TS_W_MAX;
		if (ret.x < TS_W_MIN)
			ret.x = TS_W_MIN;
		if (ret.y > TS_H_MAX)
			ret.y = TS_H_MAX;
		if (ret.y < TS_H_MIN)
			ret.y = TS_H_MIN;
	} else {
		ret.x = 9999;
		ret.y = 9999;
	}
	return ret;
}

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

TSR_t HR2046_getAxis() {
	TSR_t ret = HR2046_adjust();
	if(ret.z > TS_Z){
	ret.x = LCD_W - map(ret.x,TS_W_MIN,TS_W_MAX,0,LCD_W);
	ret.y = LCD_H - map(ret.y,TS_H_MIN,TS_H_MAX,0,LCD_H);
	}
	return ret;
}
