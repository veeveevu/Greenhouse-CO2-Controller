//
// Created by Anh Huynh on 23.9.2026.
//

#include "GenericTaskHandler.h"

#include "SensorDataHandler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

ControllerTask::ControllerTask(const std::shared_ptr<ActuatorController>& actuator, QueueHandle_t sensor_queue,
                               QueueHandle_t setpoint_queue)
    : controller(actuator), sensor_queue(sensor_queue), setpoint_queue(setpoint_queue) {}

void ControllerTask::start() {
    xTaskCreate(task_entry, "Control Valve and Fan", 1024, this, tskIDLE_PRIORITY + 2, nullptr);
}


void ControllerTask::task_entry(void* param) {

    auto* self = static_cast<ControllerTask*>(param);
    self->run();
}
void ControllerTask::run() {
    sensorData data;

    while (true) {
        //check setpoint new?
        float new_setpoint;
        if (xQueueReceive(setpoint_queue, &new_setpoint, 0) == pdPASS) {

        }
    }

}
