//
// Created by Anh Huynh on 23.9.2026.
//

#include "ActuatorController.h"

ActuatorController::ActuatorController(const std::shared_ptr<ModbusClient> &client)
	: ventilation_fan(client, ), //Fill in the addresses
	injection_valve(27,true,false, false)
{}

void ActuatorController::fan_set_power(double power)
{
	//power is percentage (0-100% power of the fan), so we need to convert it to scale 0-10

	//Set the fan's power using modbus, 0V means turn off, 10V means running at 100% power
}

void ActuatorController::fan_turn_off()
{
	//Set fan power to be 0 via modbus
}

void ActuatorController::valve_close()
{
	injection_valve.write(false);
}

void ActuatorController::valve_open()
{
	injection_valve.write(true);
}





