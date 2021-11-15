/*
 * TCommon.cpp
 *
 *  Created on: Jul 1, 2021
 *      Author: AAL
 */

#include "TCommon.hpp"

 bool tmpFlag { false } ;
 uint32_t tmpCount { 0 } ;
 uint8_t stPhotoBuf [1600*80] {0} ;

namespace common {

std::unique_ptr <app::TApplication> app { nullptr } ;	///< Указатель на данные для обеспечения работы приложения
std::string stTempStr { "" } ;
std::string stSyncTimeFileName { "time" } ;
std::string stTagFileName { "tags.tag" } ;
std::string stPhotoFileName { "photos.pho" } ;
std::string stAudioFileName { "audios.aud" } ;

uint8_t stTagBufId [25] { 0 } ;
uint8_t stFileBuf [512] { 0 } ;
uint32_t stTimeStartDebugMessage { 0 } ;
// uint8_t stPhotoBuf [1600*40] {0} ;

tdAudioFrame stAudioBuf [2][unit::stAudioBufSize] { 0 } ; 	///< Буфер для записи данных с микрофона.
unit::crAudioBufID stAudioBufId { unit::crAudioBufID::crStop } ;

//const uint32_t *stPrtSN { ;

}
