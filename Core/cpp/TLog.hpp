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

/*!
 * @brief Описание структуры для хранения одной записи лога.
 * @attention Т.к. лог хранится во встроенной флешке, то тип данных uint32_t
 */
typedef struct {
	uint32_t dateTime ;			///< Дата время события с точностью до двух сек.
	app::appState state ;		///< Состояние события
} tdLogItem ;

constexpr uint32_t stLogPtrStart { 0x081E0000 } ;///< Адрес начала лога на флешке. FLASH_SECTOR_23
constexpr uint32_t stLogSize { 0x20000 } ;		///< Размер лога на внутренней флешке
constexpr uint32_t stLogPtrEnd { stLogPtrStart + stLogSize } ;///< Адрес конца лога на флешке

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
	tdLogItem* mPrtLastItem ;						///< Указатель на место для записи лога. На хрена я его испольхую, если тоже самое записано в RTC_BKP_DR1!!!

	void writeItemToFlash () ;						///< Запись событий на флешку

public:
	TLog();
	virtual ~TLog();

	void pushItem (const app::appState&) ;			///< Запись в лог изменения состояния
	bool writeLog () ;								///< Сохранение лога на флешку и если нужно на SDIO. Всегда записывается последний банк встроенной флешки
	tdLogItem getLastItem () ;						///< Получение из лога последнего состояния
//	void clearLog
};

} /* namespace app */

#endif /* CPP_TLOG_HPP_ */
