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
/*!-----------------------------------------------------------------------------
 * @brief Обработчик прерываний по Vsync от камеры DCMI
 * @param hdcmi Хендл устройства
 * @attention Нужен исключительно, что бы остановить считывание изображения с камеры.
 */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	if (--tmpCount <= 0) {
		HAL_DCMI_Stop(hdcmi) ;
		HAL_DMA_Abort(&hdma_dcmi);
		tmpFlag = true ;
	}
}
/*!------------------------------------------------------------------------------
 * @brief Обработчик прерываний по заполнению данными PDM первой половины аудио буфера
 * @attention Если обработка предыдущего буфера ещё не закончена, то принятый буфер просто теряется
 */
void 	HAL_I2S_RxHalfCpltCallback (I2S_HandleTypeDef *hi2s)
{
	if (common::stAudioBufId == unit::crAudioBufID::crStop) {
		common::stAudioBufId = unit::crAudioBufID::crFirst ;
	}
}
/*!------------------------------------------------------------------------------
 * @brief Обработчик прерываний по заполнению данными PDM второй половины аудио буфера
 * @attention Если обработка предыдущего буфера ещё не закончена, то принятый буфер просто теряется
 */
void HAL_I2S_RxCpltCallback (I2S_HandleTypeDef *hi2s)
{
	if (common::stAudioBufId == unit::crAudioBufID::crStop) {
		common::stAudioBufId = unit::crAudioBufID::crSecond ;
	}
}
/*!
 * Обработчик прерываний по GPIO
 * @param inGpio Номер сработавшего GPIO
 */
void HAL_GPIO_EXTI_Callback(uint16_t inGpio) {

	switch (inGpio) {
	  case GPIO_PIN_9: 	// Финсируем снятие с докстанции. Нужно для выписывание пи...ы если кто-то будет играться с установкой и снятием на докстанцию.
//		if (common::app -> getState().first == app::appState::appDoc)
//			common::app -> setState()

	  break;

	  default:
	  break;
	}
}
/*! @} */



