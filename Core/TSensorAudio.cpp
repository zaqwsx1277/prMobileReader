/*
 * TSensorAudio.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: energia
 */

#include <TSensorAudio.hpp>

namespace sensor {
//---------------------------------------------------------
TSensorAudio::TSensorAudio() {
	fState = stOk ;
}
//---------------------------------------------------------
TSensorAudio::~TSensorAudio() {
	// TODO Auto-generated destructor stub
}
//---------------------------------------------------------
tpState TSensorAudio::checkSate ()
{

	return fState ;
}
//-------------------------------------------------------------
tpState TSensorAudio::getState ()
{
	return fState ;
}
//-------------------------------------------------------------
} /* namespace sensor */
