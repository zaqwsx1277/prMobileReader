/*!
 * TSensor.hpp
 *
 *  \date Jan 28 2021
 *  \author AAL
 */

#ifndef TSENSOR_HPP_
#define TSENSOR_HPP_

#include <stdint.h>

namespace sensor {

enum tpState { stUnknown, stOk, stResdy, stErr } ; 	///< Возможные состояния при работе с датчиками

constexpr uint32_t defTimeout { 100 } ;				///< Таймаут ответа датчика
/*!
 * \brief Базовый класс для работы с различными датчиками
 */
class TSensor {

protected :
	tpState fState { stUnknown } ;			///< Текущее состояние датчика
	uint32_t fTimeout { defTimeout } ;		///< Таймоут ответа датчика

public:
	TSensor() = default ;
	~TSensor() = default ;

	virtual tpState checkSate () = 0  ;		///< Проверка состояния датчика
	virtual tpState getState () = 0 ;		///< Получение состояния датчика
};

} /* namespace sensor */

#endif /* TSENSOR_HPP_ */
