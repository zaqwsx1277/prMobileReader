/*
 * TApplication.hpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */

#ifndef CPP_TAPPLICATION_HPP_
#define CPP_TAPPLICATION_HPP_

#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <gpio.h>

#include <string>
#include <memory>

#include "TButton.hpp"
#include "TFileSystem.hpp"
#include "TTag.hpp"
#include "TPhoto.hpp"
#include "TAudio.hpp"
#include "TBq25121.hpp"
#include "TI2C.hpp"
#include "TSdio.hpp"
#include "TLog.hpp"

/// Всё, что касается управления приложением
namespace app {

constexpr uint32_t stAppDebugTimeout { 5000 } ; ///< Время ожидания отправки отладочного сообщения
constexpr uint32_t stAppStupidCount { 5 } ;  	///< Количество установки/снятия устройства на докстанцию для срабатывния предупреждения
constexpr uint32_t stAppStupidTimeout { 10000 } ;///< Время звучания предупредительного сигнала о частой установке/снятии устройства на докстанцию

/*!
 * \ingroup Перечисления
 * \brief Возможные состояния приложения
*/
enum typeSound {
	tsndShort,			///< Короткий сигнал. Длительность звучания 30 мсек. с периодом 150 мсек
	tsndLong,			///< Длинный сигнал. Длительность звучания 500 мсек. с периодом 100 мсек
	tsndContinue,		///< Непрерывный
	tsndNo,				///< Выключение всех сигналов
	tsndNum				///< Кол-во возможных типов сигнала
};

extern TButton defGpioPhoto ;		///< Класс для работы с кнопкой Photo
extern TButton defGpioTag   ;		///< Класс для работы с кнопкой Сканирования метки
extern TButton defGpioAudio ;		///< Класс для работы с кнопкой Записи звука
extern TButton defGpioDoc   ;		///< Класс для работы с кнопкой Определения установки на докстанцию

/*!
 * \ingroup Перечисления
 * \brief Индексы обрабатываемых кнопок
 * @attention Менять последовательность портов нельзя!!!
*/
enum idButton {
	btnPhoto = 0,		///< Фото PA0
	btnAudio, 			///< Аудио PE3
	btnTag,				///< Считывание метки PB12
	btnDoc,				///< Флаг установки на док станцию
	btnNum
};

/*!
 * \ingroup Перечисления
 * \brief Тип информации
*/
enum typeInfo {
	infoAudio = 0,		///< Только пищалка
	infoLight, 			///< Только светодиод
	infoAudioLight,		///< И пищалка и светодиод
	infoNum
};
/*!
 * \brief Класс содержащий данные для обеспечения работы приложения
 * @attention Т.к. я пока понятия не имею как работать со временем то раз в 50 дней устройство нужно перезапускать (хотя у нас есть режим StandBy, а это считай перезапуск)
 * \attention Т.к. основной режим контроллера StandBy, то все указатели всегда освободятся при переходе в этот режим и выключать устройства нужно в деструкторе классов.
 * \attention Часы реального времени ни когда не перепрограммируются, ни при выходе из StandBy, ни при перезапуске кристала
 * \todo Сделать ведения лога работы блока и чтение его сервис-инженером
 * \todo Приделать watchdog
 */
class TApplication {
private:
	app::appState  mAppState { appUnknown } ;				///< Текущее состояние приложения
	uint32_t mAppStateChange ; 								///< Время последнего изменения состояния в миллисек.
	uint8_t mBounceCount { 0 } ; 							///< Счётчик циклов для устранения дребезга контактов. Если состояние приложения appCheckBounce и он равен 0, то завершаем приложение по таймоуту
	std::array <TButton, idButton::btnNum> mButton { defGpioPhoto, defGpioAudio, defGpioTag, defGpioDoc }; ///< Массив содержащий описание всех кнопок

	std::shared_ptr <unit::TFileSystem> mFileSystem { nullptr } ; ///< Указатель на класс работы с файловой системой.
	std::unique_ptr <unit::TTag> mTag { nullptr } ;	///< Указатель на класс работы со сканером меток
	std::unique_ptr <unit::TPhoto> mPhoto { nullptr } ;  ///< Указатель на класс работы c камерой
	std::unique_ptr <unit::TAudio> mAudio { nullptr } ; 	///< Указатель на класс работы c микрофоном
	std::unique_ptr <unit::TBq25121> mBq25121 { new unit::TBq25121 } ; ///< Указатель на класс работы c микрофоном
	std::unique_ptr <unit::TI2C> mI2c { new unit::TI2C } ;	///< Указатель на класс работы c шиной i2c
	std::unique_ptr <unit::TSdio> mSdio { nullptr } ; 		///< Указатель на класс работы с SD картой
	std::unique_ptr <app::TLog> mLog { new app::TLog } ;	///< Указатель на класс ведения лога

	void makeInfoGpio (const app::typeInfo, const GPIO_PinState) ;	///< Метод включения/выключения пищалка/светодиод
	void startBounce () ;						///< Запуск устранения дребезга контактов

	uint32_t mStartDocMode ;					///< Время перехода в режим докстанции. Нужно для корректной синхронизации времени при

public:
	TApplication();								///< Инициализация работы приложения
	virtual ~TApplication() ;					///< По большому счёту он здесь на хрен не нужен, т.к. он отродясь не будет выполняться

	void sleep () ;								///< Перевод контроллера в режим StandBy
	void checkUnits () ;						///< Тестирование устройств
	void checkBounce () ;						///< Устранение дребезга контактов и проверка сработавшей кнопки
	void stateManager () ;						///< Менеджер обработки состояний
	bool setState (app::appState) ;				///< Установка состояния приложения
	std::pair <app::appState, uint32_t> getState () ;	///< Получение состояния приложения
	std::string getMessageTime () ;				///< Получить текстовое сообщение текущего времени
	void makeInfo (const app::typeInfo, const app::typeSound, const uint32_t in = 0) ;	///< Управление информационными сигналом
	void debugMessage (const std::string &) ;	///< Отправка текстового отладочного сообщения
	void debugMessage (const char *, const std::size_t) ; ///< Отправка отладочного сообщения
	void debugMessage (const uint8_t *, const std::size_t) ; ///< Отправка цифр
//	void debugMessageDec (const uint8_t *, const std::size_t) ; ///< Отправка цифр в десятичном виде
//	void debugMessageHex (const uint8_t *, const std::size_t) ; ///< Отправка цифр в десятичном виде
	void debugMessage (const appState) ; 		///< Отправка отладочного сообщения для указанного состояния
	void debugMessage () ;				 		///< Отправка отладочного сообщения для текущего состояния

	void writePhoto () ;						///< Записываем фотку на SD'шку
	void writeAudio () ;						///< Запись звука на SD'шку

	void writeLog () ;							///< Записывем лог на флешку

	void clearStupid () ;						///< Очистка флага контроля установки/снятия с док станции
};



} /* namespace app */

#endif /* CPP_TAPPLICATION_HPP_ */
