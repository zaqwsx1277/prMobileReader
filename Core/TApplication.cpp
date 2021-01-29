/*
 * TApplication.cpp
 *
 *  Created on: Jan 11, 2021
 *      Author: energia
 */

#include <TApplication.hpp>

#include "TCommon.hpp"

namespace app {

//-------------------------------------------------------
/*!
 * \brief Конструктор класс
 */
TApplication::TApplication() {
	setGPIO () ;

	fAppError -> read () ;
	checkHW () ;
}
//-------------------------------------------------------
/*!
 * \brief Деструктор класса
 */
TApplication::~TApplication() {
	fAppError -> write () ;
}
//-------------------------------------------------------
/*!
 * \brief Проверка оборудования
 */
void TApplication::checkHW ()
{
//	if (HAL_I2C_IsDeviceReady (common::ptrPwrHandle, common::defPwrAddressRead, 5, common::defPwrI2cTimeout) != HAL_OK)
//		fAppError -> setError (appError::tePower) ;
//	if (HAL_I2C_IsDeviceReady (common::ptrPhotoHandle, common::defPhotoAddressRead, 5, common::defPhotoI2cTimeout) != HAL_OK)
//		fAppError -> setError (appError::tePhoto) ;
//	if (HAL_I2C_IsDeviceReady (common::ptrNfcHandle, common::defNfcAddressRead, 5, common::defNfcI2cTimeout) != HAL_OK)
//		fAppError -> setError (appError::teNfc) ;

	if (fSensorPhoto -> getState() != sensor::stOk) fAppError -> setError (appError::tePhoto) ;
}
//--------------------------------------------------------
/*!
 * \brief Перевод неиспользуемых GPIO в режим цифрового входа с подтяжкой к нулю.
 */
void TApplication::setGPIO ()
{
	GPIO_InitTypeDef GPIO_InitStruct {0} ;

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;

	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
			|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
			|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
			|GPIO_PIN_5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5
			|GPIO_PIN_7|GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_13
			|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5
			|GPIO_PIN_8|GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
			|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
			|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
//--------------------------------------------------------
/*!
 * \brief Переключение в энергосберегающий режим
 * @param inAppState Текущее состояние системы
 */
void TApplication::setPowerSaving (appState inAppState)
{

}
//--------------------------------------------------------
/*!
 * \brief установка текущего состояния системы
 * @param inAppState
 */
void TApplication::setState (appState inAppState)
{
	fAppState = inAppState ;
}
//--------------------------------------------------------
/*!
 * \brief Обработка текущего состояния
 * @return При необходимости выключить контроллер возвращается true
 */
bool TApplication::managerState ()
{
	bool retVal { false } ;

	setPowerSaving (fAppState) ;

	switch (fAppState) {
	  case astStart:
		checkHW () ;
		fAppState = astPowerSave ;
	  break;

	  case astPowerSave :

	  break ;

	  case astFinish :
		  retVal = true ;
	  break ;

	  default:

	  break;
	}
	return (retVal) ;
}
//--------------------------------------------------------
} /* namespace app */
