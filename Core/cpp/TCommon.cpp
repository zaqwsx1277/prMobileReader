/*
 * TCommon.cpp
 *
 *  Created on: Jul 1, 2021
 *      Author: energia
 */

#include "TCommon.hpp"

namespace common {

std::unique_ptr <app::TApplication> app { nullptr } ;	///< Указатель на данные для обеспечения работы приложения
std::string stTempStr { "" } ;
std::string stSyncTimeFileName { "time" } ;
std::string stTagFileName { "tags.tag" } ;
std::string stPhotoFileName { "photos.pho" } ;

uint8_t stTagBufId [25] { 0 } ;
uint8_t stFileBuf [512] { 0 } ;
uint8_t stPhotoBuf [1600*94] {0} ;

}
