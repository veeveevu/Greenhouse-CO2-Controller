#include "ActuatorController.h"

ActuatorController::ActuatorController(const std::shared_ptr<ModbusClient> &client)
	: ventilation_fan(client, 1, 0, true),
	injection_valve(27,false,false, false)
{}

void ActuatorController::fan_set_power(float power)
{
	if (power < 0.0) power = 0.0;
	if (power > 100.0) power = 100.0;

	//Doc: 40001 - AO1 value - Signed 16 receive value 0…1000 ~ 0.0…100.0 (%) (Max la 1000)

	auto registered_power = static_cast<uint16_t>((power / 100.0) * 1000);

	ventilation_fan.write(registered_power);
	current_fan_power = power;
}

void ActuatorController::fan_turn_off()
{
	fan_set_power(0.0);
}

float ActuatorController::fan_get_power() const {
	return current_fan_power;
}

void ActuatorController::valve_close()
{
	//printf("========== VALVE CLOSED ==========\n");
	injection_valve.write(false);
}

void ActuatorController::valve_open()
{
	//printf("========== VALVE OPEN ==========\n");
	injection_valve.write(true);
}





