/*
 * TCommon.hpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */

#ifndef CPP_TCOMMON_HPP_
#define CPP_TCOMMON_HPP_

#include <string>
#include <memory>
#include <stdint.h>

#include "TApplication.hpp"
#include "TEnum.hpp"

/*!
 * \defgroup Перечисления
 * @{
 */
/*! @} */

extern DMA_HandleTypeDef hdma_dcmi;

extern bool tmpFlag ;
extern  uint32_t tmpCount ;
extern uint8_t stPhotoBuf [1600*80] ;				///< Буфер для сканирования изображения

/// Различные общие для всего приложения определения
namespace common {
	extern std::unique_ptr <app::TApplication> app ;	///< Указатель на класс обслуживающий приложение

	extern std::string stTempStr ;						///< Временная строка для корректной отсылки сообщений по UART
	extern std::string stSyncTimeFileName ;				///< Имя файла для синхронизации времени
	extern std::string stTagFileName ;					///< Имя файла для хранения списка отсканированных меток
	extern std::string stPhotoFileName ;				///< Имя файла для хранения списка фоток
	extern std::string stAudioFileName ;				///< Имя файла для хранения списка аудио

	extern uint8_t stTagBufId [25] ;					///< Буфер для чтения метки
	extern uint8_t stFileBuf [512] ;					///< Буфер для записи данных в файл. Он здесь, т.к. SDIO работает по DMA
//	extern uint8_t stPhotoBuf [1600*40] ;				///< Буфер для сканирования изображения

	extern tdAudioFrame stAudioBuf [2][unit::stAudioBufSize] ; ///< Буфер для записи данных с микрофона.
	extern unit::crAudioBufID stAudioBufId ;			///< Индекс обрабатываемого буфера
	extern uint32_t stTimeStartDebugMessage ;			///< Вычисление таймаута при отправке отладочного сообщения по UART


	const extern uint32_t *stPrtSN ;					///< Адрес серийного номера кристала


} /* namespace common */

#endif /* CPP_TCOMMON_HPP_ */
