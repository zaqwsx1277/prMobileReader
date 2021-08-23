/*
 * TApplication.cpp
 *
 *  Created on: Jun 2, 2021
 *      Author: AAL
 */
#include <string>
#include <sstream>
#include <ios>
#include <iomanip>

#include "TApplication.hpp"

#include <rtc.h>
#include <usart.h>
#include <i2c.h>
#include <tim.h>

#include "TCommon.hpp"

namespace app {

TButton defGpioPhoto ( GPIOA, GPIO_PIN_0 ) ;
TButton defGpioTag ( GPIOB, GPIO_PIN_12 ) ;
TButton defGpioAudio ( GPIOE, GPIO_PIN_3 ) ;
TButton defGpioDoc ( GPIOA, GPIO_PIN_9 ) ;

//-----------------------------------------------------------
TApplication::TApplication()
{
	mAppStateChange = HAL_GetTick() ;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET) ;// Светодиод включесктся навсегда
}
/*!----------------------------------------------------------
 *	Деструктор в принципе не нужен вообще.
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

	if (mSdio == nullptr) mSdio = std::make_unique <unit::TSdio> () ;		// Если нужно, то инициализируем все классы, а если не нужно, то значит мы попали сюда из состояния appAudioWaitStop
	if (mFileSystem == nullptr) mFileSystem = std::make_unique <unit::TFileSystem> () ; // Размонтирование файловой системы нужно выполнять раньше отключения SDIO
//	if (mTag == nullptr) mTag = std::make_unique <app::TTag> () ;
	if (mPhoto == nullptr) mPhoto = std::make_unique <unit::TPhoto> () ;
//	if (mAudio == nullptr) mAudio = std::make_unique <app::TAudio> () ;

//	if (mI2c -> check () != true) mAppState = app::appState::appErrI2C ;

	if (mFileSystem -> check () == true) common::app -> debugMessage ("FS system check - OK") ;
	  else common::app -> debugMessage ("FS system check - Error") ;
	if (mPhoto -> check() == true) common::app -> debugMessage ("Photo check - OK") ;
	  else common::app -> debugMessage ("Photo check - Error") ;


	if (mFileSystem != nullptr) mFileSystem.reset() ;
	if (mSdio != nullptr) mSdio.reset() ;
//	if (mTag != nullptr) mTag.release() ;
	if (mPhoto != nullptr) mPhoto.release() ;
//	if (mAudio != nullptr) mAudio.release() ;
}
//-----------------------------------------------------------
void TApplication::stateManager ()
{
	switch (mAppState) {
	  case appState::appStandBy: {
		HAL_TIM_Base_Stop_IT(&htim6) ;
		HAL_TIM_Base_Stop_IT(&htim7) ;

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET) ;
		if (mFileSystem != nullptr) mFileSystem.reset() ;		// Дергаем все деструктры, т.к. в них должен быть перевод устройства в режим Sleep
		if (mTag != nullptr) mTag.reset() ;
		if (mPhoto != nullptr) mPhoto.reset() ;
		if (mAudio != nullptr) mAudio.reset() ;
		if (mSdio != nullptr) mSdio.reset() ;
		if (mLog != nullptr) mLog.reset() ;

#ifdef DEBUG
		HAL_Delay(5000) ;
		makeInfo(typeInfo::infoAudioLight,tsndShort, 1) ;
		debugMessage() ;
		common::app -> debugMessage ("Time now: " + common::app -> getMessageTime ()) ;
		common::app -> setState (app::appState::appCheckButton) ;
#else
//		HAL_PWR_DisableBkUpAccess();
		sleep () ;
#endif /* DEBUG */
	  }
	  break;

	  case appState::appReady:
		  setState(app::appState::appStartBounce) ;
	  break ;

	  case appState::appStartBounce:
		  startBounce () ;
		  setState(app::appState::appCheckBounce) ;
	  break ;

	  case appState::appCheckBounce:		// Сам дребезг контактов устраняется при срабатывания прерывания TIM7
		  if ((HAL_GetTick() - mAppStateChange) > 100) setState(appState::appBounceTimeout) ;
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
		  if (bPhoto == GPIO_PIN_SET && bTag == GPIO_PIN_SET && bAudio == GPIO_PIN_SET && bDoc == GPIO_PIN_SET) state = appState::appDocStupid ;
		  if (bPhoto == GPIO_PIN_SET && bTag == GPIO_PIN_SET && bAudio == GPIO_PIN_SET && bDoc == GPIO_PIN_RESET) state = appState::appPhoto ;
		  setState(state) ;
	  }
	  break ;

	  case appState::appAudio : {
		  clearStupid () ;
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  if (mSdio == nullptr) mSdio = std::make_unique <unit::TSdio> () ;		// Если нужно, то инициализируем все классы, а если не нужно, то значит мы попали сюда из состояния appAudioWaitStop
		  if (mFileSystem == nullptr && mAppState == appState::appAudio) mFileSystem = std::make_shared <unit::TFileSystem> () ;
		  if (mAudio == nullptr && mAppState == appState::appAudio) mAudio = std::make_unique <unit::TAudio> (mFileSystem) ;

		  while (getState ().first == appState::appAudio) {
			  if (mAudio -> process() == false) setState(appState::appAudioErr) ;
			  if (getState().second > unit::stAudioDuration) setState(appState::appAudioStop) ;
			  if (mButton [btnAudio].getState() != GPIO_PIN_RESET) {			// Если прекращается запись звука, то ждём 500 мСек
				  setState(appState::appAudioWaitStop) ;
			  }
		  }
	  }
	  break ;

	  case appState::appAudioWaitStop: 											// Ждём 500 мСек и проваливаемся в StandBy
		  while (getState ().first == appState::appAudioWaitStop) {
			  if (getState().second > 500) setState(appState::appAudioStop) ;
			  if (mButton [idButton::btnAudio].getState() == GPIO_PIN_RESET) setState(appState::appAudio) ;
		  }
	  break ;

	  case appState::appAudioStop :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  if (mAudio == nullptr) mAudio.reset () ;
		  setState(appState::appStandBy) ;
	  break ;

	  case appState::appPhoto :
		  clearStupid () ;
		  if (mPhoto == nullptr) mPhoto = std::make_unique <unit::TPhoto> () ;
		  if (mSdio == nullptr) mSdio = std::make_unique <unit::TSdio> () ;				// По хорошему инициализацию FS нужно делать после получения картинки
		  if (mFileSystem == nullptr) mFileSystem = std::make_unique <unit::TFileSystem> () ;
		  mPhoto -> process() ;
		  while (mButton [idButton::btnPhoto].getState() == GPIO_PIN_SET) {
			  if (getState().second > unit::stPhotoTimeWait && getState().first == appState::appPhoto) {
				  setState(appState::appPhotoButtonPress) ;					// Если кнопка долго не отпускается, то выдаем сообщение об ошибке
				  makeInfo(typeInfo::infoAudioLight, tsndContinue, 0) ;
			  }
		  }
		  if (getState().first == appState::appPhoto) setState(appState::appStandBy) ;
	  break ;

	  case appState::appTag :
		  clearStupid () ;
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  if (mTag == nullptr) mTag = std::make_unique <unit::TTag> () ;
		  if (mTag -> process ()) {
			  mSdio = std::make_unique <unit::TSdio> () ;
			  mFileSystem = std::make_unique <unit::TFileSystem> () ;
			  if (getState ().first == appState::appTag) mFileSystem -> setTag() ;			// в методе setTag могут быть ошибки!!!
			  if (getState ().first == appState::appTag) setState(appState::appStandBy) ; 	// если ошибок нет, то переходим в StandBy, а если есть, то сначала их обрабатываем
		  }
	  break ;

	  case appState::appDoc : {
		  mStartDocMode = HAL_GetTick() ;
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
		  mSdio = std::make_unique <unit::TSdio> () ;
		  while (mButton [idButton::btnDoc].getState() == GPIO_PIN_SET) {
			  HAL_Delay(1000) ;
			  common::app -> debugMessage ("Time now: " + common::app -> getMessageTime ()) ;
		  }
		  setState(appState::appDocSyncTime) ;
	  }
	  break ;

	  case appState::appDocSyncTime :											// Сделано отдельное состояние, что бы его можно было без проблем перенести в любое место
		  mFileSystem = std::make_unique <unit::TFileSystem> () ;
		  if (getState().first != appErrFileFS) {
			  makeInfo(typeInfo::infoAudioLight, tsndShort, 1) ;
//			  mStartDocMode = (HAL_GetTick() - mStartDocMode) / 1000 ;			// Получаем время нахождения на докстанции в секундах
			  mFileSystem -> getTime ((HAL_GetTick() - mStartDocMode) / 1000) ;
		  }
		  if (getState ().first == appState::appDocSyncTime) setState(appState::appStandBy) ;
	  break ;

	  case appState::appDocStupid : {
		  uint32_t count { 0 } ;
		  count = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) ;
		  if (count > stAppStupidCount) {
			  makeInfo(typeInfo::infoAudio, tsndContinue, 0) ;
			  HAL_Delay(stAppStupidTimeout) ;
			  clearStupid () ;
			  setState(appState::appStandBy) ;
		  }
		    else {
			  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, count + 2) ;				// Добавляем 2, т.к. при запуске таймера тоже срабатывает прерывание
			  setState(appState::appDoc) ;
		    }
	  }
	  break;

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

	  case appState::appPhotoButtonPress :
	  case appState::appBounceTimeout:
	  case appState::appTagNoId :
	  case appState::appErrFileFS :
	  case appState::appErrButton :
	  case appState::appPhotoTimeout :
	  case appState::appAudioErr :
		  makeInfo(typeInfo::infoAudioLight, tsndShort, 3) ;
		  setState(appState::appStandBy) ;
	  break ;

	  default:
		  common::app -> debugMessage ("No command handler") ;
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
		debugMessage(inAppState) ;
		mAppState = inAppState ;
		mAppStateChange = HAL_GetTick() ;
		mLog -> pushItem(mAppState) ;
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
//	if (inTypeInfo == infoLight || inTypeInfo == infoAudioLight) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, inState) ;
}
//-----------------------------------------------------------
/*!
 * @param inMesage Текстовая строка которую нужно вывести в отладочный порт
 * @todo Переделать на хрен на потоковый вывод
 */
void TApplication::debugMessage (const std::string &inMesage)
{
	uint32_t timeStart = HAL_GetTick() ;
	while (HAL_UART_GetState (&huart1) == HAL_UART_STATE_BUSY_TX)
		if ((HAL_GetTick() - timeStart) > 1000) return ;
	common::stTempStr = (inMesage  + "\r\n") ;
	debugMessage (common::stTempStr.c_str(), inMesage.size() + 2) ;
}
//-----------------------------------------------------------
/*!
 * Данный метод выводит строку в отладочный порт
 * @param inMesage Указатель на строку, которую нужно вывести в отладочный порт
 * @param inSize Размер выводимого бувера
 */
void TApplication::debugMessage (const char *inMesage, const std::size_t inSize)
{
	debugMessage ((const uint8_t *)inMesage, inSize) ;
}
/*!-----------------------------------------------------------
 * @param inMesage Указатель на массив, который нужно вывести в отладочный порт
 * @param inSize Размер выводимого бувера
 * @todo Переделать на хрен на потоковый вывод
 * @attention На хрена я приделал callback я понятия не имею :(
 */
void TApplication::debugMessage (const uint8_t *inMesage, const std::size_t inSize)
{
	uint32_t timeStart = HAL_GetTick() ;
	while(HAL_UART_Transmit_IT(&huart1, (uint8_t *) inMesage, inSize) == HAL_BUSY ) {
		if ((HAL_GetTick() - timeStart) > stAppDebugTimeout) break ;	// что бы если что, то выйти из бесконечного цикла
	}
}
//-----------------------------------------------------------
void TApplication::debugMessage ()
{
	debugMessage (mAppState) ;
}
//-----------------------------------------------------------
/*!
 * @param inState Состояние из appState
 */
void TApplication::debugMessage (const appState inState)
{
	auto res = msgAppState.find (inState) ;
	if (res != msgAppState.end ()) debugMessage (res -> second) ;
	  else debugMessage ("Unknown breed beast") ;
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

	std::stringstream tempStr ;
	tempStr << std::dec << std::internal << std::setfill('0') << std::setw (2) << static_cast<uint32_t> (date.Date) << "."
															  << std::setw (2) << static_cast<uint32_t> (date.Month) << "."
																   << std::setw (2) << static_cast<uint32_t> (date.Year) << " "
																   << std::setw (2) << static_cast<uint32_t> (time.Hours) << ":"
																   << std::setw (2) << static_cast<uint32_t> (time.Minutes) << ":"
																   << std::setw (2) << static_cast<uint32_t> (time.Seconds) ;

	return tempStr.str () ;
}
//-----------------------------------------------------------
void TApplication::writePhoto ()
{
	mFileSystem -> writePhoto () ;
}
/*! -----------------------------------------------------------
 * \brief Перевод контроллера в режим StandBy
 * \todo Разобраться какие пины нужно переводить в "Аналог", а какие нет???
 */
void TApplication::sleep ()
{
	common::app -> debugMessage ("Standby mode") ;
	makeInfo(typeInfo::infoAudio, tsndShort, 1) ;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_All ;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	//		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	//		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitTypeDef GPIO_InitStructX = {0};
	GPIO_InitStructX.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
			|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
			|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
			|GPIO_PIN_3 |GPIO_PIN_1;
	GPIO_InitStructX.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructX.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructX);

	GPIO_InitTypeDef GPIO_InitStructA = {0};
	GPIO_InitStructA.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
			|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
			|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
			|GPIO_PIN_3 |GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_1 ;
	GPIO_InitStructA.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructA.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructA);

	GPIO_InitTypeDef GPIO_InitStructB = {0};
	GPIO_InitStructB.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
			|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
			|GPIO_PIN_11|GPIO_PIN_0|GPIO_PIN_13
			|GPIO_PIN_3 |GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_1 ;
	GPIO_InitStructB.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructB.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructB);

	GPIO_InitTypeDef GPIO_InitStructE = {0};
	GPIO_InitStructE.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
			|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
			|GPIO_PIN_11|GPIO_PIN_0|GPIO_PIN_13
			|GPIO_PIN_12 |GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_1 ;
	GPIO_InitStructE.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructE.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructE);

	HAL_PWR_DisableBkUpAccess();
	__HAL_RCC_PWR_CLK_ENABLE() ;
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1) ;
	HAL_PWR_EnterSTANDBYMode() ;
	HAL_Delay(100) ;
	common::app -> debugMessage ("Error Standby mode: " + common::app -> getMessageTime ()) ;	// Сюда мы попасть не должны ни когда
}
/*!-----------------------------------------------------------
 *
 */
void TApplication::writeLog ()
{
	mLog -> writeLog () ;
}
/*!-----------------------------------------------------------
 * Очистка регистра RTC_BKP_DR2.
 */
void TApplication::clearStupid ()
{
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, 0x0) ;
}
} /* namespace app */
