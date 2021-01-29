/*
 * TSensorAudio.hpp
 *
 *  Created on: Jan 28, 2021
 *      Author: energia
 */

#ifndef TSENSORAUDIO_HPP_
#define TSENSORAUDIO_HPP_

#include "i2s.h"

#include <TSensor.hpp>

namespace sensor {

class TSensorAudio: public TSensor {
private:
	I2S_HandleTypeDef *ptrHandle { &hi2s3 } ;
public:
	TSensorAudio();
	virtual ~TSensorAudio();

	tpState checkSate () ;
	tpState getState () ;
};

} /* namespace sensor */

#endif /* TSENSORAUDIO_HPP_ */
