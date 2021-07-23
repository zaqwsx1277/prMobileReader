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

#include "TApplication.hpp"

/*!
 * \defgroup Перечисления
 * @{
 */
/*! @} */

/// Различные общие для всего приложения определения
namespace common {

	extern std::unique_ptr <app::TApplication> app ;	///< Указатель на класс обслуживающий приложение

	extern std::string stTempStr ;						///< Временная строка для корректной отсылки сообщений по UART
	extern std::string stSyncTimeFileName ;				///< Имя файла для синхронизации времени
	extern std::string stTagFileName ;					///< Имя файла для хранения меток
	extern std::string stPhotoFileName ;				///< Имя файла для хранения меток

	extern uint8_t stTagBufId [25] ;					///< Буфер для чтения метки
	extern uint8_t stFileBuf [512] ;					///< Буфер для записи данных в файл. Он здесь, т.к. SDIO работает по DMA
	extern uint8_t stPhotoBuf [1600*94] ;				///< Буфер для сканирования изображения
} /* namespace common */

#endif /* CPP_TCOMMON_HPP_ */
