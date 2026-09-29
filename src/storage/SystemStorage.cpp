//
// Created by Anh Huynh on 27.9.2026.
//
#include "FreeRTOS.h"
#include "SystemStorage.h"
#include "semphr.h"

SystemStorage::SystemStorage()
{
	mutex = xSemaphoreCreateMutex();
}

SensorReading SystemStorage::get_data()
{
	SensorReading data_copy;
	if (xSemaphoreTake(mutex,portMAX_DELAY) == pdTRUE)
	{
		data_copy = data;
		xSemaphoreGive(mutex);
	}
	return data_copy;

}

void SystemStorage::update_data(SensorReading new_data)
{
	if (xSemaphoreTake(mutex,portMAX_DELAY) == pdTRUE)
	{
		data.co2_level_ppm = new_data.co2_level_ppm;
		data.temp_celsius = new_data.temp_celsius;
		data.humidity_percent = new_data.humidity_percent;
		data.pressure_pa = new_data.pressure_pa;
		data.fan_pulse_counter = new_data.fan_pulse_counter;
		data.is_fan_running = new_data.is_fan_running;
		xSemaphoreGive(mutex);
	}
}

void SystemStorage::set_co2_point(int new_co2_point)
{
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		data.co2_set_point = new_co2_point;
		xSemaphoreGive(mutex);
	}
}





