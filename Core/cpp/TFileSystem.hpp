/*
 * TFileSystem.hpp
 *
 *  Created on: 7 июл. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_TFILESYSTEM_HPP_
#define CPP_TFILESYSTEM_HPP_

#include <memory>

#include "fatfs.h"

#include "TUnit.hpp"

namespace unit {

/*!
 * Класс для работы с файловой системой на SD-карточке. Он нужен только что-бы записывать туда фотки, аудио и метки.
 * \brief Класс для работы с файловой системой
 * \attention Перед работой с файловой системой всегда необходимо инициальзировать SD карточку
 */
//--------------------------------------------------------
class TFileSystem : public unit::TUnit {

public:
	TFileSystem();
	virtual ~TFileSystem();

	bool check () ;					///< Проверка работоспособности SD-карточки
	void init () { ; }				///< Инициализация устройства
	void sleep () { ; }				///< Перевод в режим энергосбережения
	void wakeup () { ; } 			///< Выход из режима энергосбережения
	bool process () { return true ; } ///< Получение данных

	void getTime () ;				///< Синхронизация времени
	void setTag () ;				///< Запись на SD ID сосканированной метки
	bool writePhoto () ; 			///< Запись на SD изображения полученного с камеры
};
//--------------------------------------------------------
} /* namespace unit */

#endif /* CPP_TFILESYSTEM_HPP_ */
