//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_OLEDDISPLAY_H
#define GREENHOUSE_OLEDDISPLAY_H


#include <string>
#include <memory>
#include "ssd1306os.h"
#include "PicoI2C.h"
#include "PicoI2C.h"

class OLEDDisplay {
	public:
		OLEDDisplay();
		OLEDDisplay(const OLEDDisplay &) = delete;

		void clear();
		void show_menu();
		void increment_menu_select();
		void decrement_menu_select();
		void change_co2_setting(int co2_setting);

		void clear_co2_display();

		void show_data();
		int  get_current_select() const;

	private:
		std::shared_ptr<PicoI2C> oled_i2c;
		ssd1306os display;

		//Menu
		int current_select = 2;
		int menu_length = 2;
		std::string menu[2] = {"1. View sensors data", "2. CO2 setting"};


};

#endif //GREENHOUSE_OLEDDISPLAY_H