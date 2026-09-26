//
// Created by Anh Huynh on 23.9.2026.
//

#include "GenericTaskHandler.h"
#include "data_structs.h"

#include "SensorDataHandler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

ControllerTask::ControllerTask(const std::shared_ptr<ActuatorController>& actuator, QueueHandle_t sensor_queue,
                               QueueHandle_t setpoint_queue, QueueHandle_t actuator_state_queue)
    : controller(actuator), sensor_queue(sensor_queue), setpoint_queue(setpoint_queue),
      actuator_state_queue(actuator_state_queue) {}

void ControllerTask::start() {
    xTaskCreate(task_entry, "Control Valve and Fan", 1024, this, tskIDLE_PRIORITY + 2, nullptr);
}

void ControllerTask::task_entry(void* param) {
    auto* self = static_cast<ControllerTask*>(param);
    self->run();
}

void ControllerTask::run() {
    SensorReading data;
    Co2Setting new_setting;

    while (true) {
        //check setpoint from setpoint_queue -> new -> set co2_setpoint in struct
        if (xQueueReceive(setpoint_queue, &new_setting, 0) == pdPASS) {
            co2_setpoint = new_setting.co2_setpoint;
            if (co2_setpoint > 1500) {co2_setpoint = 1500;}
            if (co2_setpoint < 0) {co2_setpoint = 0;}
        }

        //take newest sensor data from sensor_queue (dung xQueuePeek de khong xoa data sau khi doc)
        if (xQueuePeek(sensor_queue, &data, pdMS_TO_TICKS(1000)) == pdPASS) {
            handle_co2(data);
        }

        //write actuator state in struct
        bool valve_open = false;
        if (valve_state == ValveState::OPEN)
        {
            valve_open = true;
        }
        ActuatorState state{
            .fan_power_percent = controller->fan_get_power(),
            .is_valve_open = valve_open
        };
        xQueueOverwrite(actuator_state_queue, &state);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ControllerTask::handle_co2(const SensorReading& data) {
    TickType_t now = xTaskGetTickCount();

    //co2 > 2000 -> close valve -> max fan
    if (data.co2_level_ppm > CO2_SAFETY_LIMIT) {
        safety_fan_mode = true;
        controller->valve_close();
        controller->fan_set_power(100.0);
        valve_state = ValveState::CLOSED;
        return;
    }

    if (safety_fan_mode) {
        controller->fan_set_power(100.0);

        if (data.co2_level_ppm <= co2_setpoint) {
            controller->fan_turn_off();
            safety_fan_mode = false;
        }
        return;
    }

    //1500 < co2 < 2000 -> do nothing (turn off the fan if the fan is running?)
    controller->fan_turn_off();

    //!! nhớ là change setpoint phải dưới upper limit là 1500

    switch (valve_state) {
    case ValveState::CLOSED:
        if (data.co2_level_ppm < co2_setpoint) {
            controller->valve_open();
            valve_state = ValveState::OPEN;
            time_since_state_start = now;
        }
        break;

    case ValveState::OPEN:
        if (now - time_since_state_start >= pdMS_TO_TICKS(2000)) {
            controller->valve_close();
            valve_state = ValveState::WAITING;
            time_since_state_start = now;
        }
        break;

    case ValveState::WAITING:
        if (now - time_since_state_start >= pdMS_TO_TICKS(30000)) {
            valve_state = ValveState::CLOSED;
        }
        break;
    }
}
