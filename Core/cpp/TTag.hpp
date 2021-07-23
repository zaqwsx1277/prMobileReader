/*
 * TTag.hpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#ifndef CPP_TTAG_HPP_
#define CPP_TTAG_HPP_

#include <stdint.h>

#include <TUnit.hpp>

namespace unit {

constexpr uint8_t stTagCmdAck [] {0x00,0x00,0xFF,0x00,0xFF,0x00} ;
constexpr uint8_t stTagCmdAckWakeUp [] {0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00} ;
constexpr uint8_t stTagCmdScan [] {0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00} ;
constexpr uint8_t stTagCmdSleep [] {0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x16, 0x10, 0x06, 0x00} ;

constexpr uint32_t stTagUsartTimeOut { 100 } ;		///< Время ожидания отправки сообщений по USART
constexpr uint32_t stTagUsartWaitID { 10000 } ;		///< Время ожидания получения ID метки (10 сек)

constexpr uint32_t stTagWaitSync { 5000 } ;			///< Время ожидания синхронизации

//-------------------------------------------------------
/*!
 * Класс управления чипом PN532
 * Считанная метка записывается в backup регистр RTC_BKP_DR0 и доступна всегда.
 */
class TTag : public TUnit {
private :
public:
	TTag();
	virtual ~TTag();

	bool check () ;			///< Проверка оборудования
	void init () { ; }		///< Инициализация устройства
	void sleep () ;			///< Перевод в режим энергосбережения
	void wakeup () ;		///< Выход из режима энергосбережения
	bool process () ;		///< Чтения метки

	void copyTag (uint8_t *) ; ///< Копирование текущей метки из регистра RTC_BKP_DR0 в uint8_t буфер
};

} /* namespace unit */

#endif /* CPP_TTAG_HPP_ */
