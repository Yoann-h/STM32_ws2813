/*
 * ws2813_cfg.h
 *
 *  Created on: Sep 22, 2024
 *      Author: Yoann Hervagault
 */

#ifndef YHLIB_WS2813_WS2813_CFG_H_
#define YHLIB_WS2813_WS2813_CFG_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

//number of controlled led elements
#define WS2813_NUM_PIXELS 		1
#define WS2813_NUMMAX_PIXELS 	128
#define WS2813_DMA_BUFFER_SIZE	25
#define WS2813_TIM_CHANNEL	TIM_CHANNEL_3

#endif /* YHLIB_WS2813_WS2813_CFG_H_ */
