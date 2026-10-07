//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_OLEDDISPLAY_H
#define GREENHOUSE_OLEDDISPLAY_H


#include <string>
#include <memory>
#include <vector>

#include "ssd1306os.h"
#include "PicoI2C.h"
#include "UIEvent.h"

struct menuItem
{
	std::string name;
	std::vector<std::string> menu_options;
	int menu_length;
};

class OLEDDisplay {
	public:
		OLEDDisplay(const std::shared_ptr<PicoI2C> &i2c_1);
		OLEDDisplay(const OLEDDisplay &) = delete;

		void clear();

		void show_options(int option_length, std::vector<std::string> options,  int &x, int &y);

		void show_menu();

		void show_network_connect_options();

		void increment_menu_select(UIEvent current_state);
		void decrement_menu_select();

		void reset_menu_select();

		//void change_co2_setting(int co2_setting);
		void change_co2_setting(const char *co2_input, int current_set_point);

		void confirm_co2_setting(int new_co2_setting);

		void no_new_setting();

		void show_input(char *input, int line_position);

		void clear_co2_display();

		void show_data(float co2_level, float temperature, float humidity, float pressure, int co2_setting);

		void show_network(const char* ssid, const char* status);

		void connect_new_network(NetworkParam current_param, const char* ssid_input, const char* pwd_input, const char* input_buffer);

		void connecting_animation(const char* ssid);

		void connect_successfully(const char* ssid);

		void connect_failed();

		int  get_current_select() const;

		void show_reset(const char *input);

		void confirm_reset();

		void wrong_command();

	private:
		std::shared_ptr<PicoI2C> oled_i2c;
		ssd1306os display;

		//Menu
		int current_select = 1;
		int x_pointer = 5;
		int y_pointer = 11;
		menuItem main_menu{
		"Main menu",
		{"1. View sensors data", "2. CO2 setting", "3. Network", "4. Factory reset"},
		4};

		menuItem network_menu{
		"Network menu",
		{"New network"},
		1};


};

#endif //GREENHOUSE_OLEDDISPLAY_H