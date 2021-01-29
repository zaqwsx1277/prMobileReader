/*
 * TAppError.cpp
 *
 *  Created on: Jan 11, 2021
 *      Author: energia
 */

#include <TAppError.hpp>
#include <TCommon.hpp>

namespace appError {

//-------------------------------------------------------
TAppError::TAppError() {
	// TODO Auto-generated constructor stub

}
//-------------------------------------------------------
TAppError::~TAppError() {
	// TODO Auto-generated destructor stub
}

//-------------------------------------------------------
void TAppError::read ()
{
	uint32_t addr = common::defFlashAddr ;
	for (uint16_t i = 0; i < teCount; ++i) {
		fError [i].fCount = *((uint32_t *) addr) ;
		addr += (sizeof (uint32_t)) ;
	}
}
//-------------------------------------------------------
void TAppError::write ()
{

}
//-------------------------------------------------------
void TAppError::setError (const typeError inTypeError)
{
	fError [inTypeError].fCount++ ;
	HAL_RTC_GetTime(&hrtc, &fError [inTypeError].fTime, RTC_FORMAT_BIN) ;
	HAL_RTC_GetDate(&hrtc, &fError [inTypeError].fDate, RTC_FORMAT_BIN) ;
	fError [inTypeError].isWrite = false ;
}
//-------------------------------------------------------
void TAppError::getError (const typeError intypeError) const
{

}
//-------------------------------------------------------
} /* namespace appError */
