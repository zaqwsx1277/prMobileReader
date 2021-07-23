/*
 * TTag.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#include <usart.h>
#include <rtc.h>

#include "TTag.hpp"
#include "TCommon.hpp"

namespace unit {

//--------------------------------------------------------
TTag::TTag() {
	wakeup () ;
	sleep () ;
	if (common::app -> getState().first == app::appState::appTag) common::app -> debugMesage("Scan TAG init - OK") ;
}
//--------------------------------------------------------
TTag::~TTag() {
	sleep () ;
}
//--------------------------------------------------------
bool TTag::check ()
{
	return true ;
}
//--------------------------------------------------------
void TTag::sleep ()
{
	if (HAL_UART_Transmit(&huart2, (uint8_t *) stTagCmdSleep, sizeof (stTagCmdSleep), stTagUsartTimeOut) != HAL_OK) common::app -> setState(app::appState::appTagErr) ;
	HAL_Delay(50);
}
//--------------------------------------------------------
void TTag::wakeup ()
{
	if (HAL_UART_Transmit(&huart2, (uint8_t *) stTagCmdAckWakeUp, sizeof (stTagCmdAckWakeUp), stTagUsartTimeOut) != HAL_OK) common::app -> setState(app::appState::appTagErr) ;
	HAL_Delay(50);
}
//--------------------------------------------------------
/*!
 * Метод сканирует метку и записывает ее в регистр
 * @return При успешном сканировании возвращает true
 */
bool TTag::process ()
{
	bool retvalue { false } ;

	wakeup () ;
	if (HAL_UART_Transmit(&huart2, (uint8_t *) stTagCmdScan, sizeof (stTagCmdScan), stTagUsartTimeOut) != HAL_OK) common::app -> setState(app::appState::appTagErr) ;
	  else {
		if (HAL_UART_Receive (&huart2, common::stTagBufId, sizeof (common::stTagBufId), stTagUsartWaitID) != HAL_OK) common::app -> setState(app::appState::appTagNoId) ;
		  else {
			uint32_t temp = (common::stTagBufId [20] << 24) | (common::stTagBufId [21] << 16) | (common::stTagBufId [22] << 16) | common::stTagBufId [23]  ;
			HAL_RTCEx_BKUPWrite (&hrtc, temp, RTC_BKP_DR0) ;
			common::app -> debugMesage("Scan TAG - OK") ;
			retvalue = true ;
		  }
	  }
	sleep () ;
	return retvalue ;
}
/*!---------------------------------------------------------
 * @param inPtr Указатель куда копируем метку
 */
void TTag::copyTag (uint8_t *inPtr)
{
	uint32_t temp = HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR0) ;
	*((uint32_t *) inPtr) = temp ;
}
//----------------------------------------------------------
} /* namespace unit */
