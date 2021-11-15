/*
 * TFileSystem.cpp
 *
 *  Created on: 7 июл. 2021 г.
 *      Author: AAL
 */
#include <string>
#include <sstream>
#include <ios>
#include <iomanip>
#include <time.h>

#include <fatfs.h>
#include <rtc.h>

#include "TFileSystem.hpp"
#include "TCommon.hpp"
#include "TApplication.hpp"

namespace unit {
//---------------------------------------------------------
/*!
 * Монтируем файловую систему.
 * \attention SD карта должна быть уже проинициализированна
 */
TFileSystem::TFileSystem() {
	if (f_mount(&SDFatFS, (TCHAR const*)SDPath, 1) != FR_OK) common::app -> setState(app::appState::appErrFileFS) ;
	  else common::app -> debugMessage ("File system mount - OK.") ;
}
//---------------------------------------------------------
/*!
 * \attention Файловая система должна быть размонтирована всегда, в противном случае SD'шка убивается
 */
TFileSystem::~TFileSystem() {
	f_close (&mFile) ; 			// На всякий случай закрываем файл, что бы не упала FS
	f_mount(NULL, (TCHAR const*)SDPath, 1) ;
	common::app -> debugMessage ("File system unmount.") ;
}
//---------------------------------------------------------
/*!
 * \todo Приделать проверку записи и чтения тестового файла
 * @return true если провека выполнилась
 */
bool TFileSystem::check () {
	bool retValue { false } ;

	if (f_mount(&SDFatFS, (TCHAR const*)SDPath, 1) == FR_OK) {
		if (f_mount(NULL, (TCHAR const*)SDPath, 1) == FR_OK) {
			retValue = true ;
		}
	}

	return retValue ;
}
//---------------------------------------------------------
/*!
 * @param inAddTime Время в секундах добавляемое к времени считанному из файла
 * \attention Инициализацию FatFS нужно проверять до вызова этого метода
 */
void TFileSystem::getTime (const uint32_t inAddTime)
{
	  FIL file { 0 } ;
	  FILINFO tempBuf { 0 } ;

	  bool isContinue { false } ;

//	  while ((common::app -> getState().second) < unit::stTagWaitSync) {	// ждём 10 сек для синхронизации времени
//		  FILINFO tempBuf { 0 } ;
//		  if (f_stat(common::stSyncTimeFileName.c_str(), &tempBuf) == FR_OK && tempBuf.fsize != 0) {
//			  isContinue = true ;
//			  break ;
//		  }
//	  }

	  if (f_stat(common::stSyncTimeFileName.c_str(), &tempBuf) == FR_OK && tempBuf.fsize != 0) isContinue = true ;

	  if (isContinue == false) {
		  common::app -> debugMessage ("File for time synchronization is absent");
		  common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 3) ;
	  }
	    else {
	      if (f_open(&file, common::stSyncTimeFileName.c_str (), FA_READ | FA_OPEN_EXISTING) == FR_OK) {
			  uint8_t dat[6] { 0 } ;
			  UINT bitesread { 0 } ;

			  RTC_TimeTypeDef syncTime ;
			  RTC_DateTypeDef syncDate ;
			  f_read(&file, dat, sizeof (dat), &bitesread) ;
			  f_close(&file);

			  tm timeFromFile, *timeWrite ;
			  timeFromFile.tm_sec = dat[5];
			  timeFromFile.tm_min = dat[4];
			  timeFromFile.tm_hour = dat[3];
			  timeFromFile.tm_year = dat[2];
			  timeFromFile.tm_mon = dat[1];
			  timeFromFile.tm_mday = dat[0];

			  time_t timeFromFileSec = mktime (&timeFromFile) ;
			  timeFromFileSec += inAddTime ;
			  timeWrite = localtime (&timeFromFileSec);

			  syncTime.Seconds = timeWrite -> tm_sec ;
			  syncTime.Minutes = timeWrite -> tm_min ;
			  syncTime.Hours   = timeWrite -> tm_hour ;
			  syncDate.Year    = timeWrite -> tm_year ;
			  syncDate.Month   = timeWrite -> tm_mon ;
			  syncDate.Date    = timeWrite -> tm_mday ;
			  syncDate.WeekDay = 1;
			  syncTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
			  syncTime.StoreOperation = RTC_STOREOPERATION_SET;
			  HAL_RTC_SetTime(&hrtc, &syncTime, RTC_FORMAT_BIN) ;
			  HAL_RTC_SetDate(&hrtc, &syncDate, RTC_FORMAT_BIN) ;

			  f_unlink(common::stSyncTimeFileName.c_str ());				// Т.к. от этого файла мы уже синхронизировались, то удаляем файл
			  common::app -> debugMessage ("Time synchronization success");
		  }
		    else {
			  common::app -> debugMessage ("Time synchronization error");
			  common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 2) ;
		    }
	  }
}
//---------------------------------------------------------
/*!
 * \attention Перед вызовом необходимо проверить, что Метка сосканирована и FS успешно инициализированна
 */
void TFileSystem::setTag ()
{
	FIL file { 0 } ;

	if (f_open (&file, common::stTagFileName.c_str (), FA_OPEN_APPEND | FA_WRITE) == FR_OK) {
		RTC_TimeTypeDef syncTime ;
		RTC_DateTypeDef syncDate ;
		UINT byteWrite { 0 } ;

		HAL_RTC_GetTime (&hrtc, &syncTime, RTC_FORMAT_BIN) ;
		HAL_RTC_GetDate (&hrtc, &syncDate, RTC_FORMAT_BIN) ;

		common::stFileBuf[0] = syncDate.Year ;
		common::stFileBuf[1] = syncDate.Month ;
		common::stFileBuf[2] = syncDate.Date ;
		common::stFileBuf[3] = syncTime.Hours ;
		common::stFileBuf[4] = syncTime.Minutes ;
		common::stFileBuf[5] = syncTime.Seconds ;

		common::stFileBuf[6] = 0x00 ;				// фиг его знает, что должно быть здесь
		common::stFileBuf[7] = 0x00 ;
		common::stFileBuf[8] = 0x00 ;
		common::stFileBuf[9] = 0x00 ;

		common::stFileBuf[10] = common::stTagBufId [20] ;	// Копируется ключ
		common::stFileBuf[11] = common::stTagBufId [21] ;
		common::stFileBuf[12] = common::stTagBufId [22] ;
		common::stFileBuf[13] = common::stTagBufId [23] ;

		common::stFileBuf[14] = 0x00 ;				// завершающие нули
		common::stFileBuf[15] = 0x00 ;

		if (f_write (&file, common::stFileBuf, 16, &byteWrite) != FR_OK) common::app -> setState(app::appState::appErrFileFS) ;
		  else {
			common::app -> debugMessage ("Scan TAG success");
			common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 1) ;
		  }
		f_sync (&file);
		f_close (&file);
	}
	  else {
	    common::app -> debugMessage ("Write TAG-file error");
		common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 3) ;
	  }
}
/*!-------------------------------------------------------------------
 * Запись изображения на SD.
 * @return true если запись выполнилась.
 */
bool TFileSystem::writePhoto ()
{
	bool retValue { false } ;

	if (openFileName (".jpg")) {
		UINT byteswrite { 0 } ;
		auto bufLen { sizeof (stPhotoBuf) } ;
		auto result = f_write(&mFile, stPhotoBuf, bufLen, &byteswrite) ;
		if (byteswrite == bufLen && result == FR_OK) {
			uint32_t size = f_size(&mFile);
			if (f_close (&mFile) == FR_OK) {
				common::app -> debugMessage("Photo file recording - OK") ;

				common::stFileBuf [0] = mDate.Date ;
				common::stFileBuf [1] = mDate.Month;
				common::stFileBuf [2] = mDate.Year;
				common::stFileBuf [3] = mTime.Hours;
				common::stFileBuf [4] = mTime.Minutes;
				common::stFileBuf [5] = mTime.Seconds;
				*(reinterpret_cast <uint32_t *> (&common::stFileBuf [6])) = 0x0 ;
				common::stFileBuf [10] = static_cast <uint8_t> ((mTag >> 24) & 0xFF) ;
				common::stFileBuf [11] = static_cast <uint8_t> ((mTag >> 16) & 0xFF) ;
				common::stFileBuf [12] = static_cast <uint8_t> ((mTag >> 8) & 0xFF) ;
				common::stFileBuf [13] = static_cast <uint8_t> (mTag & 0xFF) ;
//				*(reinterpret_cast <uint32_t *> (&common::stFileBuf [10])) = mTag ;
				*(reinterpret_cast <uint32_t *> (&common::stFileBuf [14])) = size ;

//				try {
//					if (f_open(&mFile, common::stPhotoFileName.c_str(), FA_OPEN_APPEND|FA_WRITE) != FR_OK) throw 1 ;
//				} catch (...) {
//					retValue = false ;
//					f_close (&mFile) ;				// Что-то пошло не так
//					common::app -> setState(app::appState::appErrFileFS) ;
//				}

				if (f_open(&mFile, common::stPhotoFileName.c_str(), FA_OPEN_APPEND|FA_WRITE) == FR_OK)
					if (f_write(&mFile, common::stFileBuf,20,&byteswrite) == FR_OK) {
						if (f_close(&mFile) == FR_OK) {
							common::app -> debugMessage("List photo files recording - OK") ;
							retValue = true ;
						}
						  else common::app -> debugMessage("List photo files recording - Error") ;
					}
			}
		}
	}

	if (!retValue) {
		f_close (&mFile) ;				// Что-то пошло не так
		common::app -> setState(app::appState::appErrFileFS) ;
	}
	return retValue ;
}
/*!------------------------------------------------------------
 * Формируется имя файла и он открывается
 * @param inFileName Расширения для имени файла
 * @return true если файл сформировался и открылся
 */
bool TFileSystem::openFileName (const std::string &inFileName)
{
	bool retValue { false } ;

	HAL_RTC_GetTime(&hrtc,&mTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&mDate,RTC_FORMAT_BIN);

	mTag = HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR0) ;
	std::stringstream tempFileName ;

	tempFileName << std::dec << std::internal << std::setfill('0') << std::setw (2) << static_cast<uint32_t> (mDate.Date)
																   << std::setw (2) << static_cast<uint32_t> (mDate.Month)
																   << std::setw (2) << static_cast<uint32_t> (mDate.Year)
																   << std::setw (2) << static_cast<uint32_t> (mTime.Hours)
																   << std::setw (2) << static_cast<uint32_t> (mTime.Minutes)
																   << std::setw (2) << static_cast<uint32_t> (mTime.Seconds) ;
	tempFileName << "00000000" ;
	tempFileName << std::hex << std::setfill('0') << std::setw (2) << ((mTag >> 24) & 0xFF)
												  << std::setw (2) << ((mTag >> 16) & 0xFF)
												  << std::setw (2) << ((mTag >> 8) & 0xFF)
												  << std::setw (2) << (mTag & 0xFF) ;
//	tempFileName << std::hex << std::setfill('0') << std::setw (2) << (mTag & 0xFF)
//												  << std::setw (2) << ((mTag >> 8) & 0xFF)
//												  << std::setw (2) << ((mTag >> 16) & 0xFF)
//												  << std::setw (2) << ((mTag >> 24) & 0xFF) ;

	mFileName = tempFileName.str() + inFileName ;
	FILINFO tempFileInfo ;
	if (f_stat(mFileName.c_str(), &tempFileInfo) == FR_OK)
		f_unlink(mFileName.c_str()) ;

	if (f_open(&mFile, mFileName.c_str(), FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) retValue = true ;

	return retValue ;
}
/*!---------------------------------------------------------
 *
 * @param inBuf Записываемые данные
 * @param inBufLen Длинна записываемых данных
 * @return Возвращает true при успешной записи
 */
bool TFileSystem::writeAudio (const uint8_t* inBuf, const uint32_t inBufLen)
{
	bool retVal { false } ;
	UINT bytesWrite { 0 } ;

	FRESULT result = f_write(&mFile, inBuf, inBufLen, &bytesWrite) ;
	if (result == FR_OK && inBufLen == bytesWrite) retVal = true ;

	return retVal ;
}
/*!-----------------------------------------------------------
 * Завершение записи аудио и запись метки в список аудио.
 */
void TFileSystem::closeAudio ()
{
	WAVE_FormatTypeDef tmpHeader ;

	uint32_t fileSize = static_cast <uint32_t> (f_size (&mFile)) - 8 ;
	tmpHeader.FileSize = fileSize ;
	fileSize = (fileSize - 36) ;
	tmpHeader.SubChunk2Size = fileSize ;
	f_lseek(&mFile, 0);
	UINT tmp ;
	f_write(&mFile, (uint8_t *)&tmpHeader, sizeof (tmpHeader), &tmp);
	f_close(&mFile) ;

	common::stFileBuf [0] = mDate.Date ;
	common::stFileBuf [1] = mDate.Month;
	common::stFileBuf [2] = mDate.Year;
	common::stFileBuf [3] = mTime.Hours;
	common::stFileBuf [4] = mTime.Minutes;
	common::stFileBuf [5] = mTime.Seconds;
	*(reinterpret_cast <uint32_t *> (&common::stFileBuf [6])) = 0x0 ;
	common::stFileBuf [10] = static_cast <uint8_t> ((mTag >> 24) & 0xFF) ;
	common::stFileBuf [11] = static_cast <uint8_t> ((mTag >> 16) & 0xFF) ;
	common::stFileBuf [12] = static_cast <uint8_t> ((mTag >> 8) & 0xFF) ;
	common::stFileBuf [13] = static_cast <uint8_t> (mTag & 0xFF) ;
//	*(reinterpret_cast <uint32_t *> (&common::stFileBuf [10])) = mTag ;
	*(reinterpret_cast <uint32_t *> (&common::stFileBuf [14])) = fileSize + 44 ;
	if (f_open(&mFile, common::stAudioFileName.c_str(), FA_OPEN_APPEND | FA_WRITE) == FR_OK)
		if (f_write(&mFile, common::stFileBuf,20,&tmp) == FR_OK)
			if (f_close(&mFile) == FR_OK) common::app -> debugMessage("List audio files recording - OK") ;

}
//------------------------------------------------------------
} /* namespace unit */
