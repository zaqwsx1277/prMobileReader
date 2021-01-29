/*
 * TVoice.hpp
 *
 *  Created on: Jan 28, 2021
 *      Author: energia
 */

#ifndef TVOICE_HPP_
#define TVOICE_HPP_

#include <stdint.h>

#include "stm32f4xx_hal.h"

namespace app {

/*!
 * \brief Типы звуков
 */
enum voiceType {
	vtClear,   //!< Без звука
	vtShort,   //!< Короткое пикание длительность пика defVoiceShort мСек
	vtLong,    //!< Длинное пикание длительность пика 1 сек
	vtContinue,//!< Непрерывное звучание
	vtCount
};

constexpr uint16_t defVoiceShort = 100 ;	///< длительность короткого пика
constexpr uint16_t defVoiceLong = 1000 ;	///< длительность длинного пика

/*!
 * \brief Класс работы со звуком
 */
class TVoice {
private:
	GPIO_TypeDef* fPtrHandle { GPIOD } ;		///< Указатель на порт
	uint16_t fPIn { GPIO_PIN_13 } ;				///< Пин

public:
	TVoice() = default ;
	virtual ~TVoice() = default ;

	void make (const voiceType, const uint32_t) ; ///< Формирование звука
};

} /* namespace app */

#endif /* TVOICE_HPP_ */
