//
// Created by Anh Huynh on 27.9.2026.
//

#include "SystemStorage.h"

SystemStorage::SystemStorage()
{

}

SensorReading SystemStorage::get_data()
{
	return data;
}

void SystemStorage::update_data(SensorReading new_data)
{
	data.co2_level_ppm = new_data.co2_level_ppm;
	data.temp_celsius = new_data.temp_celsius;
	data.humidity_percent = new_data.humidity_percent;
	data.pressure_pa = new_data.pressure_pa;

	data.fan_pulse_counter = new_data.fan_pulse_counter;
	data.is_fan_running = new_data.is_fan_running;
}

void SystemStorage::set_co2_point(int new_co2_point)
{
	data.co2_set_point = new_co2_point;
}




