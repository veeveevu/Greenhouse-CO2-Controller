//
// Created by Anh Huynh on 23.9.2026.
//

#include "OLEDDisplay.h"

OLEDDisplay::OLEDDisplay(const std::shared_ptr<PicoI2C> &i2c_1) : oled_i2c(i2c_1), display(oled_i2c)
{
}

void OLEDDisplay::clear() {
	display.fill(0);
	display.show();
}
void OLEDDisplay::show_options(int option_length, std::vector<std::string> options, int &x, int &y)
{

	for ( int i = 0; i < option_length; i++)
	{
		//Highlight the option to be selected
		bool selected = ((current_select -1) == i);
		if (selected)
		{
			display.rect(x,y-1,125,10,1,true);
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
	int x = 0;
	int y = 1;
	show_options(main_menu.menu_length,main_menu.menu_options,x,y);
}
void OLEDDisplay::show_network_connect_options()
{
	int x = 0;
	int y = 45;
	show_options(network_menu.menu_length, network_menu.menu_options, x, y);
}

void OLEDDisplay::change_co2_setting(const char *co2_input, int current_set_point)
{
	//Input space
	std::string current_co2_point = "Current: " + std::to_string(current_set_point);
	display.text(current_co2_point.c_str(),0,0,1);

	display.text("Enter new: ", 0, 10,1);
	display.rect(0,20,125,15,1,false);

	//Show input
	display.rect(1,21,120,13,0,true);
	display.text(co2_input, 5, 23,1);

	//Instructions
	display.text("Exit: press SW0", 0, 50,1);
	display.show();
}



void OLEDDisplay::confirm_co2_setting(int new_co2_setting)
{
	clear();
	display.text("Saved successfully!",0,12,1);
	display.text("New set point: ", 0, 22, 1);
	std::string new_co2 = std::to_string(new_co2_setting);
	display.text(new_co2.c_str(), 0, 32, 1);
	display.show();
}

void OLEDDisplay::no_new_setting()
{
	clear();
	display.text("No new setting. Returning...",0,30,1);
	display.show();
}

void OLEDDisplay::show_data(float co2_level, float temperature, float humidity, float pressure, int co2_setting)
{
	display.fill(0);
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "CO2: %.1f ppm", co2_level);
	display.text(buffer, 0, 0, 1);

	snprintf(buffer, sizeof(buffer), "Temp: %.1f C", temperature);
	display.text(buffer, 0, 10, 1);

	snprintf(buffer, sizeof(buffer), "Hum: %.1f ", humidity);
	display.text(buffer, 0, 20, 1);

	snprintf(buffer, sizeof(buffer), "Press: %.1f Pa", pressure);
	display.text(buffer, 0, 30, 1);

	snprintf(buffer, sizeof(buffer), "CO2 set: %d ", co2_setting);
	display.text(buffer, 0, 40, 1);

	display.text("Exit: press SW0", 0, 55,1);
	display.show();
}

void OLEDDisplay::show_network(const char* ssid, const char* status)
{
	char ssid_show[20];
	char status_show[20];

	display.rect(0,0,124,25,1,false);
	snprintf(ssid_show, 20, "<%s>", ssid);
	snprintf(status_show, 20, "%s", status);

	display.text(ssid_show,0,5,1);
	display.text(status_show, 0, 15, 1);

	display.text("Change network: ", 0, 30);
	show_network_connect_options();
}

void OLEDDisplay::connect_new_network(NetworkParam current_param, const char* ssid_input, const char* pwd_input, const char* input_buffer)
{
	display.text("SSID", 0, 0,1);
	display.text("Password:", 0, 30,1);
	if (current_param != NetworkParam::DONE)
	{
		int input_y;
		if (current_param == NetworkParam::SSID)
		{
			input_y = 12;
			display.rect(0, 51, 60, 13, 1, false);
			display.text("Connect", 1, 52, 1);
		}
		else
		{
			input_y = 41;

			display.rect(0,11,125,15,0,true);
			display.text(ssid_input, 0, 10, 1);

			//Connect button
			display.rect(0, 51, 60, 13, 1, true);
			display.text("Connect", 1, 52, 0);

		}
		display.rect(0,input_y, 125,12,1,false);
		display.rect(1, input_y+ 1, 120,8,0,true);
		display.text(input_buffer , 1, input_y, 1);

	}
	else
	{
		display.text(ssid_input, 0, 10, 1);
		display.text(pwd_input, 0, 40, 1);
	}
	display.show();
}

void OLEDDisplay::connecting_animation(const char* ssid)
{
	clear();
	display.text(ssid, 0, 15,1);
	display.text("Connecting ", 0,25,1);
	for (int i = 1; i <= 3; i++ )
	{
		display.rect(80 + i*2,30,2,2,1,true);
		display.show();
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void OLEDDisplay::connect_successfully(const char* ssid)
{
	clear();
	display.text(ssid, 0, 15,1);
	display.text("Connected", 0, 25, 1);
	display.show();
	vTaskDelay(pdMS_TO_TICKS(1000));
}

void OLEDDisplay::connect_failed()
{
	display.text("Failed to connect", 0, 20, 1);
	display.show();
}

void OLEDDisplay::increment_menu_select(UIEvent current_state)
{
	int option_length = 0;
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

void OLEDDisplay::reset_menu_select()
{
	current_select = 1;
}


void OLEDDisplay::clear_co2_display()
{
	display.rect(40,14,45,10,0,true);
}


int OLEDDisplay::get_current_select() const
{
	return current_select;
}


void OLEDDisplay::show_reset(const char *input)
{
	display.text("Type \"reset\" to confirm", 0,0,1);
	display.rect(0,20,125,15,1,false);
	display.rect(1,21,120,13,0,true);
	display.text(input, 5, 23,1);
	display.text("Exit: press SW0", 0, 50,1);
	display.show();
}

void OLEDDisplay::confirm_reset()
{
	clear();
	display.text("Factory resetting...",0,10,1);
	display.text("Disconnecting WiFi", 0,20,1);
	vTaskDelay(pdMS_TO_TICKS(1000));
	display.show();
}

void OLEDDisplay::wrong_command()
{
	clear();
	display.text("Wrong command",0,10,1);
	display.text("Return to ", 0,20,1);
	display.text("main menu", 0,30,1);
	display.show();
}

