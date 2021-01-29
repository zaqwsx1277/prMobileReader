/*
 * TSensorPhoto.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: energia
 */

#include <TSensorPhoto.hpp>

namespace sensor {
//-------------------------------------------------------------
TSensorPhoto::TSensorPhoto() {
	checkSate () ;
}
//-------------------------------------------------------------
/*!
 * \brief Проверка состояния видео камеры
 * @return
 */
tpState TSensorPhoto::checkSate ()
{
	if (HAL_I2C_IsDeviceReady (ptrHandle, defAddressRead, 5, fTimeout) == HAL_OK) fState = stOk ;
	  else fState = stErr ;

	return fState ;
}
//-------------------------------------------------------------
tpState TSensorPhoto::getState ()
{
	return fState ;
}
//-------------------------------------------------------------
} /* namespace sensor */
