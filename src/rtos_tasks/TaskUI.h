//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_TASKUI_H
#define GREENHOUSE_TASKUI_H
#include "Button.h"
#include "EEPROM.h"
#include "event_groups.h"
#include "MemoryManager.h"
#include "ParentTask.h"
#include "OLEDDisplay.h"
#include  "RotaryEncoder.h"
#include "storage/SystemStorage.h"


#define SW0 9



class TaskUI : public ParentTask
{
	public:
		TaskUI (const std::shared_ptr<MemoryManager> &eeprom, const std::shared_ptr<PicoI2C> &i2c_1, SystemStorage &storage, EventGroupHandle_t event_group,  TaskHandle_t cloud)
			: ParentTask("UI Task", 2048,tskIDLE_PRIORITY + 1),
			  eeprom(eeprom),
			  i2c_1(i2c_1),
			  btn_sw(SW0),
			  storage(storage),
		event_grp(event_group),
		tsk_cloud_handle(cloud)
		{
			ui_queue = xQueueCreate(20, sizeof(int));
			configASSERT(ui_queue != NULL);

			input_queue = xQueueCreate(20, sizeof(int));
			configASSERT(ui_queue != NULL);
		};

		void task_runner() override;

		void boot();

		void update_network_credentials(WiFiStatus current_state);

		void handle_wifi_animation(WiFiStatus current_state);

		void handle_state();


		//Handle interaction
		bool handle_encoder(int encoder);
		void handle_interaction();
		void menu_interaction();
		void show_data_interaction();
		void new_network_interaction();
		void known_network_interaction();
		void factory_reset_interaction();
		void co2_setting_interaction();

		//Handle and execute state
		void check_next_state(int menu_select);
		void transition_to(UIEvent new_state);

		//Handle user input
		void reset_input_buffer();
		void exit_text_entry(UIEvent next_state);
		bool process_text_input(char ch, bool allow_alpha);

		void ui_reset();

		QueueHandle_t get_ui_queue_handle();
		QueueHandle_t get_input_queue_handle();

	private:
		QueueHandle_t ui_queue;
		QueueHandle_t input_queue;

		std::unique_ptr<OLEDDisplay> oled;
		std::unique_ptr<Button> button;
		std::shared_ptr<MemoryManager> eeprom;
		SystemStorage &storage;
		SensorReading last_data{};

		std::shared_ptr<PicoI2C> i2c_0;
		std::shared_ptr<PicoI2C> i2c_1;
		int btn_sw;

		EventGroupHandle_t event_grp;
		bool state_change = true;
		bool clear_oled = false;
		UIEvent current_state = UIEvent::MENU;
		int co2_setting_display = 1500;

		//Network
		TaskHandle_t tsk_cloud_handle;
		NetworkParam current_network_input = NetworkParam::SSID;
		char ssid_input[20];
		char pwd_input[20];


		//Input
		TaskHandle_t tsk_console_handle;
		char input_buffer[20];
		int input_count = 0;
};


#endif //GREENHOUSE_TASKUI_H