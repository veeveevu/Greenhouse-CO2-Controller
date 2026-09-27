//
// Created by Anh Huynh on 23.9.2026.
//

#include "TaskUI.h"
#define CO2_UPPER_LIM 1500
#define CO2_CHANGE 10



namespace EncoderEvent
{
	constexpr int CLOCKWISE = 1;
	constexpr int ANTI_CLOCKWISE = -1;
	constexpr int PRESS = 0;
}

void TaskUI::task_runner()
{
	//Read from EEPROM and save
	eeprom.read_co2_setting(reinterpret_cast<uint8_t *>(&co2_setting_display));
	storage.set_co2_point(co2_setting_display);

	while (true)
	{
		switch (current_state)
		{
			case UIEvent::MENU:
				printf("Show menu\n");
				oled.show_menu();
				break;
			case UIEvent::CO2_SETTING:
				oled.change_co2_setting(co2_setting_display);
				break;
			case UIEvent::SHOW_DATA:
				oled.show_data();
				break;
		}
		handle_interaction();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void TaskUI::handle_interaction()
{
	switch (current_state)
	{
		case UIEvent::MENU:
			menu_interaction();
			break;
		case UIEvent::CO2_SETTING:
			co2_setting_interaction();
			break;
		case UIEvent::SHOW_DATA:
			show_data_interaction();
			break;
	}
}

void TaskUI::transition_to(UIEvent new_state)
{
	oled.clear();
	current_state = new_state;
}

void TaskUI::menu_interaction()
{
	int encoder_receive;
	if (xQueueReceive(ui_queue, &encoder_receive,0) == pdPASS)
	{
		switch (encoder_receive)
		{
			case EncoderEvent::CLOCKWISE:
				oled.increment_menu_select();
				break;
			case EncoderEvent::ANTI_CLOCKWISE:
				oled.decrement_menu_select();
				break;
			case EncoderEvent::PRESS:
				current_state = oled.get_current_select() == 1 ? UIEvent::SHOW_DATA : UIEvent::CO2_SETTING;
				transition_to(current_state);
				break;
			default:
				break;
		}
	}
	else if (button.is_pressed())
	{
		transition_to(UIEvent::MENU);
	}
}

void TaskUI::co2_setting_interaction()
{
	int encoder_receive;
	int temp_co2_setting = co2_setting_display;
	if (xQueueReceive(ui_queue, &encoder_receive,0) == pdPASS)
	{
		switch (encoder_receive)
		{
			case EncoderEvent::CLOCKWISE:
				set_co2(CO2_CHANGE);
				oled.clear_co2_display();
				break;
			case EncoderEvent::ANTI_CLOCKWISE:
				set_co2(-CO2_CHANGE);
				oled.clear_co2_display();
				break;
			case EncoderEvent::PRESS:
				//Save to EEPROM
				eeprom.save_new_co2_setting(co2_setting_display);
				//Send to controller
				//Go back to main screen - menu
				transition_to(UIEvent::MENU);
				break;
			default:
				break;
		}
	}
	else if (button.is_pressed())
	{
		co2_setting_display = temp_co2_setting; //Reset co2 setting because it was not saved
		transition_to(UIEvent::MENU);
	}
}
void TaskUI::set_co2(int change)
{
	if (change > 0 && co2_setting_display <= CO2_UPPER_LIM - CO2_CHANGE)
	{
		co2_setting_display += CO2_CHANGE;
	}
	else if (change < 0 && co2_setting_display >= CO2_CHANGE)
	{
		co2_setting_display -= CO2_CHANGE;
	}
	storage.set_co2_point(co2_setting_display);
}

void TaskUI::show_data_interaction()
{
	if (button.is_pressed())
	{
		transition_to(UIEvent::MENU);
	}
}

QueueHandle_t TaskUI::get_queue_handle()
{
	return ui_queue;
}
