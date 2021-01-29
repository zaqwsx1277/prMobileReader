/*
 * TError.hpp
 *
 *  Created on: Jan 11, 2021
 *      Author: energia
 */

#ifndef TERROR_HPP_
#define TERROR_HPP_

#include <array>

#include "rtc.h"

namespace appError {

/*!
 * \brief Описание ошибки необходимое для учёта.
 * \details Все ошибки записываются во флешку.
 * Частота записи определяется в настройках и происходит либо раз в определённый промежуток, либо при возникновении ошибки нового типа.
 * Это сделано для того, что бы минимизировать количество записей во флешку
 */
struct TError {
	uint32_t fCount {0} ; 		///< Кол-во ошибок
	RTC_TimeTypeDef fTime {0};	///< Время возникновения последней ошибки
	RTC_DateTypeDef fDate {0};  ///< Дата возникновения последней ошибки
	bool isWrite {false} ;		///< Флаг записи информации об ошибке на внутренюю
};

/*!
 * \brief Типы возможных ошибок
 */
enum typeError {
	tePower = 0,//!< тип ошибки по питанию (BQ25121A)
	tePhoto, 	//!< Тип ошибки обработки фото
	teNfc,		//!<
	teAudio,	//!< Тип ошибки обработки аудио
	teHw,		//!< Ошибка оборудования
	teSw,		//!< Ошибка ПО
	teRTC,		//!< Неправильное время в RTC (нулевой год)
	teCount 	//!< кол-во типов ошибок
};

class TAppError {
private:
	std::array<TError, teCount> fError ; ///< Массив содержащий кол-во всех ошибок
public:
	TAppError();
	virtual ~TAppError();

	void read () ;						// Чтение кол-ва ошибок из флешки
	void write () ;						// Записи кол-ва ошибок на флешку
	void setError (const typeError)  ;	// Увеличение кол-ва ошибок указанного типа
	void getError (const typeError) const ; // Получение кол-ва ошибок указанного типа
};

} /* namespace appError */

#endif /* TERROR_HPP_ */
