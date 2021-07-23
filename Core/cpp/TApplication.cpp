/*
 * TApplication.cpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */

#include "TApplication.hpp"

#include <rtc.h>
#include <usart.h>
#include <i2c.h>

#include "TCommon.hpp"

namespace app {

TButton defGpioPhoto ( GPIOA, GPIO_PIN_0 ) ;
TButton defGpioTag ( GPIOB, GPIO_PIN_12 ) ;
TButton defGpioAudio ( GPIOE, GPIO_PIN_3 ) ;
TButton defGpioDoc ( GPIOA, GPIO_PIN_9 ) ;

//-----------------------------------------------------------
TApplication::TApplication()
{
	HAL_PWR_EnableBkUpAccess() ;			// Чтобы не взрывать себе мозги, доступ к backup регистрам открываем навсегда
	mAppStateChange = HAL_GetTick() ;

	uint32_t xxx = 0xA223355 ;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, xxx) ;
}
/*!----------------------------------------------------------
 *
 */
TApplication::~TApplication()
{

}
//-----------------------------------------------------------
/*!
 * Проверка периферийного оборудование. Выполняется при запуске устройства и при снятии с док станции.
 * \attention После вызова этого метода, все периферийные устройства будут размонтированы и поэтому ОБЯЗАТЕЛЬНО (!!!) нужно уходить в режим StandBy или инициализировать по новой
 */
void TApplication::checkUnits ()
{
//	if (mFileSystem == nullptr) mFileSystem = std::make_unique <unit::TFileSystem> () ;
//	  else mFileSystem -> check () ;								// И на хрена что-то проверять, если FS уже смонтирована?
//	if (mTag == nullptr) mTag = std::make_unique <app::TTag> () ;
//	if (mPhoto = nullptr) mPhoto = std::make_unique <app::TPhoto> () ;
//	if (mAudio == nullptr) mAudio = std::make_unique <app::TAudio> () ;

//	if (mI2c -> check () != true) mAppState = app::appState::appErrI2C ;

//	if (mFileSystem != nullptr) mFileSystem.release() ;
//	if (mTag != nullptr) mTag.release() ;
//	if (mPhoto != nullptr) mPhoto.release() ;
//	if (mAudio != nullptr) mAudio.release() ;
}
//-----------------------------------------------------------
void TApplication::stateManager ()
{
	switch (mAppState) {
	  case appState::appStandBy: {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET) ;
		if (mFileSystem != nullptr) mFileSystem.release() ;		// Дергаем все деструктры, т.к. в них может быть перевод устройства в режим Sleep
		if (mTag != nullptr) mTag.release() ;
		if (mPhoto != nullptr) mPhoto.release() ;
		if (mAudio != nullptr) mAudio.release() ;
		if (mSdio != nullptr) mSdio.release() ;

		HAL_PWR_DisableBkUpAccess();
#ifdef DEBUG
		HAL_Delay(5000) ;
		makeInfo(typeInfo::infoAudioLight,tsndShort, 1) ;
		debugMesage() ;
		common::app -> debugMesage ("Time now: " + common::app -> getMessageTime ()) ;
#else
		makeInfo(typeInfo::infoAudio, tsndShort, 1) ;

		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_All ;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_InitTypeDef GPIO_InitStructX = {0};
		GPIO_InitStructX.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                |GPIO_PIN_3 |GPIO_PIN_1; ;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructX);


		__HAL_RCC_PWR_CLK_ENABLE() ;
		HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1) ;
		HAL_PWR_EnterSTANDBYMode() ;
#endif /* DEBUG */
	  }
	  break;

	  case appState::appStartBounce:
		  startBounce () ;
		  setState(app::appState::appCheckBounce) ;
	  break ;

	  case appState::appCheckBounce:		// Сам дребезг контактов устраняется при срабанывания прерывания TIM7
		  if ((HAL_GetTick() - mAppStateChange) > 100)
			  setState(appState::appBounceTimeout) ;
	  break ;

	  case appState::appCheckButton: {
		  app::appState state { appErrButton } ;
		  GPIO_PinState bPhoto = mButton [idButton::btnPhoto].getState() ;
		  GPIO_PinState bAudio = mButton [idButton::btnAudio].getState() ;
		  GPIO_PinState bTag = mButton [idButton::btnTag].getState() ;
		  GPIO_PinState bDoc = mButton [idButton::btnDoc].getState() ;
		  if (bPhoto == GPIO_PIN_RESET && bTag == GPIO_PIN_SET && bAudio == GPIO_PIN_SET && bDoc == GPIO_PIN_RESET) state = appState::appStandBy ;
		  if (bPhoto == GPIO_PIN_SET && bTag == GPIO_PIN_RESET && bAudio == GPIO_PIN_SET && bDoc == GPIO_PIN_RESET) state = appState::appTag ;
		  if (bPhoto == GPIO_PIN_SET && bTag == GPIO_PIN_SET && bAudio == GPIO_PIN_RESET && bDoc == GPIO_PIN_RESET) state = appState::appAudio ;
		  if (bPhoto == GPIO_PIN_SET && bTag == GPIO_PIN_SET && bAudio == GPIO_PIN_SET && bDoc == GPIO_PIN_SET) state = appState::appDocSyncTime ;
		  if (bPhoto == GPIO_PIN_SET && bTag == GPIO_PIN_SET && bAudio == GPIO_PIN_SET && bDoc == GPIO_PIN_RESET) state = appState::appPhoto ;
		  setState(state) ;
	  }
	  break ;

	  case appState::appAudio :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  if (mSdio == nullptr) mSdio = std::make_unique <unit::TSdio> () ;		// Если нужно, то инициализируем все классы, а если не нужно, то значит мы попали сюда из состояния appAudioWaitStop
		  if (mFileSystem == nullptr) mFileSystem = std::make_unique <unit::TFileSystem> () ;
		  if (mAudio == nullptr) mAudio = std::make_unique <unit::TAudio> () ;

		  while (getState ().first == appState::appAudio) {
			  if (getState().second > unit::stAudioDuration) setState(appState::appAudioStop) ;
			  if (mButton [btnAudio].getState() != GPIO_PIN_RESET) {			// Если прекращается запись звука, то ждём 500 мСек
				  setState(appState::appAudioWaitStop) ;
			  }
		  }
	  break ;

	  case appState::appAudioWaitStop: 											// Ждём 500 мСек и проваливаемся в StandBy
		  if (getState().second > 500) setState(appState::appAudioStop) ;
		  if (mButton [idButton::btnAudio].getState() == GPIO_PIN_RESET) setState(appState::appAudio) ;
	  break ;

	  case appState::appAudioStop :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  if (mAudio == nullptr) mAudio.release() ;
		  setState(appState::appStandBy) ;
	  break ;

	  case appState::appPhoto :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  mPhoto -> process() ;
		  while (mButton [idButton::btnPhoto].getState() == GPIO_PIN_SET) {
			  if (getState().second > unit::stPhotoTimeWait && getState().first == appState::appPhoto) {
				  setState(appState::appPhotoButtonPress) ;
				  makeInfo(typeInfo::infoAudioLight, tsndContinue, 0) ;
			  }
		  }
		  if (getState().first == appState::appPhoto) setState(appState::appStandBy) ;
	  break ;

	  case appState::appTag :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  if (mTag -> process ()) {
			  mSdio = std::make_unique <unit::TSdio> () ;
			  mFileSystem = std::make_unique <unit::TFileSystem> () ;
			  if (getState ().first == appState::appTag) mFileSystem -> setTag() ;			// в методе setTag могут быть ошибки!!!
			  if (getState ().first == appState::appTag) setState(appState::appStandBy) ; // если ошибок нет, то переходим в StandBy, а если есть, то сначала их обрабатываем
		  }
	  break ;

	  case appState::appDoc :
#ifdef DEBUG
		  while (1) {//mButton [idButton::btnDoc].getState() == GPIO_PIN_SET) {
			  HAL_Delay(1000) ;
			  common::app -> debugMesage ("Time now: " + common::app -> getMessageTime ()) ;
		  } ;
#else
		  while (mButton [idButton::btnDoc].getState() == GPIO_PIN_SET) {
			  HAL_Delay(1000) ;
			  common::app -> debugMesage ("Time now: " + common::app -> getMessageTime ()) ;
		  }
#endif
		  setState(appState::appStandBy) ;
	  break ;

	  case appState::appDocSyncTime:
		  mSdio = std::make_unique <unit::TSdio> () ;
		  mFileSystem = std::make_unique <unit::TFileSystem> () ;
		  if (getState().first != appErrFileFS) {
			  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
			  mFileSystem -> getTime () ;
			  mFileSystem.release() ;
		  }
		    else {
		      mFileSystem.release() ;
			  mSdio.release() ;										// Т.к. ошибка FS, то выдаем сообщение и проваливаемся в режим StandBy
		    }
		  setState(appState::appDoc) ;
	  break ;

	  case appState::appErrI2C :
		  makeInfo(typeInfo::infoAudioLight, tsndLong, 1) ;
		  HAL_Delay(500) ;
		  makeInfo(typeInfo::infoAudioLight,tsndShort, 1) ;
		  setState(appState::appStandBy) ;
	  break;

	  case appState::appErrBq25121 :
		  makeInfo(typeInfo::infoAudioLight, tsndLong, 1) ;
		  HAL_Delay(500) ;
		  makeInfo(typeInfo::infoAudioLight,tsndShort, 2) ;
		  setState(appState::appStandBy) ;
	  break;

	  case appState::appBounceTimeout:
	  case appState::appTagNoId :
	  case appState::appErrFileFS :
	  case appState::appPhotoButtonPress :
	  case appState::appErrButton :
	  case appState::appPhotoTimeout :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 2) ;
		  setState(appState::appStandBy) ;
	  break ;

	  default:
		  common::app -> debugMesage ("No command handler") ;
		  makeInfo(typeInfo::infoAudioLight, tsndLong, 2000) ;
		  setState(appState::appStandBy) ;
	  break;
	}
}
//-----------------------------------------------------------
/*!
 *
 * @param inAppState
 * @return При успешной установке статуса возвращается true
 * @attention Возврат может пригодиться в будущем, если вдруг будет учитываться допустимость перехода состояний
 */
bool TApplication::setState (app::appState inAppState)
{
	bool retValue { false } ;

	if (inAppState != mAppState) {
		debugMesage(msgAppState.at (inAppState)) ;
		mAppState = inAppState ;
		mAppStateChange = HAL_GetTick() ;
		retValue = true ;
	}

	return retValue ;
}
//-----------------------------------------------------------
/*!
 *
 * @return пара - Текущее состояние и продолжительность данного состояния
 */
std::pair <app::appState, uint32_t> TApplication::getState ()
{

	return std::make_pair(mAppState, HAL_GetTick() - mAppStateChange) ;
}
//-----------------------------------------------------------
/*!
 * Включение пищалки и звука
 * Если продолжительность звучания равна 0, то сигнал будет звучать непрерывно и прервать его будет нельзя.
 * Если продолжительность звучания меньше 100, то измеряется в количестве пиков (максимально 99)
 * @param inTypeSound Тип звукового сигнала
 * @param inDuration Продолжительность в милисекундах. Если = 0, то сигнал звучит непрерывно.
 */
void TApplication::makeInfo (const app::typeInfo inTypeInfo, const app::typeSound inTypeSound, const uint32_t inDuration)
{
	uint32_t timeStart = HAL_GetTick() ;

	switch (inTypeSound) {
	  case typeSound::tsndContinue :
		makeInfoGpio (inTypeInfo, GPIO_PIN_SET) ;
		if (inDuration != 0) {
			HAL_Delay(inDuration) ;
			makeInfoGpio (inTypeInfo, GPIO_PIN_RESET) ;
		}
	  break;

	  case typeSound::tsndNo:
	    makeInfoGpio (inTypeInfo, GPIO_PIN_RESET) ;
	  break;

	  case typeSound::tsndShort: {
		uint32_t timeFinish = inDuration < 100 ? timeStart + 110 * inDuration : timeStart + inDuration ;

		while (HAL_GetTick() < timeFinish|| inDuration == 0) {
			makeInfoGpio (inTypeInfo, GPIO_PIN_SET) ;
			HAL_Delay(10) ;
			makeInfoGpio (inTypeInfo, GPIO_PIN_RESET) ;
			HAL_Delay(100) ;
		}
		makeInfoGpio (inTypeInfo, GPIO_PIN_RESET) ;
	  }
	  break;

	  case typeSound::tsndLong: {
		uint32_t timeFinish = inDuration < 100 ? timeStart + 1000 * inDuration : timeStart + inDuration ;
		while (HAL_GetTick() < timeFinish || inDuration == 0) {
			makeInfoGpio (inTypeInfo, GPIO_PIN_SET) ;
			HAL_Delay(500) ;
			makeInfoGpio (inTypeInfo, GPIO_PIN_RESET) ;
			HAL_Delay(500) ;
		}
		makeInfoGpio (inTypeInfo, GPIO_PIN_RESET) ;
	  }
	  break;

	  default:
	  break;
	}
}
//-----------------------------------------------------------
/*!
 * @param inTypeInfo Тип информации (пищать/моргать)
 * @param inState Устанавливаемое состояние GPIO
 */
void TApplication::makeInfoGpio (const app::typeInfo inTypeInfo, const GPIO_PinState inState)
{
	if (inTypeInfo == infoAudio || inTypeInfo == infoAudioLight) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, inState) ;
	if (inTypeInfo == infoLight || inTypeInfo == infoAudioLight) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, inState) ;
}
//-----------------------------------------------------------
/*!
 * @param inMesage Текстовая строка которую нужно вывести в отладочный порт
 * @todo Переделать на хрен на потоковый вывод
 */
void TApplication::debugMesage (const std::string &inMesage)
{
	uint32_t timeStart = HAL_GetTick() ;
	while (HAL_UART_GetState (&huart1) == HAL_UART_STATE_BUSY_TX)
		if ((HAL_GetTick() - timeStart) > 1000) return ;
	common::stTempStr = (inMesage  + "\r\n") ;
	debugMesage (common::stTempStr.c_str(), inMesage.size() + 2) ;
}
//-----------------------------------------------------------
/*!
 * Данный метод выводит строку в отладочный порт
 * @param inMesage Указатель на строку, которую нужно вывести в отладочный порт
 * @param inSize Размер выводимого бувера
 */
void TApplication::debugMesage (const char *inMesage, const std::size_t inSize)
{
	debugMesage ((const uint8_t *)inMesage, inSize) ;
}
/*!-----------------------------------------------------------
 * @param inMesage Указатель на массив, который нужно вывести в отладочный порт
 * @param inSize Размер выводимого бувера
 * @todo Переделать на хрен на потоковый вывод
 * @attention На хрена я приделал callback я понятия не имею :(
 */
void TApplication::debugMesage (const uint8_t *inMesage, const std::size_t inSize)
{
//	uint32_t timeStart = HAL_GetTick() ;
	while(HAL_UART_Transmit_IT(&huart1, (uint8_t *) inMesage, inSize) == HAL_BUSY ) {
//		if ((HAL_GetTick() - timeStart) > stAppDebugTimeout) break ;	// что бы если что, то выйти из бесконечного цикла
	}
}
//-----------------------------------------------------------
void TApplication::debugMesage ()
{
	debugMesage (mAppState) ;
}
//-----------------------------------------------------------
/*!
 * @param inState Состояние из appState
 */
void TApplication::debugMesage (const appState inState)
{
	auto res = msgAppState.find (inState) ;
	if (res != msgAppState.end ()) debugMesage (res -> second) ;
	  else debugMesage ("Unknown breed beast") ;
}
//-----------------------------------------------------------
void TApplication::startBounce ()
{
	for (auto &item : mButton) item.startBounce() ;
}
//-----------------------------------------------------------
/*!
 * @attention Данный запускается в обработчике прерывания и поэтому не фиг споли жевать. Быстренько проверяем нажатые кнопки и завершаем выполнение
 */
void TApplication::checkBounce ()
{
	bool bounce = true ;			// Флаг завершения устранения дребезга контактов
	for (auto &item : mButton) {
		if (!item.checkBounce()) bounce = false ;
	}
	if (bounce) setState(app::appState::appCheckButton) ;
}
//-----------------------------------------------------------
/*!
 * @return Возврат строки с текущими датой и временем
 */
std::string TApplication::getMessageTime ()
{
	RTC_TimeTypeDef time { 0 } ;
	RTC_DateTypeDef date { 0 } ;

	HAL_RTC_GetTime (&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	return std::to_string(date.Date) + "." + std::to_string(date.Month) + "." + std::to_string(date.Year) + " " +
		   std::to_string(time.Hours) + ":" + std::to_string(time.Minutes) + ":" + std::to_string(time.Seconds) ;
}
//-----------------------------------------------------------
void TApplication::writePhoto ()
{
	mFileSystem -> writePhoto () ;
}
//-----------------------------------------------------------
} /* namespace app */
