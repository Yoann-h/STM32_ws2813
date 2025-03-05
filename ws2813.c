/*
 * ws2813.c
 *
 *  Created on: Sep 22, 2024
 *      Author: Yoann Hervagault
 *
 * derivated from: https://controllerstech.com/interface-ws2812-with-stm32/
 * 				   https://forum.digikey.com/t/controlling-neopixels-with-stm32/20527
 * tested product:https://fr.vittascience.com/shop/230/led-rgb-neopixel-grove
 */

/* Includes ------------------------------------------------------------------*/
#include "ws2813.h"

WS2813_HandlerTypeDef ledhandler;
uint8_t u8WS2813_idatasentflag = 0;
uint8_t u8WS2813_iCommand = WS2813Off;
/** @defgroup ws2813_Private_Variables
 * @{
 */
WS2813_DrvTypeDef ws2813Drv = { WS2813_eInit, WS2813_eDeInit, WS2813_eSetColor,
		WS2813_eReset };

/**
 * @}
 */

WS2813_Result WS2813_eInit(WS2813_HandlerTypeDef *Fhandler,
		TIM_HandleTypeDef *Ftdhtim, uint32_t *Fpu32Buffer) {
	WS2813_Result eResult = WS2813_OK;
	if (Fhandler == 0 || Ftdhtim == 0 || Fpu32Buffer == 0) {
		eResult = WS2813_PARAMETER_ERROR;
	} else {
		Fhandler->WS2813_htim = Ftdhtim;
		Fhandler->WS2813_dmaBuffer = Fpu32Buffer;
	}
	return eResult;
}

WS2813_Result WS2813_eDeInit(WS2813_HandlerTypeDef *Fhandler) {
	WS2813_Result eResult = WS2813_OK;
	if (Fhandler == 0) {
		eResult = WS2813_PARAMETER_ERROR;
	} else {
		Fhandler->WS2813_htim = 0;
		Fhandler->WS2813_dmaBuffer = 0;
	}
	return eResult;
}

WS2813_Result WS2813_eReset(WS2813_HandlerTypeDef *Fhandler) {
	WS2813_Result eResult = WS2813_OK;
	for (uint8_t i = 0; i < WS2813_NUM_PIXELS; i++) {
		eResult |= WS2813_SetColor(Fhandler, WS2813Off, i);
	}
	return eResult;
}

WS2813_Result WS2813_eSetColor(WS2813_HandlerTypeDef *Fhandler,
		WS2813_ColorTypeDef FeColor, uint8_t Fu8LedId) {
	WS2813_Result eResult;
	WS2813_PixelRGBTypeDef fu32ColorVal;
	switch ((uint8_t) FeColor) {
	case WS2813Blue:
		fu32ColorVal.strcolor.dblue = 255;
		fu32ColorVal.strcolor.dred = 0;
		fu32ColorVal.strcolor.dgreen = 0;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Red:
		fu32ColorVal.strcolor.dblue = 0;
		fu32ColorVal.strcolor.dred = 255;
		fu32ColorVal.strcolor.dgreen = 0;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Green:
		fu32ColorVal.strcolor.dblue = 0;
		fu32ColorVal.strcolor.dred = 0;
		fu32ColorVal.strcolor.dgreen = 255;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Cyan:
		fu32ColorVal.strcolor.dblue = 255;
		fu32ColorVal.strcolor.dred = 0;
		fu32ColorVal.strcolor.dgreen = 255;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Purple:
		fu32ColorVal.strcolor.dblue = 128;
		fu32ColorVal.strcolor.dred = 128;
		fu32ColorVal.strcolor.dgreen = 0;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Yellow:
		fu32ColorVal.strcolor.dblue = 0;
		fu32ColorVal.strcolor.dred = 255;
		fu32ColorVal.strcolor.dgreen = 255;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Orange:
		fu32ColorVal.strcolor.dblue = 0;
		fu32ColorVal.strcolor.dred = 255;
		fu32ColorVal.strcolor.dgreen = 137;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	case WS2813Off:
		fu32ColorVal.strcolor.dblue = 0;
		fu32ColorVal.strcolor.dred = 0;
		fu32ColorVal.strcolor.dgreen = 0;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	default:
		fu32ColorVal.strcolor.dblue = 0;
		fu32ColorVal.strcolor.dred = 0;
		fu32ColorVal.strcolor.dgreen = 0;
		WS2813_eSetColorval(Fhandler, fu32ColorVal, Fu8LedId);
		break;
	}
}

WS2813_Result WS2813_eSetColorval(WS2813_HandlerTypeDef *Fhandler,
		WS2813_PixelRGBTypeDef fu32ColorVal, uint8_t Fu8LedId) {
	WS2813_Result eResult = WS2813_OK;
	if (Fu8LedId >= WS2813_NUM_PIXELS) {
		eResult = WS2813_PARAMETER_ERROR;
	} else {
		for (uint8_t u8LedId = 0; u8LedId <= Fu8LedId; u8LedId++) {
			if (u8LedId == Fu8LedId)
				Fhandler->WS2813_pixel[u8LedId] = fu32ColorVal;
			uint32_t *pBuff;
			pBuff = Fhandler->WS2813_dmaBuffer;
			for (int8_t j = 23; j >= 0; j--) {
				if ((Fhandler->WS2813_pixel[Fu8LedId].data >> j) & 0x01) {
					*pBuff = WS2813_ONE;
				} else {
					*pBuff = WS2813_ZERO;
				}
				pBuff++;
			}
			Fhandler->WS2813_dmaBuffer[WS2813_DMA_BUFF_SIZE - 1] = 0;
			HAL_TIM_PWM_Start_DMA(Fhandler->WS2813_htim, WS2813_TIM_CHANNEL,
					Fhandler->WS2813_dmaBuffer, WS2813_DMA_BUFF_SIZE);
			while (!u8WS2813_idatasentflag) {
			};
			u8WS2813_idatasentflag = 0;
		}
	}
	return eResult;
}

void WS28i13_eRainbow(WS2813_HandlerTypeDef *Fhandler, uint32_t u32Speed) {
	WS2813_PixelRGBTypeDef sColor;
	int i, j;
	uint16_t stepSize = 4;
	j = 0;
	static int k = 0;
	while (1) {
		for (i = (WS2813_NUM_PIXELS - 1); i > 0; i--) {
			Fhandler->WS2813_pixel[i].data = Fhandler->WS2813_pixel[i - 1].data;
		}

		if (k < 255) {
			sColor.strcolor.dgreen = 254 - k; //[254, 0]
			sColor.strcolor.dred = k + 1;  //[1, 255]
			sColor.strcolor.dblue = 0;
		} else if (k < 510) {
			sColor.strcolor.dgreen = 0;
			sColor.strcolor.dred = 509 - k; //[254, 0]
			sColor.strcolor.dblue = k - 254; //[1, 255]
			j++;
		} else if (k < 765) {
			sColor.strcolor.dgreen = k - 509; //[1, 255];
			sColor.strcolor.dred = 0;
			sColor.strcolor.dblue = 764 - k; //[254, 0]
		}
		k = (k + stepSize) % 765;

		// not so bright
		sColor.strcolor.dgreen >>= 2;
		sColor.strcolor.dred >>= 2;
		sColor.strcolor.dblue >>= 2;

		Fhandler->WS2813_pixel[0] = sColor;
		WS2813_eSetColorval(Fhandler, sColor, 0);
		HAL_Delay(u32Speed);
	}
}

void WS2813_eTest(TIM_HandleTypeDef *Ftdhtim)
{
	  WS2813_HandlerTypeDef ledhandler;
	  uint32_t ledBuffer[WS2813_DMA_BUFFER_SIZE];
	  WS2813_eInit(&ledhandler, Ftdhtim, ledBuffer);
	  uint8_t u8Color=0;
	  while (1)
	  {
	    /* USER CODE END WHILE */

	    /* USER CODE BEGIN 3 */
		WS2813_eSetColor(&ledhandler, u8Color, 0);
		u8Color++;
		if(u8Color==WS2813Off)
		{
			u8Color=0;
			WS28i13_eRainbow(&ledhandler, 100);
		}
		HAL_Delay(1000);
	  }
}

void WS2813_eProcess(void)
{
	  uint32_t ledBuffer[WS2813_DMA_BUFFER_SIZE];
	  WS2813_PixelRGBTypeDef sColor;
	  int i,j;
	  j=0;
	  uint16_t stepSize = 4;
	  static int k = 0;
	  while (1)
	  {
		  if(u8WS2813_iCommand!=WS2813Rainbow){
			  WS2813_eSetColor(&ledhandler, u8WS2813_iCommand, 0);
			  osDelay(1000);
		  }else{
			  for (i = (WS2813_NUM_PIXELS - 1); i > 0; i--) {
				  ledhandler.WS2813_pixel[i].data = ledhandler.WS2813_pixel[i - 1].data;
			  		}

			  		if (k < 255) {
			  			sColor.strcolor.dgreen = 254 - k; //[254, 0]
			  			sColor.strcolor.dred = k + 1;  //[1, 255]
			  			sColor.strcolor.dblue = 0;
			  		} else if (k < 510) {
			  			sColor.strcolor.dgreen = 0;
			  			sColor.strcolor.dred = 509 - k; //[254, 0]
			  			sColor.strcolor.dblue = k - 254; //[1, 255]
			  			j++;
			  		} else if (k < 765) {
			  			sColor.strcolor.dgreen = k - 509; //[1, 255];
			  			sColor.strcolor.dred = 0;
			  			sColor.strcolor.dblue = 764 - k; //[254, 0]
			  		}
			  		k = (k + stepSize) % 765;

			  		// not so bright
			  		sColor.strcolor.dgreen >>= 2;
			  		sColor.strcolor.dred >>= 2;
			  		sColor.strcolor.dblue >>= 2;

			  		ledhandler.WS2813_pixel[0] = sColor;
			  		WS2813_eSetColorval(&ledhandler, sColor, 0);
			  		osDelay(100);
		  }

	  }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	HAL_TIM_PWM_Stop_DMA(htim, WS2813_TIM_CHANNEL);
	u8WS2813_idatasentflag = 1;
}
