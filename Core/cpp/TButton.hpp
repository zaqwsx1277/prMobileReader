/*
 * TButton.h
 *
 *  Created on: Jul 2, 2021
 *      Author: AAL
 */

#ifndef CPP_TBUTTON_H_
#define CPP_TBUTTON_H_

#include <stdint.h>

#include <gpio.h>

namespace app {

/*!
 * \brief Класс для работы с кнопками
 */
class TButton {
	GPIO_TypeDef* mGpioPort { nullptr } ;	///< Порт к которому подключена кнопка
	uint16_t mGpio { 0 } ;					///< Номер GPIO к которому подключена кнопка
	GPIO_PinState mState { GPIO_PIN_RESET } ;///< Состояние кнопки. Состояние запоминается после полного устранения дребезга контактов
	uint16_t mBounce { GPIO_PIN_RESET } ;	///< Служебная переменная для устранения дребезга контактов.
public:
	void startBounce () ;					///< Запуск процедуры устранения дребезга
	bool checkBounce () ;					///< Проверка устранения дребезга контактов
	GPIO_PinState getState () ;				///< Получение состояния кнопки.

	TButton(GPIO_TypeDef*, uint16_t) ;
	TButton() = default ;
	virtual ~TButton() = default ;
};

} /* namespace app */

#endif /* CPP_TBUTTON_H_ */
