/*
 * TFileSystem.hpp
 *
 *  Created on: 7 июл. 2021 г.
 *      Author: AAL
 */

#ifndef CPP_TFILESYSTEM_HPP_
#define CPP_TFILESYSTEM_HPP_

#include <memory>
#include <string>
#include <vector>

#include "fatfs.h"

#include "TUnit.hpp"

struct WAVE_FormatTypeDef
{
  const uint32_t   ChunkID { 0x46464952 } ; /* 0  RIFF */
  uint32_t   FileSize;      				/* 4 */
  const uint32_t   FileFormat { 0x45564157 } ;    /* 8 WAVE */
  const uint32_t   SubChunk1ID { 0x20746D66 } ;   /* 12 fmt*/
  const uint32_t   SubChunk1Size { 0x10 } ;	/* 16  16*/
  const uint16_t   AudioFormat { 0x1 } ;	    /* 20 1 */
  const uint16_t   NbrChannels { 0x1 } ;   	/* 22 1 */
  const uint32_t   SampleRate { 0x1F40 } ;    /* 24 8000 */

  const uint32_t   ByteRate { 0x3E80 } ;      /* 28 1600 */
  const uint16_t   BlockAlign { 0x02 } ;	    /* 32 2 */
  const uint16_t   BitPerSample { 0x10 } ;		/* 34 16 */
  const uint32_t   SubChunk2ID { 0x61746164 } ;   /* 36 data */
  uint32_t   SubChunk2Size; 				/* 40 */

} ;

namespace unit {

/*!
 * Класс для работы с файловой системой на SD-карточке. Он нужен только что-бы записывать туда фотки, аудио и метки.
 * \brief Класс для работы с файловой системой
 * \attention Перед работой с файловой системой всегда необходимо инициальзировать SD карточку
 */
//--------------------------------------------------------
class TFileSystem : public unit::TUnit {
private:
	FIL mFile { 0 } ;				///< Хендл для работы с файлом
	uint32_t mTag ;					///< Последняя считанная метка
	std::string mFileName ;			///< Имя открытого файла

	RTC_TimeTypeDef mTime;			///< Время создания файла
	RTC_DateTypeDef mDate;			///< Дата создания файла

public:
	TFileSystem() ;
	~TFileSystem() ;

	bool check () ;					///< Проверка работоспособности SD-карточки
	void init () { ; }				///< Инициализация устройства
	void sleep () { ; }				///< Перевод в режим энергосбережения
	void wakeup () { ; } 			///< Выход из режима энергосбережения
	bool process () { return true ; } ///< Получение данных

	void getTime (const uint32_t) ;	///< Синхронизация времени
	void setTag () ;				///< Запись на SD ID сосканированной метки
	bool writePhoto () ; 			///< Запись на SD изображения полученного с камеры
	bool writeAudio (const uint8_t*, const uint32_t) ;	///< Запись аудио блока на SD
	void closeAudio () ; 			///< Запись размера в хидер аудио файла и завершение его записи
	bool openFileName (const std::string&) ; ///< Формирование имени файла для записи звука и фото
};
//--------------------------------------------------------
} /* namespace unit */

#endif /* CPP_TFILESYSTEM_HPP_ */
