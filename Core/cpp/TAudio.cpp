/*
 * TAudio.cpp
 *
 *  Created on: Jul 7, 2021
 *      Author: AAL
 */
#include <string>
#include <sstream>
#include <ios>
#include <iomanip>

#include <TAudio.hpp>
#include <TCommon.hpp>

#include <i2s.h>

namespace unit {

//--------------------------------------------------------
/*!
 * Конструктор инициализирующий работу для записи аудио.
 * Все данные для PDM -> PCM преобразования задаются в настройках CubeMX
 * @param inPtrFileSystem Указатель на класс работы с файловой системой
 * @attention Я ни коем образом не проверяю указатель на файловую систему!!!  sizeof (common::stAudioBuf)/2
 */
TAudio::TAudio(std::shared_ptr<TFileSystem> inPtrFileSystem)
{
	mPtrFileSystem = inPtrFileSystem ;
	if (mPtrFileSystem -> openFileName (".wav") == false) common::app -> setState(app::appState::appAudioErr) ;
	  else {
		makeHeader () ;
		if (HAL_I2S_Receive_DMA(&hi2s3, (uint16_t *) common::stAudioBuf, sizeof (common::stAudioBuf)/sizeof(tdAudioFrame) ) != HAL_OK)	common::app -> setState(app::appState::appAudioErr) ;
		  else {
			mBufToFile.reserve (stAudioBufOutSize) ;
			common::app -> debugMessage("Write Audio start - OK") ;
		  }
	  }
}
//--------------------------------------------------------
/*!
 * @attention По хорошему нужно проверять и дожидаться остановки DMA, т.к. в противном случае контроллер не уйдёт в StandBy
 */
TAudio::~TAudio()
{
	mPtrFileSystem -> closeAudio () ;

	common::stAudioBufId = unit::crAudioBufID::crStop ;
	HAL_I2S_DMAStop(&hi2s3);
	common::app -> debugMessage("Write Audio stop") ;
}
//--------------------------------------------------------
bool TAudio::check ()
{
	return true ;
}
/*!-------------------------------------------------------
 * Запись буфера полученного с микрофона
 * @return	true в случае успешной записи
 * @todo Переделать на хрен, т.к. написано через жопу кривыми руками. :(
 */
bool TAudio::process ()
{
	bool retValue { false } ;
	uint16_t tmpBuf [48] { 0 } ; 	// Буфер для PCM данных
	uint16_t xxxTmp [stAudioBufSize] ;

	switch (common::stAudioBufId) {
	  case crFirst:
		for (uint32_t i = 0; i < stAudioBufSize; ++i) xxxTmp [i] = common::stAudioBuf [0][i] ;
		PDM_Filter (xxxTmp, &tmpBuf [0], &PDM1_filter_handler) ;
		retValue = writeBuf(tmpBuf) ;
		common::stAudioBufId = unit::crAudioBufID::crStop ;
	  break;

	  case crSecond:
		for (uint32_t i = 0; i < stAudioBufSize; ++i) xxxTmp [i] = common::stAudioBuf [1][i] ;
		PDM_Filter (xxxTmp, &tmpBuf [0], &PDM1_filter_handler) ;
		retValue = writeBuf(tmpBuf) ;
		common::stAudioBufId = unit::crAudioBufID::crStop ;
	  break;

	  default:
		retValue = true ;
	  break;
	}


	return retValue ;
}
//---------------------------------------------------------
bool TAudio::makeHeader ()
{
	WAVE_FormatTypeDef header ;

	return mPtrFileSystem -> writeAudio ((uint8_t *)&header, sizeof (header)) ;
}
/*!-----------------------------------------------------------
 * Добавление в буфер данных и при заполнении 512 сэмплов запись их на SD
 * Первые 300 мСек записи удаляются, т.к. первоначально сыплется какая-то ересть
 * @param inBuf указатель на буфер преобразованных сэмплов (16 сэмплов)
 * @return true если запись выполнирась
 * @todo Переделать нахрен что бы PDM to PCM выполнялось тут
 */
bool TAudio::writeBuf (tdAudioFrame *inBuf)
{
	bool retVal { true } ;

	if ((HAL_GetTick() - mTimeStart) > stAudioWaitStart) {
		for (uint32_t i = 0; i < 48; ++i) {
			mBufToFile.push_back (inBuf [i]) ;
		}
		if (mBufToFile.size() >= stAudioBufOutSize) {
			retVal = mPtrFileSystem -> writeAudio ((uint8_t *) mBufToFile.data(), (mBufToFile.size() * sizeof (tdAudioFrame))) ;
			mBufToFile.clear() ;

			if (retVal) common::app -> debugMessage ("Write - OK") ;
			  else common::app -> debugMessage ("Write - Fail!!!") ;
		}
	}
	return retVal ;
}
//---------------------------------------------------------
} /* namespace app */
