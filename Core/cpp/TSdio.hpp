/*
 * TSdio.hpp
 *
 *  Created on: 15 июл. 2021 г.
 *      Author: energia
 */

#ifndef CPP_TSDIO_HPP_
#define CPP_TSDIO_HPP_

#include <TUnit.hpp>

namespace unit {

class TSdio: public TUnit {
public:
	TSdio() ;
	virtual ~TSdio() ;

	bool check () ;			///< Проверка работоспособности SDIO

	void init () { ; }				///< Инициализация устройства
	void sleep () { ; }				///< Перевод в режим энергосбережения
	void wakeup () { ; } 			///< Выход из режима энергосбережения
	bool process () { return true ; } ///< Получение данных
};

} /* namespace unit */

#endif /* CPP_TSDIO_HPP_ */
