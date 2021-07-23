/*
 * TUnit.hpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#ifndef CPP_TUNIT_HPP_
#define CPP_TUNIT_HPP_

/// Всё, что касается работы управления внешними чипами
namespace unit {

/*!
 * Базовый класс для работы с периферийным оборудованием
 */
class TUnit {
public:
	TUnit() = default ;
	virtual ~TUnit() = default ;

	virtual bool check () = 0 ;			///< Проверка оборудования устройства
	virtual void init () = 0 ;			///< Инициализация устройства
	virtual void sleep () = 0 ;			///< Перевод в режим энергосбережения
	virtual void wakeup () = 0 ;		///< Выход из режима энергосбережения
	virtual bool process () = 0 ;		///< Получение данных
};

} /* namespace unit */

#endif /* CPP_TUNIT_HPP_ */
