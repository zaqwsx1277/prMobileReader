/*
 * TCallback.cpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */

#include "stm32f4xx_hal.h"

#include "rtc.h"

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
 * TIM6 - базовый таймер на 10 сек, для контроля частой установки/снятия на докстанцию
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

	if(htim -> Instance == TIM6) {
		static uint32_t count { 0 } ;
		count = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) ;
		if (count  > 0) {
			--count ;
			HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, count) ;
		}
	}
}
//-----------------------------------------------------------------------------
/*!
 * @brief Обработчик прерываний UART
 * @param huart Хэнвд сработавшего порта
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart -> Instance == USART1) {
		common::stTimeStartDebugMessage = 0 ;
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
/*!--------------------------------------------------------------------------------
 * @brief Обработчик прерываний по GPIO
 * @details При срабатывании прерывания по PA9 значерие регистра DR2 увеличивается на единицу, а раз в 10 сек  этот регистр уменьшается на единицу. При превышении определенного числа (проверяется при снятии с докстанции) включается пищалка
 * @param inGpio Номер сработавшего GPIO
 */
void HAL_GPIO_EXTI_Callback(uint16_t inGpio) {

	switch (inGpio) {
	  case GPIO_PIN_9: { 	// Фиксируем снятие с докстанции. Нужно для выписывание пи...ы если кто-то будет играться с установкой и снятием на докстанцию.
		  uint32_t count { 0 } ;
		  count = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) ;
		  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, count + 1) ;
	  }
	  break;

	  default:
	  break;
	}
}
//--------------------------------------------------------------------------------
/*! @} */



