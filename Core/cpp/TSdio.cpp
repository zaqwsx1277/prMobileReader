/*
 * TSdio.cpp
 *
 *  Created on: 15 июл. 2021 г.
 *      Author: AAL
 */

#include <sdio.h>


#include <TSdio.hpp>
#include "TCommon.hpp"

namespace unit {

//----------------------------------------------
/*!
 * \attention Нет проверки инициализирована SD карточка или нет!!!
 */
TSdio::TSdio()
{
	if (HAL_SD_Init(&hsd) != HAL_OK)
		common::app -> setState(app::appState::appErrSDIO) ;
	  else common::app -> debugMesage ("SDIO initialization - OK.") ;
}
//----------------------------------------------
TSdio::~TSdio()
{
	HAL_SD_DeInit(&hsd) ;
	common::app -> debugMesage ("SDIO deinitialization - OK.") ;
}
//----------------------------------------------
/*!
 * @return Если все Ok, то возвращается true
 */
bool TSdio::check ()
{
	return true ;
}

//----------------------------------------------
} /* namespace unit */
