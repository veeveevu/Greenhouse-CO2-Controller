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
		data = new_data;
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





