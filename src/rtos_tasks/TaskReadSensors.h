//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_TASKREADSENSORS_H
#define GREENHOUSE_TASKREADSENSORS_H


#include "ModbusClient.h"
#include "SensorDataHandler.h"


class SensorTask
{
public:
    SensorTask(const std::shared_ptr<ModbusClient>& client, QueueHandle_t out_queue);
    void start();

private:
    static void task_entry(void* param);
    void run();

    SensorDataHandler handler;
    QueueHandle_t out_queue;
};


#endif //GREENHOUSE_TASKREADSENSORS_H