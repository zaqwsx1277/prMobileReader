/*
 * TVoice.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: energia
 */

#include <TVoice.hpp>

namespace app {
/*!
 * \brief Формирование звука указанного типа и длительности
 * \details При длительности 0xFFFF звук будет навсегда.
 * \todo Необходимо потом переделать на работу через таймер
 * @param inType		Тип формируемого звука
 * @param inDuration	Длительность формируемого звука в мСек
 */
void TVoice::make (const voiceType inType, uint32_t inDuration)
{
	uint32_t numPic = 0 ;							// Кличество пиков
	if (inDuration == 0) inDuration = 0xFFFF ;		// Избегаем деления на ноль
	switch (inType) {
	  case vtClear:
		HAL_GPIO_WritePin(fPtrHandle, fPIn, GPIO_PIN_RESET);
	  break;

	  case vtContinue:
		HAL_GPIO_WritePin(fPtrHandle, fPIn, GPIO_PIN_SET);
	  break ;

	  case vtShort:
		numPic = inDuration / defVoiceShort ;
		HAL_GPIO_WritePin(fPtrHandle, fPIn, GPIO_PIN_SET);
	  break ;

	  case vtLong:
		numPic = inDuration / defVoiceLong ;
		HAL_GPIO_WritePin(fPtrHandle, fPIn, GPIO_PIN_SET);
	  break ;

	  default:
	  break;
	}

	if (inDuration != 0xFFFF) {		// Крутим цикл для нужной длительности
		do {
			HAL_Delay()
		} while (--numPic > 0) ;
	}
}
//-----------------------------------------------------------------------
} /* namespace app */
