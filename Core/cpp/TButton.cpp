/*
 * TButton.cpp
 *
 *  Created on: Jul 2, 2021
 *      Author: energia
 */

#include <TButton.hpp>

namespace app {

//---------------------------------------------------
/*!
 * @param inGpioPort	Регистр GPIO
 * @param inGpio		Пин
 * @attention Я ни коем образом не проверяю правильность передачи параметров
 */
TButton::TButton(GPIO_TypeDef* inGpioPort, uint16_t inGpio)
{
	mGpioPort = inGpioPort ;
	mGpio = inGpio ;
}
//---------------------------------------------------
/*!
 * Вне зависимости от состояния тупо запускаем устранение дребезга контактов.
 */
void TButton::startBounce ()
{
	mBounce = GPIO_PIN_SET ;
}
//---------------------------------------------------
/*!
 * @return true Если дребезг устранен
 */
bool TButton::checkBounce ()
{
	bool retValue { false } ;

	if (mBounce == 0 || mBounce == 0xFF) {
		retValue = true ;
	}
	  else {
		  mBounce = mBounce << 1 ;
		  mBounce |= HAL_GPIO_ReadPin(mGpioPort, mGpio) ;
	  }

	return retValue ;
}
//---------------------------------------------------
/*!
 * @return Состояние нажатой кнопки.
 * @attention Устранение дребезга не проверяется вообще.
 */
GPIO_PinState TButton::getState ()
{
	return HAL_GPIO_ReadPin(mGpioPort, mGpio) ;
}
//---------------------------------------------------
} /* namespace app */
