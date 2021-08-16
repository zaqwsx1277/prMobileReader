/*
 * TBq25121.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#include <i2c.h>

#include <TBq25121.hpp>
#include "TCommon.hpp"
#include "TApplication.hpp"

namespace unit {

//--------------------------------------------------
/*!
 * В конструкторе класса всегда программируется работа чипа BQ25121
 * При возникновении ошибки будет подан сигнал Один длинный гудок и два коротких
 */
TBq25121::TBq25121() {
	if (HAL_I2C_IsDeviceReady(&hi2c2, defBq25121AddrRead, 1, defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;

	init () ;
}
//--------------------------------------------------
TBq25121::~TBq25121() {
	// TODO Auto-generated destructor stub


}
/*!--------------------------------------------------
 * Полный аппаратный сброс чипа и программирование регистров
 */
void TBq25121::init ()
{
	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_reset, sizeof (defBq25121Cmd_reset), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;

	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_09, sizeof (defBq25121Cmd_09), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;
	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_0B, sizeof (defBq25121Cmd_0B), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;
	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_06, sizeof (defBq25121Cmd_06), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;
	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_03, sizeof (defBq25121Cmd_03), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;
	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_04, sizeof (defBq25121Cmd_04), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;
	if (HAL_I2C_Master_Transmit(&hi2c2, defBq25121AddrWrite, (uint8_t *) defBq25121Cmd_08, sizeof (defBq25121Cmd_08), defBq25121TimeOut) != HAL_OK) common::app -> setState(app::appState::appErrBq25121) ;

}
//--------------------------------------------------
bool TBq25121::check ()
{
	return true ;
}
//--------------------------------------------------
} /* namespace bq25121 */
