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

const tdLogItem* stPtrLogStart { reinterpret_cast <tdLogItem*> (0x081E0000U) }  ;


/*!------------------------------------------------------------------
 *
 */
TLog::TLog()
{
	mPrtLastItem = reinterpret_cast <tdLogItem*> (HAL_RTCEx_BKUPRead (&hrtc, RTC_BKP_DR1)) ;
//	tdLogItem lastItem { 0, app::appState::appUnknown } ;

	if (mPrtLastItem == 0x0000) {	// Почему-то в регистре ни чего нет и нужно найти адрес последней записи (Например, при перезапуске контроллера)
		uint32_t i = 0 ;
		for (auto ptrItem = stPtrLogStart; ptrItem < stPtrLogStart + 0x2000; ++stPtrLogStart, ++i) {
			if (ptrItem -> dateTime == 0xFFFF) {
				if (i == 0) {
//					prtLastItem = ptrItem ;

				}
				HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, reinterpret_cast <uint32_t> (ptrItem)) ;
				break ;
			}
		}
	}
//	mStateQueue.push_back(*prtLastItem) ;
}
//-------------------------------------------------------------------
TLog::~TLog() {
	writeLog () ;
}
//--------------------------------------------------------------------
bool TLog::writeLog ()
{
	bool retVal { true } ;
									// Если коробочка не ушла в циклический перезапуск по железу, то записывем сожержимое контейнера на флешку
	if (mStateQueue.front().state != app::appState::appHwErr || mStateQueue.back().state != app::appState::appHwErr) {
		// Здесь нужно проверить место на флешке и если нужно, то скинуть все на SDIO

		if (mStateQueue.size() != 0) {
			std::vector <tdLogItem> tmpBuf (mStateQueue.begin(), mStateQueue.end()) ;	// Вектор нужен тут исключительно для того, что бы получить буфер .data () и не трахать себе мозги
			HAL_FLASH_Unlock() ;
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, reinterpret_cast <uint32_t> (tmpBuf.data()), sizeof (tdLogItem) * tmpBuf.size()) ;
			HAL_FLASH_Lock() ;
		}
	}

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
	  case app::appState::appStarted :				// Финсируем только нужные состояния
	  case app::appState::appReady :
	  case app::appState::appAudio :
	  case app::appState::appAudioErr :
	  case app::appState::appPhoto :
	  case app::appState::appPhotoI2CErr :
	  case app::appState::appPhotoTimeout :
	  case app::appState::appTag :
	  case app::appState::appTagErr :
	  case app::appState::appDoc :
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
/*!-----------------------------------------------------------------------
 *
 */
void TLog::writeItemToFlash ()
{

}
//--------------------------------------------------------------------
} /* namespace app */
