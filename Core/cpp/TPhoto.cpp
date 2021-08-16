/*
 * TPhoto.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#include "dcmi.h"
#include "dma.h"

#include "TPhoto.hpp"
#include "TCommon.hpp"

namespace unit {

/*! -----------------------------------------------------------------
 *	Конструктор инициализирующий камеру
 */
TPhoto::TPhoto() {
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_Delay(50) ;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);

	init () ;
}
/*! -----------------------------------------------------------------
 *
 */
TPhoto::~TPhoto() {
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_DMA_Abort(&hdma_dcmi);
	HAL_DCMI_Stop(&hdcmi) ;
	sleep () ;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
}
/*! -----------------------------------------------------------------
 *
 * @return При успешной проверке возвращается true
 */
bool TPhoto::check ()
{
	bool retValue { true } ;

	if (HAL_I2C_IsDeviceReady(&hi2c2, stPhotoI2CRead, 1, stPhotoI2CTimeout) != HAL_OK) {
		common::app -> makeInfo(app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 3) ;
		common::app -> setState (app::appState::appPhotoI2CErr) ;

		retValue = false ;
	}

	return retValue ;
}
/*! -----------------------------------------------------------------
 *	Задаются все конфигурацилнные настройки
 */
void TPhoto::init ()
{
	OV2640_JPEGConfig(JPEG_320x240);
	OV2640_BrightnessConfig(0x20);
	OV2640_AutoExposure(2);
}
/*! -----------------------------------------------------------------
 *
 */
void TPhoto::sleep ()
{
	OV2640_Soft_Standby () ;
}
/*! -----------------------------------------------------------------
 *
 */
void TPhoto::wakeup ()
{
	OV2640_Soft_Wakeup() ;
}
/*! -----------------------------------------------------------------
 * \attention Я ни коим образом не проверяю корректность инициализации и возвращаемые ошибки!!!
 * \todo Переделать ожидание получения изображения
 */
bool TPhoto::process ()
{
	HAL_GPIO_WritePin (GPIOB,GPIO_PIN_8,GPIO_PIN_RESET) ;
	wakeup () ;

	common::stPhotoBuf [0] = 0 ;			// Очищаем маркер начала jpeg
	common::stPhotoBuf [1] = 0 ;

	common::app -> makeInfo(app::typeInfo::infoAudio, app::typeSound::tsndContinue) ;
	HAL_Delay(stPhotoPositionTimeout) ;
	common::app -> makeInfo(app::typeInfo::infoAudio, app::typeSound::tsndNo) ;
	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t) common::stPhotoBuf, 1600*22) ;

	tmpFlag = false ;
	tmpCount =  10 ;

	while (!tmpFlag) {						// Ждём оцифровки изображения или вылетаем по таймауту
//	while (HAL_DCMI_GetState (&hdcmi) != HAL_DCMI_STATE_READY && common::stPhotoBuf [0] != 0xFF && common::stPhotoBuf [1] != 0xD8) {
		if (common::app -> getState().second > (stPhotoDCMITimeout + stPhotoPositionTimeout)) {
			common::app -> setState(app::appState::appPhotoTimeout) ;
			break ;
		}
	}
	if (common::app -> getState().first == app::appState::appPhoto) {
		common::app -> writePhoto() ;
	}

	sleep () ;
	HAL_GPIO_WritePin (GPIOB,GPIO_PIN_8,GPIO_PIN_SET) ;

	return common::app -> getState().first == app::appState::appPhoto ? true : false ;
}
//-------------------------------------------------------------------
} /* namespace unit */
