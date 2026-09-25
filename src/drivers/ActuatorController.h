//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_ACTUATORCONTROLLER_H
#define GREENHOUSE_ACTUATORCONTROLLER_H
#include "GPIOPin.h"
#include "ModbusRegister.h"

class ActuatorController
{
	public:
		explicit ActuatorController(std::shared_ptr<ModbusClient>client);
		ActuatorController(const std::shared_ptr<ModbusClient>& client);
		void fan_set_power(double power);
		void fan_turn_off();
		void valve_open();
		void valve_close();

	private:
		ModbusRegister ventilation_fan_speed;
		GPIOPin injection_valve;
};


#endif //GREENHOUSE_ACTUATORCONTROLLER_H