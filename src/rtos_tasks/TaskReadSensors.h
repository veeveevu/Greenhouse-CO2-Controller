//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_TASKREADSENSORS_H
#define GREENHOUSE_TASKREADSENSORS_H


#include "ModbusClient.h"
#include "ParentTask.h"
#include "SensorDataHandler.h"
#include "storage/SystemStorage.h"

#define READING_PERIOD_MS 5000


class SensorTask : public ParentTask
{
public:
    SensorTask(const std::shared_ptr<ModbusClient>& client, QueueHandle_t sensor_queue, SystemStorage &storage, std::shared_ptr<PicoI2C> i2c);


private:
    void task_runner() override;

    SensorDataHandler handler;
    QueueHandle_t sensor_queue;
		PressureSensor pressure_sensor_;
	SystemStorage &storage;
};


#endif //GREENHOUSE_TASKREADSENSORS_H