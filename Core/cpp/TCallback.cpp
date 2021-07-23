/*
 * TCallback.cpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */

#include "stm32f4xx_hal.h"

#include "TCommon.hpp"
#include "TApplication.hpp"

/*!
 * \defgroup Callback Обработчики прерываний
 * @{
 */

//-----------------------------------------------------------------------------
/*!
 * @brief Обработчик прерываний от таймера
 * @param htim Сработавший таймер
 *
 * TIM7 - базовый таймер на 1 мСек.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM7) {
		auto state = common::app -> getState() ;
		switch (state.first) {
		  case app::appState::appCheckBounce :
			common::app -> checkBounce () ;
		  break;

		  default:
		  break;
		}
	}
}
//-----------------------------------------------------------------------------
/*!
 * @brief Обработчик прерываний UART
 * @param huart Хэнвд сработавшего порта
 * @attention И на хрена он нужен???
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart -> Instance == USART1) {

	}
}
//-----------------------------------------------------------------------------
/*! @} */



