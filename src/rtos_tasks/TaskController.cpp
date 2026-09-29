//
// Created by Anh Huynh on 23.9.2026.
//

#include "TaskController.h"
#include "data_structs.h"

#include "SensorDataHandler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

ControllerTask::ControllerTask(const std::shared_ptr<ActuatorController> &actuator, SystemStorage &storage)
	: ParentTask("Control Valve and Fan", 1024, tskIDLE_PRIORITY + 3),
	  controller(actuator),
	  storage(storage)
{
}


void ControllerTask::handle_co2(const SensorReading &data) {
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
	if (data.co2_level_ppm < 2000 && data.co2_level_ppm > data.co2_set_point)
	{
		controller->fan_turn_off();
	}

    //!! nhớ là change setpoint phải dưới upper limit là 1500
	switch (valve_state) {
    case ValveState::CLOSED:
        if (data.co2_level_ppm < co2_setpoint) {
            controller->valve_open();
            valve_state = ValveState::OPEN;
            time_since_state_start = xTaskGetTickCount();
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


void ControllerTask::task_runner() {

	while (true) {
		SensorReading data = storage.get_data();
		co2_setpoint = data.co2_set_point;
		handle_co2(data);

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}