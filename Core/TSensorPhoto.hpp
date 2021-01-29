/*
 * TSensorPhoto.hpp
 *
 *  Created on: Jan 28, 2021
 *      Author: energia
 */

#ifndef TSENSORPHOTO_HPP_
#define TSENSORPHOTO_HPP_

#include "i2c.h"

#include <TSensor.hpp>

namespace sensor {

constexpr uint8_t defAddress = 0x30 ;					///< Адрес видео камеры
constexpr uint8_t defAddressRead = (defAddress << 1) + 1 ;
constexpr uint8_t defAddressWrite = (defAddress << 1) ;

class TSensorPhoto: public TSensor {
private:
	I2C_HandleTypeDef *ptrHandle { &hi2c2 } ;
public:
	TSensorPhoto();
	virtual ~TSensorPhoto() = default ;

	tpState checkSate () ;
	tpState getState () ;
};

} /* namespace sensor */

#endif /* TSENSORPHOTO_HPP_ */
