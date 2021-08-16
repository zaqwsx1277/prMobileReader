/*
 * TPhoto.hpp
 *
 *  Created on: Jul 8, 2021
 *      Author: AAL
 */

#ifndef CPP_TPHOTO_HPP_
#define CPP_TPHOTO_HPP_

#include "ov2640.h"
#include "TUnit.hpp"

namespace unit {

constexpr uint32_t stPhotoTimeWait { 5000 } ;	///< Время ожидания отпускания кнопки после
constexpr uint16_t stPhotoI2CWrite { 0x60 } ;	///< Адрес для записи на шине I2C
constexpr uint16_t stPhotoI2CRead { stPhotoI2CWrite | 0x01 } ;	///< Адрес для чтения на шине I2C
constexpr uint32_t stPhotoI2CTimeout { 100 } ;	///< Таймаут при работе по шине I2C
constexpr uint32_t stPhotoDCMITimeout { 10000 } ;///< Таймаут получения изображения
constexpr uint32_t stPhotoPositionTimeout { 3000 } ; ///< Таймаут для выбора объекта фотографирования

/*!
 * Класс работы с камерой OV2640
 * \todo Убрать ov2640.c и передалать на плюсы (Хотя не очень понятно зачем???)
 * \todo Убрать все Warning в ov2640.c
 */
class TPhoto : public TUnit {
public:
	TPhoto();
	virtual ~TPhoto();

	bool check () ;			///< Проверка оборудования

	void init () ;			///< Инициализация устройства
	void sleep () ;			///< Перевод в режим энергосбережения
	void wakeup () ;		///< Выход из режима энергосбережения
	bool process () ;		///< Получение изображения с камеры и запись его в файл
};

} /* namespace unit */

#endif /* CPP_TPHOTO_HPP_ */
