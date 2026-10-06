//
// Created by Anh Huynh on 23.9.2026.
//

#include "TaskUI.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sys/stat.h>
#define CO2_UPPER_LIM 1500
#define CO2_CHANGE 10
#define CO2_LOWER_LIM 0

namespace EncoderEvent
{
	constexpr int CLOCKWISE = 1;
	constexpr int ANTI_CLOCKWISE = -1;
	constexpr int PRESS = 0;
}

namespace MainMenuItem
{
	constexpr  int VIEW_SENSORS = 1;
	constexpr  int SET_CO2 = 2;
	constexpr  int SET_NETWORK = 3;
	constexpr  int FACTORY_RESET = 4;
}

namespace NetworkItem
{
	constexpr  int NEW_NETWORK = 1;
	constexpr  int KNOWN_NETWORK = 2;
}
void TaskUI::task_runner()
{
	boot();

	while (true)
	{
		WiFiStatus current_wifi_state = storage.get_wifi_status();


		//Main function of taskUI
		if (current_wifi_state != WiFiStatus::IDLE && current_wifi_state != WiFiStatus::CONNECTED)
		{
			//Only run when attempt to connect Wifi
			handle_wifi_animation(current_wifi_state);
			update_network_credentials(current_wifi_state);
		}
		else
		{
			handle_state();
			handle_interaction();
		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

void TaskUI::boot()
{
	//Init
	oled = std::make_unique<OLEDDisplay>(i2c_1);
	eeprom = std::make_shared<MemoryManager>(i2c_0);
	button = std::make_unique<Button>(btn_sw);
	input_buffer[0] = '\0';


	//Read from EEPROM and save
	eeprom->save_new_co2_setting(1500);
	eeprom->read_co2_setting(reinterpret_cast<uint8_t *>(&co2_setting_display));
	printf("EEPROM read, co2 set: %d\n",co2_setting_display);
	storage.set_co2_point(co2_setting_display);
	printf("Storage co2 set: %d\n", storage.get_data().co2_set_point);

	//Check old network
	int network_setting_count = eeprom->read_network_setting(ssid_input, pwd_input);
	if (network_setting_count == 2) //If the saved wifi is valid
	{
		storage.update_network(ssid_input, pwd_input);
		xEventGroupSetBits(event_grp,WIFI_RECONNECT_BIT);
	}
}

void TaskUI::update_network_credentials(WiFiStatus current_wifi_state)
{
	switch (current_wifi_state)
	{
		case WiFiStatus::CONNECTING:
			//New attempt to connect to wifi
			strcpy(ssid_input,storage.get_network_settings().ssid);
			strcpy(pwd_input, storage.get_network_settings().pwd);
			break;
		case WiFiStatus::CONNECT_SUCCESS:
			eeprom->save_network_setting(ssid_input,pwd_input);
			break;
		case WiFiStatus::CONNECT_FAIL:
			ssid_input[0] = '\0';
			pwd_input[0] = '\0';
			break;
		case WiFiStatus::IDLE:
		case WiFiStatus::CONNECTED:
			break;
	}
}

void TaskUI::handle_wifi_animation(WiFiStatus current_wifi_state)
{
	switch (current_wifi_state)
	{
		case WiFiStatus::CONNECTING:
			oled->connecting_animation(ssid_input);
			oled->clear();
			state_change = true;
			break;
		case WiFiStatus::CONNECT_SUCCESS:
			oled->connect_successfully(ssid_input);
			oled->clear();
			storage.update_wifi_status(WiFiStatus::CONNECTED);
			break;
		case WiFiStatus::CONNECT_FAIL:
			oled->connect_failed();
			oled->clear();
			storage.update_wifi_status(WiFiStatus::IDLE);
			break;
		case WiFiStatus::CONNECTED:
			break;
		case WiFiStatus::IDLE:
			break;
	}
}
void TaskUI::handle_state()
{
	if (state_change || current_state == UIEvent::SHOW_DATA)
	{
		switch (current_state)
		{
			case UIEvent::MENU:
				oled->show_menu();
				break;
			case UIEvent::CO2_SETTING:
				oled->change_co2_setting(input_buffer,co2_setting_display);
				break;
			case UIEvent::SHOW_DATA:
			{
				SensorReading data = storage.get_data();
				if (data!=last_data)
				{
					oled->show_data(data.co2_level_ppm, data.temp_celsius, data.humidity_percent, data.pressure_pa, data.co2_set_point);
					last_data = data;
				}
				break;
			}
			case UIEvent::NETWORK:
			{
				std::string status = storage.get_wifi_status() == WiFiStatus::CONNECTED ? "Connected" : "Not connected";
				oled->show_network(storage.get_network_settings().ssid, status.c_str());
				break;
			}
			case UIEvent::NEW_NETWORK:
				oled->connect_new_network(current_network_input,ssid_input, pwd_input, input_buffer);
				break;
			case UIEvent::RESET:
				oled->show_reset(input_buffer);
				break;
			default:
				break;
		}
		state_change = false;
	}
}


void TaskUI::handle_interaction()
{
	switch (current_state)
	{
		case UIEvent::MENU:
			menu_interaction();
			break;
		case UIEvent::NETWORK:
			menu_interaction();
		case UIEvent::CO2_SETTING:
			co2_setting_interaction();
			break;
		case UIEvent::NEW_NETWORK:
			new_network_interaction();
			break;
		case UIEvent::SHOW_DATA:
			show_data_interaction();
			break;
		case UIEvent::RESET:
			factory_reset_interaction();
			break;
	}
}

void TaskUI::transition_to(UIEvent new_state)
{
	oled->clear();
	current_state = new_state;
	state_change = true;

	if ( new_state == UIEvent::CO2_SETTING || new_state == UIEvent::KNOWN_NETWORK || new_state == UIEvent::RESET || new_state == UIEvent::NEW_NETWORK)
	{
		reset_input_buffer(); // Guarantee input_count = 0 and input_buffer[0] = '\0'
		xEventGroupSetBits(event_grp, CONSOLE_ACTIVATE_BIT); // Wake up TaskConsole
	}
	if (new_state == UIEvent::NEW_NETWORK)
	{
		current_network_input = NetworkParam::SSID;
	}
}

void TaskUI::menu_interaction()
{
	int encoder_receive;
	if (xQueueReceive(ui_queue, &encoder_receive,pdMS_TO_TICKS(10)) == pdPASS)
	{
		if (bool encoder_pressed = handle_encoder(encoder_receive))
		{
			transition_to(current_state);
		}
	}
	else if (button->is_pressed())
	{
		transition_to(UIEvent::MENU);
	}
}


bool TaskUI::handle_encoder(int encoder_receive)
{
	bool encoder_pressed = false;
	switch (encoder_receive)
	{
		case EncoderEvent::CLOCKWISE:
			oled->increment_menu_select(current_state);
			state_change = true;
			break;
		case EncoderEvent::ANTI_CLOCKWISE:
			oled->decrement_menu_select();
			state_change = true;
			break;
		case EncoderEvent::PRESS:
			int select = oled->get_current_select();

			//Reset menu parameters
			oled->reset_menu_select();
			encoder_pressed = true;

			//Update the state based on selection
			check_next_state(select);
			break;
	}
	return encoder_pressed;
}

void TaskUI::check_next_state(int menu_select)
{
	if (current_state == UIEvent::MENU)
	{
		switch (menu_select)
		{
			case MainMenuItem::VIEW_SENSORS:
				current_state = UIEvent::SHOW_DATA;
				break;
			case MainMenuItem::SET_CO2:
				current_state = UIEvent::CO2_SETTING;
				break;
			case MainMenuItem::SET_NETWORK:
				current_state = UIEvent::NETWORK;
				break;
			case MainMenuItem::FACTORY_RESET:
				current_state = UIEvent::RESET;
				break;
		}
	}
	else if (current_state == UIEvent::NETWORK)
	{
		switch (menu_select)
		{
			case NetworkItem::NEW_NETWORK:
				current_state = UIEvent::NEW_NETWORK;
				break;
			case NetworkItem::KNOWN_NETWORK:
				current_state = UIEvent::KNOWN_NETWORK;
				break;
		}
	}
}

void TaskUI::co2_setting_interaction()
{
	char char_received;
	int temp_co2_setting = co2_setting_display;
	if (xQueueReceive(input_queue,&char_received,0))
	{
		bool enter_pressed = process_text_input(char_received, false);
		if (enter_pressed)
		{
			if (input_count > 0)
			{
				char *endptr = nullptr;
				int value = std::strtol(input_buffer, &endptr, 10);
				if (value >= CO2_LOWER_LIM && value <= CO2_UPPER_LIM)
				{
					co2_setting_display = value;
					eeprom->save_new_co2_setting(value);
					oled->confirm_co2_setting(value);
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
				else
				{
					oled->no_new_setting();
					vTaskDelay(pdMS_TO_TICKS(1000));
				}
			}
			exit_text_entry(UIEvent::MENU);
		}
	}
	else if (button->is_pressed())
	{
		co2_setting_display = temp_co2_setting; //Reset co2 setting because it was not saved
		oled->no_new_setting();
		vTaskDelay(pdMS_TO_TICKS(1000));
		exit_text_entry(UIEvent::MENU);
	}
}

void TaskUI::reset_input_buffer()
{
	input_count = 0;
	input_buffer[0] = '\0';
}

void TaskUI::exit_text_entry(UIEvent next_state)
{
	reset_input_buffer();
	xEventGroupClearBits(event_grp,CONSOLE_ACTIVATE_BIT);
	transition_to(next_state);
}

bool TaskUI::process_text_input(char ch, bool allow_alpha)
{
	if (ch == '\n')
	{
		return true;
	}

	if (ch == '\r')
	{
		return false;
	}
	if ((ch == '\b' || ch == 127) && input_count > 0)
	{
		input_buffer[--input_count] = '\0';
		state_change = true;
		return false;
	}

	bool is_valid = allow_alpha ? true : std::isdigit(ch);
	if (is_valid && input_count < 14)
	{
		input_buffer[input_count++] = ch;
		input_buffer[input_count] = '\0';
		state_change = true;
		return false;
	}
	return false;
}


void TaskUI::show_data_interaction()
{
	int encoder_receive;
	xQueueReceive(ui_queue, &encoder_receive,0);
	if (button->is_pressed())
	{
		transition_to(UIEvent::MENU);
	}
}

void TaskUI::factory_reset_interaction()
{
	char rx_char;
	if (xQueueReceive(input_queue, &rx_char,pdMS_TO_TICKS(10)))
	{
		bool enter_pressed = process_text_input(rx_char, true);
		if (enter_pressed)
		{
			if (strcmp(input_buffer,"reset") == 0)
			{
				storage.factory_reset();
				ui_reset();
			}
			else
			{
				oled->wrong_command();
				vTaskDelay(pdMS_TO_TICKS(1000));
				exit_text_entry(UIEvent::MENU);
			}
		}
	}
	else if (button->is_pressed())
	{
		exit_text_entry(UIEvent::MENU);
	}
}

void TaskUI::new_network_interaction()
{
	char rx_char;
	int encoder_receiver;
	bool encoder_pressed = handle_encoder(encoder_receiver);

	if (xQueueReceive(input_queue, &rx_char,pdMS_TO_TICKS(10)))
	{
		bool enter_pressed = process_text_input(rx_char, true);
		if (enter_pressed)
		{
			if (current_network_input == NetworkParam::SSID)
			{
				std::strcpy(ssid_input, input_buffer);
				reset_input_buffer();
				current_network_input = NetworkParam::PASSWORD;
				state_change = true;
			}
			else if (current_network_input == NetworkParam::PASSWORD)
			{
				std::strcpy(pwd_input, input_buffer);
				reset_input_buffer();
				current_network_input = NetworkParam::DONE;
				state_change = true;

				//Update SSID and PWD - then set bit in event group
				storage.update_network(ssid_input, pwd_input);
				storage.update_wifi_status(WiFiStatus::CONNECTING);
				xEventGroupSetBits(event_grp,WIFI_RECONNECT_BIT);

				exit_text_entry(UIEvent::MENU);
				current_network_input = NetworkParam::SSID;
			}
		}
	}
	else if (encoder_pressed && current_network_input == NetworkParam::DONE)
	{
		oled->connecting_animation(ssid_input);
		oled->connect_successfully(ssid_input);
		exit_text_entry(UIEvent::MENU);
	}
	else if (button->is_pressed())
	{
		exit_text_entry(UIEvent::MENU);
	}
}


void TaskUI::ui_reset()
{
	input_count = 0;
	input_buffer[0] = '\0';
	co2_setting_display = 1500;
	eeprom->save_new_co2_setting(1500);
	eeprom->reset_network_setting();
	transition_to(UIEvent::MENU);
}

QueueHandle_t TaskUI::get_ui_queue_handle()
{
	return ui_queue;
}

QueueHandle_t TaskUI::get_input_queue_handle()
{
	return input_queue;
}
