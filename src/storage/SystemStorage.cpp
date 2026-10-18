//
// Created by Anh Huynh on 27.9.2026.
//
#include "FreeRTOS.h"
#include "SystemStorage.h"

#include <cstring>

#include "semphr.h"

SystemStorage::SystemStorage() :network_setting(), data()
{
	mutex = xSemaphoreCreateMutex();
}

SensorReading SystemStorage::get_data()
{
	SensorReading data_copy = {};
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
		data_available = true;
		xSemaphoreGive(mutex);
	}
}
void SystemStorage::update_fan_speed(float new_fan_speed)
{
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		data.fan_speed = new_fan_speed;
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

void SystemStorage::factory_reset()
{
	if (xSemaphoreTake(mutex,portMAX_DELAY) == pdTRUE)
	{
		data.co2_level_ppm = 0;
		data.temp_celsius = 0;
		data.humidity_percent = 0;
		data.pressure_pa = 0;
		data.fan_pulse_counter = 0;
		data.is_fan_running = false;
		data_available = false;
		data.co2_set_point = 1500;

		network_setting.ssid[0] = '\0';
		network_setting.pwd[0] = '\0';
		wifi_status = WiFiStatus::IDLE;
		xSemaphoreGive(mutex);
	}
}


bool SystemStorage::data_available_to_read() const
{
	bool available;
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		available = data_available;
		xSemaphoreGive(mutex);
	}
	return available;
}

void SystemStorage::update_network(const char *ssid_input, const char *pwd_input)
{
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		std::strcpy(network_setting.ssid, ssid_input);
		std::strcpy(network_setting.pwd, pwd_input);
		xSemaphoreGive(mutex);
	}
}


NetworkSetting SystemStorage::get_network_settings() const
{
	NetworkSetting network;
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		network = network_setting;
		xSemaphoreGive(mutex);
	}
	return network;
}

void SystemStorage::update_wifi_status(WiFiStatus new_status)
{
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		wifi_status = new_status;
		xSemaphoreGive(mutex);
	}
}

WiFiStatus SystemStorage:: get_wifi_status()
{
	WiFiStatus status;
	if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
	{
		status = wifi_status;
		xSemaphoreGive(mutex);
	}
	return status;
}




