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
#include <exception>

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
	if (f_mount(&SDFatFS, (TCHAR const*)SDPath, 1) != FR_OK)
		common::app -> setState(app::appState::appErrFileFS) ;
	  else common::app -> debugMesage ("File system mount - OK.") ;
}
//---------------------------------------------------------
/*!
 * \attention Файловая система должна быть размонтирована всегда, в противном случае SD'шка убивается
 */
TFileSystem::~TFileSystem() {
	common::app -> debugMesage ("File system unmount.") ;
	f_mount(NULL, (TCHAR const*)SDPath, 1) ;
}
//---------------------------------------------------------
/*!
 * \todo Приделать проверку записи и чтения тестового файла
 * @return
 */
bool TFileSystem::check () {
	return true ;
}
//---------------------------------------------------------
/*!
 * \attention Инициализацию FatFS нужно проверять до вызова этого метода
 */
void TFileSystem::getTime ()
{
	  FIL file { 0 } ;

	  bool isContinue { false } ;

	  while ((common::app -> getState().second) < unit::stTagWaitSync) {	// ждём 5 сек для синхронизации времени
		  FILINFO tempBuf { 0 } ;
		  if (f_stat(common::stSyncTimeFileName.c_str(), &tempBuf) == FR_OK) {
			  isContinue = true ;
			  continue ;
		  }
	  }

	  if (f_open(&file, common::stSyncTimeFileName.c_str (), FA_READ | FA_OPEN_EXISTING) == FR_OK && isContinue) {
		  uint8_t dat[6] { 0 } ;
		  UINT bitesread { 0 } ;

		  RTC_TimeTypeDef syncTime ;
		  RTC_DateTypeDef syncDate ;
		  f_read(&file, dat, sizeof (dat), &bitesread) ;
		  f_close(&file);

		  syncTime.Seconds = dat[5];
		  syncTime.Minutes = dat[4];
		  syncTime.Hours   = dat[3];
		  syncDate.Year    = dat[2];
		  syncDate.Month   = dat[1];
		  syncDate.Date    = dat[0];
		  syncDate.WeekDay = 1;
		  syncTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		  syncTime.StoreOperation = RTC_STOREOPERATION_SET;
		  HAL_RTC_SetTime(&hrtc, &syncTime, RTC_FORMAT_BIN) ;
		  HAL_RTC_SetDate(&hrtc, &syncDate, RTC_FORMAT_BIN) ;

//		  f_unlink(common::stSyncTimeFileName.c_str ());				// Т.к. от этого файла мы уже синхронизировались, то удаляем файл
		  common::app -> debugMesage ("Time synchronization success");
		  common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 1) ;
	  }
	    else {
	      common::app -> debugMesage ("Time synchronization error");
		  common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 3) ;
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
			uint32_t tempId = *(reinterpret_cast <uint32_t *> (&common::stTagBufId [20])) ;
			HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, tempId) ;
			common::app -> debugMesage ("Scan TAG success");
			common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 1) ;
		  }
		f_sync (&file);
		f_close (&file);
	}
	  else {
	    common::app -> debugMesage ("Write TAG-file error");
		common::app -> makeInfo (app::typeInfo::infoAudioLight, app::typeSound::tsndShort, 3) ;
	  }
}
//---------------------------------------------------------
bool TFileSystem::writePhoto ()
{
	bool retValue { false } ;

	FIL file { 0 } ;
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);

	uint32_t tempTag = HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR0) ;
	std::stringstream tempFileName ;

	tempFileName << std::dec << std::internal << std::setfill('0') << std::setw (2) << static_cast<uint32_t> (date.Date)
																   << std::setw (2) << static_cast<uint32_t> (date.Month)
																   << std::setw (2) << static_cast<uint32_t> (date.Year)
																   << std::setw (2) << static_cast<uint32_t> (time.Hours)
																   << std::setw (2) << static_cast<uint32_t> (time.Minutes)
																   << std::setw (2) << static_cast<uint32_t> (time.Seconds) ;
	tempFileName << "00000000" ;
	tempFileName << std::hex << std::setfill('0') << std::setw (2) << ((tempTag >> 24) & 0xFF)
												  << std::setw (2) << ((tempTag >> 16) & 0xFF)
												  << std::setw (2) << ((tempTag >> 8) & 0xFF)
												  << std::setw (2) << (tempTag & 0xFF) ;
	tempFileName << ".jpg" ;
#ifdef DEBUG
	common::app -> debugMesage (tempFileName.str ()) ;
#endif

	try {
		if (f_open(&file, tempFileName.str ().c_str(), FA_CREATE_ALWAYS|FA_WRITE) != FR_OK) throw ;
		UINT byteswrite { 0 } ;
		auto bufLen { sizeof (common::stFileBuf) } ;
		auto result = f_write(&file, common::stFileBuf, bufLen, &byteswrite) ;
		if (byteswrite != bufLen || result != FR_OK) throw ;
		uint32_t size = f_size(&file);
		if (f_close (&file) != FR_OK) throw ;
		common::app -> debugMesage("Photo file recording - OK") ;

		common::stFileBuf [0] = date.Date ;
		common::stFileBuf [1] = date.Month;
		common::stFileBuf [2] = date.Year;
		common::stFileBuf [3] = time.Hours;
		common::stFileBuf [4] = time.Minutes;
		common::stFileBuf [5] = time.Seconds;
		*(reinterpret_cast <uint32_t *> (&common::stFileBuf [6])) = 0x0 ;
		*(reinterpret_cast <uint32_t *> (&common::stFileBuf [10])) = tempTag ;
		*(reinterpret_cast <uint32_t *> (&common::stFileBuf [14])) = size ;
		if (f_open(&file, common::stPhotoFileName.c_str(), FA_OPEN_APPEND|FA_WRITE) != FR_OK) throw ;
		if (f_write(&file, common::stFileBuf,20,&byteswrite) != FR_OK) throw ;
		if (f_close(&file) != FR_OK) throw ;
		common::app -> debugMesage("List photo files recording - OK") ;

		retValue = true ;
	}
	  catch (...) {
		retValue = false  ;
		f_unlink(tempFileName.str ().c_str());					// Что-то пошло не так
		common::app ->setState(app::appState::appErrFileFS) ;
	  }
	return retValue ;
}
//---------------------------------------------------------
} /* namespace app */
