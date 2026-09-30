//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_TASKUI_H
#define GREENHOUSE_TASKUI_H
#include "Button.h"
#include "EEPROM.h"
#include "MemoryManager.h"
#include "ParentTask.h"
#include "OLEDDisplay.h"
#include  "RotaryEncoder.h"
#include "storage/SystemStorage.h"


#define SW0 9

enum class UIEvent {MENU, CO2_SETTING, SHOW_DATA};

class TaskUI : public ParentTask
{
	public:
		TaskUI (const std::shared_ptr<PicoI2C> &i2c_0, const std::shared_ptr<PicoI2C> &i2c_1, SystemStorage &storage)
			: ParentTask("UI Task", 2048,tskIDLE_PRIORITY + 1),
			  i2c_0(i2c_0),
			  i2c_1(i2c_1),
			  btn_sw(SW0),
			  storage(storage)

		{
			ui_queue = xQueueCreate(20, sizeof(int));
			configASSERT(ui_queue != NULL);
		};

		void task_runner() override;

		void init();

		void          handle_interaction();
		void          transition_to(UIEvent new_state);
		void          menu_interaction();
		void          co2_setting_interaction();
		void          set_co2(int change);
		void          show_data_interaction();
		QueueHandle_t get_queue_handle();

	private:
		QueueHandle_t ui_queue;
		std::unique_ptr<OLEDDisplay> oled;
		std::unique_ptr<Button> button;
		std::shared_ptr<MemoryManager> eeprom;
		SystemStorage &storage;
		SensorReading last_data{};

		std::shared_ptr<PicoI2C> i2c_0;
		std::shared_ptr<PicoI2C> i2c_1;
		int btn_sw;

		bool state_change = true;
		UIEvent current_state = UIEvent::MENU;
		int co2_setting_display = 1500;
};


#endif //GREENHOUSE_TASKUI_H