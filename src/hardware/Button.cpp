//
// Created by Anh Huynh on 26.9.2026.
//

#include "Button.h"


bool Button::is_pressed()
{
	bool current_btn_state = btn.read();
	TickType_t current_time = xTaskGetTickCount();

	if (current_btn_state && !last_btn_state)
	{
		if (current_time - last_press_time >= pdMS_TO_TICKS(50))
		{
			last_press_time = current_time;
			last_btn_state = true;
			return true;
		}
	}
	if (!current_btn_state)
	{
		if ((current_time - last_press_time) >= pdMS_TO_TICKS(50))
		{
			last_btn_state = false;
		}
	}
	return false;
}