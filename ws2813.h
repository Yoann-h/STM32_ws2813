/*
 * ws2813.h
 *
 *  Created on: Sep 22, 2024
 *      Author: Yoann Hervagault
 *
 * derivated from: https://controllerstech.com/interface-ws2812-with-stm32/
 *                 https://forum.digikey.com/t/controlling-neopixels-with-stm32/20527
 * tested product:https://fr.vittascience.com/shop/230/led-rgb-neopixel-grove
 */

// In order to use this library, initiate a timer channel with dma.
//	  for example with timer2 and channel 3:
//	  hdma_tim2_ch3.Instance = DMA1_Channel1;
//    hdma_tim2_ch3.Init.Request = DMA_REQUEST_4;
//    hdma_tim2_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim2_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim2_ch3.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim2_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//    hdma_tim2_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//    hdma_tim2_ch3.Init.Mode = DMA_NORMAL;
//    hdma_tim2_ch3.Init.Priority = DMA_PRIORITY_VERY_HIGH;
//    if (HAL_DMA_Init(&hdma_tim2_ch3) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(tim_baseHandle,hdma[TIM_DMA_ID_CC3],hdma_tim2_ch3);
#ifndef YHLIB_WS2813_WS2813_H_
#define YHLIB_WS2813_WS2813_H_

/* Includes ------------------------------------------------------------------*/
#include "ws2813_cfg.h"

/** @defgroup WS2813_time_encoding
 * @{
 */
#define WS2813_ZERO 			((uint8_t)25)
#define WS2813_ONE 				((uint8_t)51)
#define WS2813_DMA_BUFF_SIZE 	((uint8_t)25)
#define WS2813_T0H 				((uint32_t)300)
#define WS2813_T0L 				(uint32_t)870)
#define WS2813_T1H 				((uint32_t)870)
#define WS2813_T1L 				((uint32_t)320)
#define WS2813_TRES 			((uint32_t)290000)
#define WS2813_COLOR_GREEN 		((uint32_t)0xff0000)
#define WS2813_COLOR_RED 		((uint32_t)0x00ff00)
#define WS2813_COLOR_BLUE 		((uint32_t)0x0000ff)

/**
 * @}
 */
typedef enum {
	WS2813_OK, WS2813_PARAMETER_ERROR, WS2813_INTERNAL_ERROR, WS2813_RESULTNB
} WS2813_Result;

/** @defgroup WS2813_Driver_structure  WS2813 Driver structure
 * @{
 */
typedef struct {
	WS2813_Result (*Init)(WS2813_HandlerTypeDef);
	WS2813_Result (*DeInit)(WS2813_HandlerTypeDef);
	WS2813_Result (*SetColor)(WS2813_HandlerTypeDef, eWS2813Color);
	WS2813_Result (*Reset)(WS2813_HandlerTypeDef);
} WS2813_DrvTypeDef;
/**
 * @}
 */

/** @defgroup WS2813_Pixel_structure  WS2813 Pixel definition structure
 * @{
 */
typedef union {
	struct {
		uint8_t dblue;
		uint8_t dred;
		uint8_t dgreen;
	} strcolor;
	uint32_t data;
} WS2813_PixelRGBTypeDef;
/**
 * @}
 */

/** @defgroup WS2813_Color_enum  WS2813 Color definition enum
 * @{
 */
typedef enum {
	WS2813Blue = 0,
	WS2813Red,
	WS2813Green,
	WS2813Cyan,
	WS2813Purple,
	WS2813Yellow,
	WS2813Orange,
	WS2813Off,
	WS2813Rainbow,//available only with OS active
	WS2813Colnb
} WS2813_ColorTypeDef;
/**
 * @}
 */

/** @defgroup WS2813_Handler_struct  WS2813 handler structure
 * @{
 */
typedef struct {
	WS2813_PixelRGBTypeDef WS2813_pixel[WS2813_NUM_PIXELS];
	uint32_t *WS2813_dmaBuffer;
	TIM_HandleTypeDef *WS2813_htim;
} WS2813_HandlerTypeDef;

/**
 * @}
 */
WS2813_Result WS2813_eInit(WS2813_HandlerTypeDef *Fhandler,
		TIM_HandleTypeDef *Ftdhtim, uint32_t *Fpu32Buffer);
WS2813_Result WS2813_eDeInit(WS2813_HandlerTypeDef *Fhandler);
WS2813_Result WS2813_eReset(WS2813_HandlerTypeDef *Fhandler);
WS2813_Result WS2813_eSetColor(WS2813_HandlerTypeDef *Fhandler,
		WS2813_ColorTypeDef FeColor, uint8_t Fu8LedId);
WS2813_Result WS2813_eSetColorval(WS2813_HandlerTypeDef *Fhandler,
		WS2813_PixelRGBTypeDef fu32ColorVal, uint8_t Fu8LedId);
void WS28i13_eRainbow(WS2813_HandlerTypeDef *Fhandler, uint32_t u32Speed);
void WS2813_eTest(TIM_HandleTypeDef *Ftdhtim);
void WS2813_eProcess(void);

#endif /* YHLIB_WS2813_WS2813_H_ */
