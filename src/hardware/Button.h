//
// Created by Anh Huynh on 26.9.2026.
//

#ifndef GREENHOUSE_BUTTON_H
#define GREENHOUSE_BUTTON_H
#include "FreeRTOS.h"
#include "task.h"
#include "GPIOPin.h"
#include "hardware/gpio.h"
#include "pico/types.h"


class Button
{
	public:
		Button(const int btn_pin) : btn(btn_pin, true, true, true){};
		bool is_pressed();
	private:
		GPIOPin btn;
		bool last_btn_state = false;
		uint32_t last_press_time = 0;

};




#endif //GREENHOUSE_BUTTON_H