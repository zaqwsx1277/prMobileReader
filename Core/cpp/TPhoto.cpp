/*
 * TPhoto.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#include "dcmi.h"

#include "TPhoto.hpp"
#include "TCommon.hpp"

namespace unit {

/*! -----------------------------------------------------------------
 *	Конструктор инициализирующий камеру и переводящий ее в режим sleep
 */
TPhoto::TPhoto() {
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
	if (HAL_I2C_IsDeviceReady(&hi2c2, stPhotoI2CRead, 1, stPhotoI2CTimeout) != HAL_OK) common::app -> setState (app::appState::appPhotoI2CErr) ;
	sleep () ;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
}
/*! -----------------------------------------------------------------
 *
 */
TPhoto::~TPhoto() {
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
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
	return true ;
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
 */
bool TPhoto::process ()
{
//	bool retValue { false } ;

	HAL_GPIO_WritePin (GPIOB,GPIO_PIN_8,GPIO_PIN_RESET) ;
	wakeup () ;

	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT , (uint32_t) common::stPhotoBuf, 1600*22) ;
																	// Ждём оцифровки изображения или вылетаем по таймауту
	while (HAL_DCMI_GetState (&hdcmi) != HAL_DCMI_STATE_READY) {
		if (common::app -> getState().second > stPhotoDCMITimeout) common::app -> setState(app::appState::appPhotoTimeout) ;
	}
	if (common::app -> getState().first == app::appState::appPhoto) common::app -> writePhoto() ;

	sleep () ;
	HAL_GPIO_WritePin (GPIOB,GPIO_PIN_8,GPIO_PIN_SET) ;

	return common::app -> getState().first == app::appState::appPhoto ? true : false ;
}
//-------------------------------------------------------------------
} /* namespace unit */
