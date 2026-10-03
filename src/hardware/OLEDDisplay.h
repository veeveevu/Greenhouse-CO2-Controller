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
		OLEDDisplay(std::shared_ptr<PicoI2C> i2c_1);
		OLEDDisplay(const OLEDDisplay &) = delete;

		void clear();

		void show_options(int option_length, std::vector<std::string> options);

		void show_menu();

		void show_network_setting();

		void increment_menu_select(UIEvent current_state);
		void decrement_menu_select();
		void change_co2_setting(int co2_setting);

		void clear_co2_display();

		void show_data(float co2_level, float temperature, float humidity, float pressure, float co2_setting);
		int  get_current_select() const;

	private:
		std::shared_ptr<PicoI2C> oled_i2c;
		ssd1306os display;

		//Menu
		int current_select = 1;

		menuItem main_menu{
		"Main menu",
		{"1. View sensors data", "2. CO2 setting", "3. Network settings", "4. Factory reset"},
		4};

		menuItem network_menu{
		"Network menu",
		{"1. Connect to new network", "Connect to known network"},
		2};
};

#endif //GREENHOUSE_OLEDDISPLAY_H