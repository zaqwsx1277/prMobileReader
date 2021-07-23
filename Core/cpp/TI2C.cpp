/*
 * TI2C.cpp
 *
 *  Created on: Jul 12, 2021
 *      Author: AAL
 */

#include <i2c.h>

#include <TI2C.hpp>

namespace unit {

//---------------------------------------------------
/*!
 *
 * @return При успешной проверке возвращает true ;
 */
bool TI2C::check ()
{
	bool retvalue = true ;
	if (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY) retvalue = false ;
	return retvalue ;
}
//---------------------------------------------------
} /* namespace unit */
