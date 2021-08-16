/*
 * TAudio.hpp
 *
 *  Created on: Jul 7, 2021
 *      Author: AAL
 */

#ifndef CPP_TAUDIO_HPP_
#define CPP_TAUDIO_HPP_

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

#include "TUnit.hpp"
#include "TFileSystem.hpp"

#include <pdm2pcm.h>

typedef uint16_t tdAudioFrame ;							///< Тип аудио фрейма. Т.к. у нас моно микрофон, то и размер фрейма 16 бит

namespace unit {

constexpr uint32_t stAudioDuration { 10000 } ;			///< Максимальная длительность записи аудио
constexpr uint32_t stAudioBitrate { I2S_AUDIOFREQ_8K } ;///< Битрейт получаемый с микрофона
constexpr uint32_t stAudioBufSize { 384 } ;				///> Размер буфера для получения звука с микрофона
constexpr uint32_t stAudioBufOutSize { 256 } ;			///> Размер бувера для записи звука
constexpr uint32_t stAudioWaitStart { 300 } ;			///> Задержка начала записи для устранения посторонних звуков

/*!
 * Заголовок аудио-файла фомата .WAV
 */


struct Audio_BufferTypeDef {
  int32_t offset;
  uint32_t fptr;
} ;

/*!
 * \ingroup Перечисления
 * \brief Индекс обрабатываемоно аудио буфера
*/
enum crAudioBufID {
	crStop = 0xFF,		///< Обработка успешно завершена или вообще не начиналась
	crFirst = 0x00,		///< Обрабатывается первый буфер (первая половина)
	crSecond = 0x01		///< Обрабатывается второй буфер (вторая половина)
};

/*!---------------------------------------------------------------------------------
 * Класс для работы со звуком
 * Запись звука идёт в буфер по DMA и по срабатываний прерываний выполняется преобразование из PDM в PCM запись результата на SD
 */
class TAudio : public TUnit {
private:
	uint32_t mTimeStart { 0 } ;						///< Время запуска записи. Нужно для удаления начальных 100 мСек

	std::shared_ptr<TFileSystem> mPtrFileSystem ;	///< Указатель на класс TFileSystem

	std::vector <tdAudioFrame> mBufToFile ; 		///< Буфер для записи на диск

	bool makeHeader () ;							///< Формирование заголовка .WAV файла
	bool writeBuf (tdAudioFrame *) ; 				///< Формирование и запись буфера на SDIO

public:
	TAudio (std::shared_ptr<TFileSystem>) ;
	virtual ~TAudio();

	bool check () ;			///< Проверка работоспособности аудио (запись и вопроизведение пищалки)

	void init () { ; }		///< Инициализация устройства
	void sleep () { ; }		///< Перевод в режим энергосбережения
	void wakeup () { ; }	///< Выход из режима энергосбережения
	bool process () ;		///< Получение данных с микрофона и запись его в файл
};

} /* namespace unit */

#endif /* CPP_TAUDIO_HPP_ */
