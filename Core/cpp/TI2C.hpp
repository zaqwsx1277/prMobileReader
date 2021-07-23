/*
 * TI2C.hpp
 *
 *  Created on: Jul 12, 2021
 *      Author: AAL
 */

#ifndef CPP_TI2C_HPP_
#define CPP_TI2C_HPP_

#include <TUnit.hpp>

namespace unit {
//-----------------------------------------------------
/*!
 * Класс для проверки работы шины i2c. И на хрена я это сделел??? :(
 */
class TI2C: public unit::TUnit {
public:

	TI2C() = default ;
	virtual ~TI2C() = default ;

	bool check () ;		///< Проверка шины i2c

	void init () { ; }				///< Инициализация устройства
	void sleep () { ; }				///< Перевод в режим энергосбережения
	void wakeup () { ; } 			///< Выход из режима энергосбережения
	bool process () { return true ; }			///< Получение данных
};

} /* namespace unit */

#endif /* CPP_TI2C_HPP_ */
