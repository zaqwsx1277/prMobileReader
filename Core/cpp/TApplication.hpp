/*
 * TApplication.hpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */

#ifndef CPP_TAPPLICATION_HPP_
#define CPP_TAPPLICATION_HPP_

#include <stm32f4xx_hal.h>
#include <gpio.h>

#include <string>
//#include <array>
#include <memory>
#include <unordered_map>

#include "TButton.hpp"
#include "TFileSystem.hpp"
#include "TTag.hpp"
#include "TPhoto.hpp"
#include "TAudio.hpp"
#include "TBq25121.hpp"
#include "TI2C.hpp"
#include "TSdio.hpp"

/// Всё, что касается управления приложением
namespace app {

constexpr uint32_t stAppDebugTimeout { 5000 } ; ///< Время ожидания отправки отладочного сообщения

/*!
 * \ingroup Перечисления
 * \brief Возможные состояния приложения
 * \todo Найти оптимальный размер диска для работы с SD карточкой и разобраться с работой SDIO по DMA
 * \todo Убрать FatFS и сделать свою работу с карточкой
*/
enum appState {
	appUnknown = 0,		//!< Пиздец котёнку. :(
	appStarted,			///< Начальный запуск. Приложение
	appStandBy,			///< Переход в режим StandBy
	appReady,			///< Приложение восстановилось из режима энергосбережения
	appAudio,			///< Нажата кнопка записи звука
	appAudioWaitStop,	///< Режим предотвращения случайного отпускания кнопки записи звука
	appAudioStop,		///< режим окончания записи звука
	appPhoto,			///< Нажата кнопка фото
	appPhotoButtonPress,///< Длительное нажатие кнопки сканирования фото
	appPhotoI2CErr,		///< Ошибка по шине I2C видеокамеры
	appPhotoTimeout,	///< Превышен таймаут получения изображения
	appTag,				///< Нажата кнопка сканирования метки
	appTagErr,			///< Ошибка при работе со сканером меток
	appDoc,				///< Устройство находится на докстанции
	appDocWaitStop,		///< Режим предотвращения кратковременное снятие с докстанции (пока не реализован т.к. )
	appCheckBounce,		///< Устранение дребезга контактов
	appCheckButton,		///< Проверяем нажатые кнопки
	appBounceTimeout,	///< За 256 мСек дребезг не устранился.
	appStartBounce,		///< Запуск устранения дребезга контактов
	appErrButton,		///< Ошибка нажатий кнопок
	appErrI2C,			///< Ошибка по шине I2C
	appErrBq25121,		///< Ошибка при работе с чипом BQ25121
	appErrSDIO,			///< Ошибка SD-карточки
	appErrFileFS,		///< Ошибка файловой системы
	appDocSyncTime,		///< Синхронизация времени после установки на докстанцию
	appTagNoId,			///< Чтение метки выполнено с ошибкой (например, тупо не сосканировали)
	appNum				///< Кол-во возможных состояний
};

/*!
 * \ingroup Сообщения
 * Сообщения выдаваемые при переходе в режим перечисленный в appState
 */
const std::unordered_map <appState, std::string> msgAppState {
	{ appUnknown, "Unknown" },
	{ appStarted, "Start application ver: A20.01 - 31.07.2021" },
	{ appStandBy, "StandBy mode" },
	{ appReady, "Recovery from mode StandBy" },
	{ appAudio, "Audio mode" },
	{ appAudioWaitStop, "Waiting for sound recording stop" },
	{ appAudioStop, "Stop sound recording" },
	{ appPhoto, "Photo mode" },
	{ appPhotoButtonPress, "Photo mode. Long press of the button" },
	{ appPhotoI2CErr, "Photo mode. I2C error" },
	{ appPhotoTimeout, "Photo mode. DCMI timeout" },
	{ appTag, "Tag mode" },
	{ appTagErr, "PN532 error" },
	{ appDoc, "DocStation mode" },
	{ appDocWaitStop, "Waiting for DocStation mode stop" },
	{ appCheckBounce, "Debouncing contacts mode" },
	{ appCheckButton, "Check button mode" },
	{ appBounceTimeout, "Timeout debouncing contacts" },
	{ appStartBounce, "Start debouncing contacts" },
	{ appErrButton, "Button pressing error" },
	{ appErrI2C, "Bus I2C error" },
	{ appErrBq25121, "BQ25121 error" },
	{ appErrSDIO, "SD-card error" },
	{ appErrFileFS, "File System error" },
	{ appTagNoId, "No TAG ID" },
	{ appDocSyncTime, "Time synchronization mode" }
} ;

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
 * @attention Т.к. я пока понятия не имея как работать со временем то раз в 50 дней устройство нужно перезапускать (хотя у нас есть режим StandBy, а это считай перезапуск)
 * \attention Т.к. основной режим контроллера StandBy, то все указатели всегда освободятся при переходе в этот режим и выключать устройства нужно в деструкторе классов.
 */
class TApplication {
private:
	app::appState  mAppState { appUnknown } ;				///< Текущее состояние приложения
	uint32_t mAppStateChange ; 								///< Время последнего изменения состояния в миллисек.
	uint8_t mBounceCount { 0 } ; 							///< Счётчик циклов для устранения дребезга контактов. Если состояние приложения appCheckBounce и он равен 0, то завершаем приложение по таймоуту
	std::array <TButton, idButton::btnNum> mButton { defGpioPhoto, defGpioAudio, defGpioTag, defGpioDoc }; ///< Массив содержащий описание всех кнопок

	std::unique_ptr <unit::TFileSystem> mFileSystem { nullptr } ; ///< Указатель на класс работы с файловой системой.
	std::unique_ptr <unit::TTag> mTag { new unit::TTag } ;	///< Указатель на класс работы со сканером меток
	std::unique_ptr <unit::TPhoto> mPhoto { new unit::TPhoto } ;  ///< Указатель на класс работы c камерой
	std::unique_ptr <unit::TAudio> mAudio { nullptr } ; 	///< Указатель на класс работы c микрофоном
	std::unique_ptr <unit::TBq25121> mBq25121 { new unit::TBq25121 } ; ///< Указатель на класс работы c микрофоном
	std::unique_ptr <unit::TI2C> mI2c { new unit::TI2C } ;	///< Указатель на класс работы c шиной i2c
	std::unique_ptr <unit::TSdio> mSdio { nullptr } ; 		///< Указатель на класс работы с SD картой

	void makeInfoGpio (const app::typeInfo, const GPIO_PinState) ;	///< Метод включения/выключения пищалка/светодиода
	void startBounce () ;						///< Запуск устранения дребезга контактов

public:
	TApplication();								///< Инициализация работы приложения
	virtual ~TApplication() ;			///< По большому счёту он здесь на хрен не нужен, т.к. он отродясь не будет выполняться

	void checkUnits () ;						///< Тестирование устройств
	void checkBounce () ;						///< Устранение дребезга контактов и проверка сработавшей кнопки
	void stateManager () ;						///< Менеджер обработки состояний
	bool setState (app::appState) ;				///< Установка состояния приложения
	std::pair <app::appState, uint32_t> getState () ;	///< Получение состояния приложения
	std::string getMessageTime () ;				///< Получить текстовое сообщение текущего времени
	void makeInfo (const app::typeInfo, const app::typeSound, const uint32_t) ;	///< Управление информационными сигналом
	void debugMesage (const std::string &) ;	///< Отправка текстового отладочного сообщения
	void debugMesage (const char *, const std::size_t) ; ///< Отправка отладочного сообщения
	void debugMesage (const uint8_t *, const std::size_t) ; ///< Отправка цифр
	void debugMesage (const appState) ; 		///< Отправка отладочного сообщения для указанного состояния
	void debugMesage () ;				 		///< Отправка отладочного сообщения для текущего состояния

	void writePhoto () ;						///< Записываем фотку на SD'шку
};

} /* namespace app */

#endif /* CPP_TAPPLICATION_HPP_ */
