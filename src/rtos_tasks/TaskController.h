//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_GENERICTASKHANDLER_H
#define GREENHOUSE_GENERICTASKHANDLER_H


#include "ActuatorController.h"
#include "ModbusClient.h"

#include "ActuatorController.h"
#include "data_structs.h"
#define CO2_SAFETY_LIMIT 2000
#define CO2_UPPER_LIMIT 1500

enum class ValveState {
    CLOSED, //van đóng
    OPEN, //van mở (max 2s)
    WAITING //van đóng nhưng chờ 30s
};

class ControllerTask {
public:
    ControllerTask(const std::shared_ptr<ActuatorController>& actuator, QueueHandle_t sensor_queue,
                   QueueHandle_t setpoint_queue, QueueHandle_t actuator_state_queue);
    void start();

private:
    static void task_entry(void* param);
    void run();

    std::shared_ptr<ActuatorController> controller;

    void handle_co2(const SensorReading& data);

    QueueHandle_t sensor_queue;
    QueueHandle_t setpoint_queue;
    QueueHandle_t actuator_state_queue;

    ValveState valve_state = ValveState::CLOSED;

    float co2_setpoint = 1500.0;
    TickType_t time_since_state_start = 0;

    bool safety_fan_mode = false;
};

#endif //GREENHOUSE_GENERICTASKHANDLER_H
