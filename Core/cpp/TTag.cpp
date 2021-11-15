/*
 * TTag.cpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */
#include <string>
#include <sstream>
#include <ios>
#include <iomanip>

#include <usart.h>
#include <rtc.h>

#include "TTag.hpp"
#include "TCommon.hpp"

namespace unit {
/*!--------------------------------------------------------
 * @attention Работа со сканированием метки должна инициализироваться при запуске контроллера, что бы загнать чит в режим Sleep
 */
TTag::TTag() {
//	wakeup () ;
//	sleep () ;
//	common::app -> debugMessage("Scan TAG init") ;
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
	common::app -> debugMessage("NFC sleep") ;
}
//--------------------------------------------------------
void TTag::wakeup ()
{
	if (HAL_UART_Transmit(&huart2, (uint8_t *) stTagCmdAckWakeUp, sizeof (stTagCmdAckWakeUp), stTagUsartTimeOut) != HAL_OK) common::app -> setState(app::appState::appTagErr) ;
	HAL_Delay(50);
	common::app -> debugMessage("NFC wakeup") ;
}
//--------------------------------------------------------
/*!
 * Метод сканирует метку и записывает ее в регистр RTC_BKP_DR0
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
			uint32_t temp = (common::stTagBufId [20] << 24) | (common::stTagBufId [21] << 16) | (common::stTagBufId [22] << 8) | common::stTagBufId [23]  ;
			HAL_RTCEx_BKUPWrite (&hrtc, RTC_BKP_DR0, temp) ;

			std::stringstream tempTag ;
			tempTag << std::hex << std::setfill('0') << std::setw (2) << static_cast<uint32_t> (common::stTagBufId [20]) << " "
													 << std::setw (2) << static_cast<uint32_t> (common::stTagBufId [21]) << " "
													 << std::setw (2) << static_cast<uint32_t> (common::stTagBufId [22]) << " "
													 << std::setw (2) << static_cast<uint32_t> (common::stTagBufId [23]) ;
			common::app -> debugMessage("Scan TAG - OK / " + tempTag.str()) ;
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
