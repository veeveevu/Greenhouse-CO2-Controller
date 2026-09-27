//
// Created by Anh Huynh on 23.9.2026.
//

#include "TaskUI.h"

void TaskUI::task_runner()
{
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
	if (current_state == UIEvent::MENU)
	{
		menu_interaction();
	}
	if (current_state == UIEvent::CO2_SETTING)
	{
		co2_setting_interaction();
	}
	if (current_state == UIEvent::SHOW_DATA)
	{
		show_data_interaction();
	}

}

void TaskUI::menu_interaction()
{
	int isr_receive;
	if (xQueueReceive(ui_queue, &isr_receive,0) == pdPASS)
	{
		if (isr_receive == 1)
		{
			oled.increment_menu_select();
		}
		else if (isr_receive == -1)
		{
			oled.decrement_menu_select();
		}
		else if (isr_receive == 0)
		{
			if (oled.get_current_select() == 1)
			{
				oled.clear();
				current_state = UIEvent::SHOW_DATA;
			}
			else if (oled.get_current_select() == 2)
			{
				oled.clear();
				current_state = UIEvent::CO2_SETTING;
			}
		}
	}
	else if (button.is_pressed())
	{
		current_state = UIEvent::MENU;
		oled.clear();
	}
}

void TaskUI::co2_setting_interaction()
{
	int isr_receive;
	int temp_co2_setting = co2_setting_display;
	if (xQueueReceive(ui_queue, &isr_receive,0) == pdPASS)
	{
		if (button.is_pressed())
		{
			//Return without any change
			current_state = UIEvent::MENU;
		}
		else if (isr_receive == 1)
		{
			co2_setting_display += 10;
			oled.clear_co2_display();
		}
		else if (isr_receive == -1)
		{
			co2_setting_display -= 10;
			oled.clear_co2_display();
		}
		else if (isr_receive == 0)
		{
			//Save to EEPROM
			//Send to controller
			//Go back to main screen - menu
			oled.clear();
			current_state = UIEvent::MENU;
		}
	}
	else if (button.is_pressed())
	{
		oled.clear();
		co2_setting_display = temp_co2_setting; //Reset co2 setting because it was not saved
		current_state = UIEvent::MENU;
	}
}

void TaskUI::show_data_interaction()
{
	if (button.is_pressed())
	{
		oled.clear();
		current_state = UIEvent::MENU;
	}
}

QueueHandle_t TaskUI::get_queue_handle()
{
	return ui_queue;
}
