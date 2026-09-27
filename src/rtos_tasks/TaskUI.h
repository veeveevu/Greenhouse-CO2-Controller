//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_TASKUI_H
#define GREENHOUSE_TASKUI_H
#include "Button.h"
#include "GenericTaskHandler.h"
#include "OLEDDisplay.h"
#include  "RotaryEncoder.h"



#define SW0 9

enum class UIEvent {MENU, CO2_SETTING, SHOW_DATA};

class TaskUI : public GenericTaskHandler
{
	public:
		TaskUI ()
		: GenericTaskHandler("UI Task",2048,tskIDLE_PRIORITY + 1),
		oled(),
		button(SW0)
		{
			ui_queue = xQueueCreate(20, sizeof(int));
			configASSERT(ui_queue != NULL);
		};
		void task_runner() override;
		void handle_interaction();
		void menu_interaction();
		void co2_setting_interaction();
		void show_data_interaction();
		QueueHandle_t get_queue_handle();

	private:
		QueueHandle_t ui_queue;
		OLEDDisplay oled;
		Button button;

		UIEvent current_state = UIEvent::MENU;
		int co2_setting_display = 1500;
};


#endif //GREENHOUSE_TASKUI_H