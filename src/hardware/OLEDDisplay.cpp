//
// Created by Anh Huynh on 23.9.2026.
//

#include "OLEDDisplay.h"

OLEDDisplay::OLEDDisplay(std::shared_ptr<PicoI2C> i2c_1) : oled_i2c(i2c_1), display(oled_i2c)
{
}

void OLEDDisplay::clear() {
	display.fill(0);
	display.show();
}
void OLEDDisplay::show_options(int option_length, std::vector<std::string> options)
{
	int x = 0;
	int y = 1;

	int num_visible_line = 5;
	int i = 0;
	for ( ; i < option_length; i++)
	{
		//Highlight the option to be selected
		bool selected = ((current_select -1) == i);
		if (selected)
		{
			display.rect(x,y-1,125,10,1,1);
			display.text(options[i],x, y, 0);
		}

		//Show the options that are not highlighted
		else
		{
			display.text(options[i],x, y, 1);
		}
		y += 12;
	}
	display.show();
}

void OLEDDisplay::show_menu()
{
	show_options(main_menu.menu_length,main_menu.menu_options);
}
void OLEDDisplay::show_network_setting()
{
	show_options(network_menu.menu_length, network_menu.menu_options);
}

void OLEDDisplay::change_co2_setting(int co2_setting)
{
	int x = 0;
	std::string co2_str = std::to_string(co2_setting);

	//Format the spacing of co2 setting display
	if (co2_setting < 99)
	{
		x = 62;
	}
	else if (co2_setting < 999)
	{
		x = 55;
	}
	else
	{
		x = 50;
	}
	display.text(co2_str.c_str(),x, 14,1);

	//show the symbol
	display.text("+", 90, 4, 1);
	display.text(">", 90, 14, 1);
	display.text("-", 30, 4, 1);
	display.text("<", 30, 14, 1);

	//Press encoder to save
	display.text("Save: press ", 0, 30,1);
	display.text("encoder", 49, 40,1);
	display.text("Exit: press SW0", 0, 50,1);
	display.show();
}


void OLEDDisplay::show_data(float co2_level, float temperature, float humidity, float pressure, float co2_setting)
{
	display.fill(0);
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "CO2: %.1f ppm", co2_level);
	display.text(buffer, 0, 0, 1);

	snprintf(buffer, sizeof(buffer), "Temp: %.1f C", temperature);
	display.text(buffer, 0, 12, 1);

	snprintf(buffer, sizeof(buffer), "Hum: %.1f ", humidity);
	display.text(buffer, 0, 24, 1);

	snprintf(buffer, sizeof(buffer), "Press: %.1f Pa", pressure);
	display.text(buffer, 0, 36, 1);

	snprintf(buffer, sizeof(buffer), "CO2 set: %.1f ", co2_setting);
	display.text(buffer, 0, 48, 1);

	display.show();
}


void OLEDDisplay::increment_menu_select(UIEvent current_state)
{
	int option_length;
	if (current_state == UIEvent::MENU)
	{
		option_length = main_menu.menu_length;
	}
	else if (current_state == UIEvent::NETWORK)
	{
		option_length = network_menu.menu_length;
	}
	if (current_select < option_length)
	{
		current_select++;
		clear();
	}
}

void OLEDDisplay::decrement_menu_select()
{
	if (current_select > 1)
	{
		current_select--;
		clear();
	}
}


void OLEDDisplay::clear_co2_display()
{
	display.rect(40,14,45,10,0,true);
}


int OLEDDisplay::get_current_select() const
{
	return current_select;
}


