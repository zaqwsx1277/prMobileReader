/*
 * TApplication.hpp
 *
 *  Created on: Jan 11, 2021
 *      Author: energia
 */

#ifndef TAPPLICATION_HPP_
#define TAPPLICATION_HPP_

#include <TAppError.hpp>
#include <memory>

#include "rtc.h"

#include "TSensorPhoto.hpp"

namespace app {

/*!
 * \brief Возможные состояния приложения
 */
enum appState {
	astUnknown, 	///< Хрен его знает, что за состояние
	astStart,		///< Режим первоначального запуска
	astPowerSave,   ///< Режим энергосбережения
	astPhoto,		///< Режим получения фото
	astVoice,		///< Режим записи аудио
	astFinish,		///< Выключение контроллера
	astCount		///< Кол-во возможных состояний
} ;

/*!
 * \brief Класс обеспечивающий работу приложения
 */
class TApplication {
private:
	appState fAppState { astUnknown } ;	///< Текущее состояние системы
	std::unique_ptr <appError::TAppError> fAppError { new appError::TAppError } ; ///< Указатель на класс обрабатывающий возникающие ошибки

	std::unique_ptr <sensor::TSensorPhoto> fSensorPhoto { new sensor::TSensorPhoto } ;	///< Указатель на класс работы с видеокамерой
//	std::unique_ptr <sensor::TSensorAudio> fSensorAudio { new sensor::TSensorAudio } ;	///< Указатель на класс работы со звуком

public:
	TApplication();
	virtual ~TApplication();

	void checkHW () ;					// Проверка оборудования
	void setGPIO () ;					// Перевод неиспользуемых GPIO в режим цифрового входа с подтяжкой к нулю.
	void setPowerSaving (appState) ;	// Переключение в энергосберегающий режим
	void setState (appState) ;			// установка текущего состояния системы
	bool managerState () ;				// Обработка текущего состояния
};

} /* namespace app */

#endif /* TAPPLICATION_HPP_ */
