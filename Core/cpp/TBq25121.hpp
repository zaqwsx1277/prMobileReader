/*
 * TBq25121.hpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#ifndef CPP_TBQ25121_HPP_
#define CPP_TBQ25121_HPP_

#include <i2c.h>

#include <TUnit.hpp>

namespace unit {

constexpr uint8_t defBq25121Adrr { 0x6A } ;							///< I2C адрес чипа BQ25121
constexpr uint8_t defBq25121AddrRead { (defBq25121Adrr << 1) + 1 } ;///< I2C адрес чипа BQ25121 для чтения
constexpr uint8_t defBq25121AddrWrite (defBq25121Adrr << 1) ;		///< I2C адрес чипа BQ25121 для записи

constexpr uint8_t defBq25121Cmd_00 [] { 0x00, 0x00 } ;
constexpr uint8_t defBq25121Cmd_01 [] { 0x01, 0x00 } ;
constexpr uint8_t defBq25121Cmd_02 [] { 0x02, 0x88 } ;
constexpr uint8_t defBq25121Cmd_03 [] { 0x03, 0x8C } ;
constexpr uint8_t defBq25121Cmd_04 [] { 0x04, 0x26 } ;
constexpr uint8_t defBq25121Cmd_05 [] { 0x05, 0x78 } ;
constexpr uint8_t defBq25121Cmd_06 [] { 0x06, 0xFE } ;
constexpr uint8_t defBq25121Cmd_07 [] { 0x07, 0xD0 } ;
constexpr uint8_t defBq25121Cmd_08 [] { 0x08, 0x48 } ;
constexpr uint8_t defBq25121Cmd_09 [] { 0x09, 0x3A } ;
constexpr uint8_t defBq25121Cmd_0B [] { 0x0B, 0x34 } ;

constexpr uint8_t defBq25121Cmd_reset [] { 0x09, 0x80 } ;

constexpr uint32_t defBq25121TimeOut { 100 } ;					///< Таймаут для работы по шине I2C

/*!
 * Класс для работы с чипом BQ25121
 */
class TBq25121 : public unit::TUnit {

public:
	TBq25121();
	virtual ~TBq25121();

	bool check () ;					///< Проверка работоспособности чипа BQ25121

	void init () ;					///< Инициализация устройства
	void sleep () { ; }				///< Перевод в режим энергосбережения
	void wakeup () { ; } 			///< Выход из режима энергосбережения
	bool process () { return true ; }	///< Получение данных
};

} /* namespace unit */

#endif /* CPP_TBQ25121_HPP_ */
