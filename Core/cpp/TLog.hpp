/*
 * TLog.hpp
 *
 *  Created on: Aug 9, 2021
 *      Author: AAL
 */

#ifndef CPP_TLOG_HPP_
#define CPP_TLOG_HPP_

#include <deque>

#include <stdint.h>
#include "TEnum.hpp"

namespace app {

constexpr uint32_t stLogSize { 0x20000 } ;	///< Размер лога на внутренней флешке

/*!
 * @brief Описание структуры для хранения одной записи лога.
 */
typedef struct {
	uint32_t dateTime ;			///< Дата время события с точностью до двух сек.
	app::appState state ;		///< Состояние события
} tdLogItem ;

/*!
 * @brief Класс ведения лога работы
 * @details Все события пишутся в контейнер std::deque и записываются в последний банк флешки при переходе в состояние StandBy.
 * При начале активности последнее событие пишется в контейнер и если первое и последнее событие == appState::appHwErr, то запись на флешку не производится. Т.о. мы убираем поток ошибок при падении по памяти
 * Что бы не взрывать себе мозги, указатель на пустое место для записи хранится в регистре RTC_BKP_DR1
 * \todo Переделать на запись на SD карточку.
 * \todo Написать класс для работы с встроенной флешкой
 */
class TLog {
	std::deque <tdLogItem> mStateQueue ;			///< Очередь сообщений для записи, первый элемент которой всегда последнее записанное состояние на флешке или appState::appUnknown
	tdLogItem* mPrtLastItem ;						///< Указатель на место для записи лога

	void writeItemToFlash () ;						///< Запись событий на флешку

public:
	TLog();
	virtual ~TLog();

	void pushItem (const app::appState&) ;			///< Запись в лог изменения состояния
	bool writeLog () ;								///< Сохранение лога на флешку и если нужно на SDIO. Всегда записывается последний банк встроенной флешки
};

} /* namespace app */

#endif /* CPP_TLOG_HPP_ */
