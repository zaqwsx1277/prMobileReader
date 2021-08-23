/*
 * TEnum.hpp
 *
 *  Created on: 10 авг. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_TENUM_HPP_
#define CPP_TENUM_HPP_

#include <unordered_map>

namespace app {

/*!
 * \ingroup Перечисления
 * \brief Возможные состояния приложения
 * \todo Найти оптимальный размер диска для работы с SD карточкой и разобраться с работой SDIO по DMA
 * \todo Убрать FatFS и сделать свою работу с карточкой
 * \todo Реализовать режим сервисного обслуживания (appService)
 * \todo Реализовать режим восстановления файловой системы
*/
enum appState {
	appUnknown = 0,		//!< Непонятной породы зверь
	appService,			///< Режим сервисного обслуживания
	appStarted,			///< Начальный запуск. Приложение
	appStandBy,			///< Переход в режим StandBy
	appReady,			///< Приложение восстановилось из режима энергосбережения
	appAudio,			///< Нажата кнопка записи звука
	appAudioWaitStop,	///< Режим предотвращения случайного отпускания кнопки записи звука
	appAudioStop,		///< режим окончания записи звука
	appAudioErr,		///< Ошибка при записи звука
	appPhoto,			///< Нажата кнопка фото
	appPhotoButtonPress,///< Длительное нажатие кнопки сканирования фото
	appPhotoI2CErr,		///< Ошибка по шине I2C видеокамеры
	appPhotoTimeout,	///< Превышен таймаут получения изображения
	appTag,				///< Нажата кнопка сканирования метки
	appTagErr,			///< Ошибка при работе со сканером меток
	appDoc,				///< Устройство находится на докстанции
	appDocRemove,		///< Устройство снято с докстанции
	appDocStupid,		///< Устройство снято и установлено на докстанцию более 10 раз за 30 сек
	appDocWaitStop,		///< Режим предотвращения кратковременное снятие с докстанции (пока не реализован)
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
	appFsRepair,		///< Режим восстановления файловой системы
	appHwErr,			///< Ошибка падения по железу
	appNum				///< Кол-во возможных состояний
}  ;

/*!
 * Сообщения выдаваемые при переходе в режим перечисленный в appState
 */
const std::unordered_map <appState, std::string> msgAppState {
	{ appUnknown, "Unknown" },
	{ appService, "Service mode" },
	{ appStarted, "Start application ver: A21.08 - 31.08.2021" },
	{ appStandBy, "Move StandBy mode" },
	{ appReady, "Recovery from mode StandBy. ver: A21.08 - 31.08.2021" },
	{ appAudio, "Audio mode" },
	{ appAudioWaitStop, "Waiting for sound recording stop" },
	{ appAudioStop, "Stop sound recording" },
	{ appAudioErr, "Audio mode. ERR!!!" },
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
	{ appFsRepair, "File System repair mode" },
	{ appDocSyncTime, "Time synchronization mode" },
	{ appDocStupid, "Protected from Stupid mode" }
} ;

} /* namespace app */

#endif /* CPP_TENUM_HPP_ */
