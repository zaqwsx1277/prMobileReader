/*
 * TAudio.cpp
 *
 *  Created on: Jul 7, 2021
 *      Author: energia
 */

#include <TAudio.hpp>
#include <TFileSystem.hpp>
#include <TCommon.hpp>

#include <i2s.h>
#include "fatfs.h"

namespace unit {
//--------------------------------------------------------
TAudio::TAudio() {

	common::app -> debugMesage("Write Audio start - OK") ;
}
//--------------------------------------------------------
TAudio::~TAudio() {

	common::app -> debugMesage("Write Audio stop - OK") ;
}
//--------------------------------------------------------
bool TAudio::check ()
{
	return true ;
}
//--------------------------------------------------------
} /* namespace app */
