//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_GENERICTASKHANDLER_H
#define GREENHOUSE_GENERICTASKHANDLER_H


#include "ActuatorController.h"
#include "ModbusClient.h"
#include "ActuatorController.h"


class ControllerTask
{
public:
    ControllerTask(const std::shared_ptr<ActuatorController>& actuator, QueueHandle_t sensor_queue, QueueHandle_t setpoint_queue);
    void start();

private:
    static void task_entry(void* param);
    void run();

    std::shared_ptr<ActuatorController> controller;
    QueueHandle_t sensor_queue;
    QueueHandle_t setpoint_queue;
};

#endif //GREENHOUSE_GENERICTASKHANDLER_H