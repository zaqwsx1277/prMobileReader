/*
 * TLog.cpp
 *
 *  Created on: Aug 9, 2021
 *      Author: AAL
 */

#include <vector>

#include <rtc.h>

#include <TLog.hpp>

namespace app {

tdLogItem* stLogPrt { reinterpret_cast <tdLogItem*> (stLogPtrStart) }  ;

/*!------------------------------------------------------------------
 * @todo Сделать проверку, на полностью заполненную флешку
 */
TLog::TLog()
{
	mPrtLastItem = reinterpret_cast <tdLogItem*> (HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR1)) ;
	tdLogItem lastItem { 0, app::appState::appUnknown } ;

	if (mPrtLastItem != 0x0000) lastItem = getLastItem () ;
	  else {		// Почему-то в регистре ни чего нет и нужно найти адрес последней записи
		uint32_t i = 0 ;
		for (auto ptrItem = stLogPrt; ptrItem < reinterpret_cast <tdLogItem*> (stLogPtrEnd); ++ptrItem, ++i) {
			if (ptrItem -> dateTime == 0xFFFFFFFF) {
				mPrtLastItem = ptrItem ;
				HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, reinterpret_cast <uint32_t> (ptrItem)) ;
				if (i != 0) lastItem = getLastItem () ;
				break ;
			}
		}
	  }

	mStateQueue.push_back(lastItem) ;
}
//-------------------------------------------------------------------
TLog::~TLog() {
	writeLog () ;
}
/*!------------------------------------------------------------------
 * @attention Состояние перехода в StandBy писать не надо, т.к. оно всегда будет последним
 * @return true если запись на флешку прошла без ошибок
 */
bool TLog::writeLog ()
{
	bool retVal { true } ;
									// Если коробочка не ушла в циклический перезапуск по железу, то записывем сожержимое контейнера на флешку
	if (mStateQueue.front().state != app::appState::appHwErr || mStateQueue.back().state != app::appState::appHwErr) {
		// Здесь нужно проверить место на флешке и если нужно, то скинуть все на SDIO

		if (mStateQueue.size() != 0) {
			if (HAL_FLASH_Unlock() != HAL_OK) retVal = false ;
			if ((stLogPtrEnd - reinterpret_cast <uint32_t> (mPrtLastItem)) < (mStateQueue.size() * sizeof (tdLogItem))) {
				FLASH_EraseInitTypeDef flashStr { 0 } ;
				uint32_t sectorError { 0 } ;

				flashStr.TypeErase = FLASH_TYPEERASE_SECTORS ;
				flashStr.VoltageRange = FLASH_VOLTAGE_RANGE_3 ;
				flashStr.Sector = FLASH_SECTOR_23 ;
				flashStr.NbSectors = 1 ;
				if (HAL_FLASHEx_Erase(&flashStr, &sectorError) != HAL_OK) retVal = false ;
			}

			for (auto item : mStateQueue) {		// Т.к. нам торопиться некуда, то пишем данные без DMA. И нужно быть внимательным, при изменении структуры tdLogItem
				if (retVal != true) break ;
				uint32_t prtDateTime = reinterpret_cast <uint32_t> (mPrtLastItem) ;
				uint32_t ptrState = prtDateTime + sizeof (item.dateTime) ;
				if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, prtDateTime, item.dateTime) != HAL_OK) retVal = false ;
				if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ptrState, item.state) != HAL_OK) retVal = false ;
				++mPrtLastItem ;
			}
			HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, reinterpret_cast <uint32_t> (mPrtLastItem)) ;
			if (HAL_FLASH_Lock() != HAL_OK) retVal = false ;
		}
	}

	mStateQueue.clear() ;			// на хрена я чищу контейнер я не знаю, т.к. контроллер всё равно идет в режим StandBy
	return retVal ;
}
//--------------------------------------------------------------------
/*!
 * Формирование очереди сообщений которые нужно записывать в лог.
 * Новые сообщения пишутся в конец (push_back)
 * По хорошему, можно использовать std::vector и писать флешку всё сразу массивом ...data, но хрен его знает, что будет при динамическом выделении памяти. :(
 * @param inState записываемое состояние
 */
void TLog::pushItem (const app::appState& inState)
{
	RTC_TimeTypeDef getTime { 0 } ;
	RTC_DateTypeDef getDate { 0 } ;
	tdLogItem logItem { 0 } ;

	switch (inState) {
	  case app::appState::appStarted :				// Фиксируем только нужные состояния
	  case app::appState::appReady :
	  case app::appState::appAudio :
	  case app::appState::appAudioErr :
	  case app::appState::appPhoto :
	  case app::appState::appPhotoI2CErr :
	  case app::appState::appPhotoTimeout :
	  case app::appState::appTag :
	  case app::appState::appTagErr :
	  case app::appState::appDoc :
	  case app::appState::appDocSyncTime :
	  case app::appState::appErrButton :
	  case app::appState::appErrI2C :
	  case app::appState::appErrBq25121:
	  case app::appState::appErrSDIO :
	  case app::appState::appErrFileFS :
	  case app::appState::appFsRepair :
	  case app::appState::appHwErr :

		HAL_RTC_GetTime (&hrtc, &getTime, RTC_FORMAT_BIN) ;
		HAL_RTC_GetDate (&hrtc, &getDate, RTC_FORMAT_BIN) ;

		logItem.dateTime = getTime.Seconds & 0b111111 ;
		logItem.dateTime |= (getTime.Minutes & 0b111111) << 6 ;
		logItem.dateTime |= (getTime.Hours & 0b111111) << 12 ;
		logItem.dateTime |= (getDate.Date & 0b11111) << 16 ;
		logItem.dateTime |= (getDate.Month & 0b1111) << 21 ;
		logItem.dateTime |= (getDate.Year & 0b111111) << 26 ;
		logItem.state = inState ;

		mStateQueue.push_back(logItem) ;
	  break;

	  default:
	  break;
	}
}
/*!---------------------------------------------------------------------------------------
 *
 * @return Последнее записанное в лог состояние
 */
tdLogItem TLog::getLastItem ()
{
	tdLogItem retValue { 0 } ;

	tdLogItem* prtLastItem = mPrtLastItem - 1 ;
	retValue.dateTime = prtLastItem -> dateTime ;
	retValue.state = prtLastItem -> state ; //reinterpret_cast <tdLogItem*> (HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR1)) ;

	return retValue ;

}
//--------------------------------------------------------------------
} /* namespace app */
