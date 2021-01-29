/*
 * TCommon.hpp
 *
 *  Created on: 12 янв. 2021 г.
 *      Author: energia
 */

#ifndef TCOMMON_HPP_
#define TCOMMON_HPP_

#include "i2c.h"

namespace common {

constexpr uint32_t defFlashAddr = ((uint32_t)0x08000000) ; 	///< 2047 Адрес блока на флешке где храним данные по ошибкам

constexpr uint8_t defPwrAddress = 0x6A ;					///< Адрес чипа управления питание BQ25121A
constexpr uint8_t defPwrAddressRead = (defPwrAddress << 1) + 1 ;
constexpr uint8_t defPwrAddressWrite = (defPwrAddress << 1) ;
constexpr uint32_t defPwrI2cTimeout = 100 ;					///< Время ожидания ответа по I2C чипа управления питание BQ25121A

static I2C_HandleTypeDef *ptrPwrHandle = &hi2c2 ;
static uint8_t stPwrCmdSSM [] { 0x00, 0x00 } ;				///< Команда Status & ShipMode

static I2C_HandleTypeDef *ptrPhotoHandle = &hi2c2 ;

constexpr uint8_t defNfcAddress = 0x24 ;					///< Адрес считывателя NFC метки
constexpr uint8_t defNfcAddressRead = (defNfcAddress << 1) + 1 ;
constexpr uint8_t defNfcAddressWrite = (defNfcAddress << 1) ;
constexpr uint32_t defNfcI2cTimeout = 100 ;					///< Время ожидания ответа по I2C считывателя NFC метки

static I2C_HandleTypeDef *ptrNfcHandle = &hi2c2 ;

//constexpr uint32_t defAudioI2sTimeout = 100 ;				///< Время ожидания ответа по I2S от микрофона
//constexpr I2S_HandleTypeDef *ptrAudioHandle = &hi2s3 ;		///< Указатель на хендл для работы с микрофоном

}



#endif /* TCOMMON_HPP_ */
