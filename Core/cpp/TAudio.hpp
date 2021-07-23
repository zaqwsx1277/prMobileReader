/*
 * TAudio.hpp
 *
 *  Created on: Jul 7, 2021
 *      Author: AAL
 */

#ifndef CPP_TAUDIO_HPP_
#define CPP_TAUDIO_HPP_

#include <stdint.h>

#include "TUnit.hpp"

namespace unit {

constexpr uint32_t stAudioDuration { 10000 } ;	///< Максимальная длительность записи аудио

/*!---------------------------------------------------------------------------------
 * Класс для работы со звуком
 */
class TAudio : public TUnit {
public:
	TAudio();
	virtual ~TAudio();

	bool check () ;			///< Проверка работоспособности аудио (запись и вопроизведение пищалки)

	void init () { ; }		///< Инициализация устройства
	void sleep () { ; }		///< Перевод в режим энергосбережения
	void wakeup () { ; }	///< Выход из режима энергосбережения
	bool process () { return true ; }	///< Получение данных с микрофона и запись его в файл
};

} /* namespace unit */

#endif /* CPP_TAUDIO_HPP_ */
