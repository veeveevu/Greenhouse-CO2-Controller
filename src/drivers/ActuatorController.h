//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_ACTUATORCONTROLLER_H
#define GREENHOUSE_ACTUATORCONTROLLER_H
#include "GPIOPin.h"
#include "ModbusRegister.h"

class ActuatorController {
public:
    ActuatorController(const std::shared_ptr<ModbusClient>& client);
    void fan_set_power(float power);
    void fan_turn_off();
    float fan_get_power() const;

    void valve_open();
    void valve_close();



private:
    ModbusRegister ventilation_fan;
    GPIOPin injection_valve;
    float current_fan_power = 0.0;
};


#endif //GREENHOUSE_ACTUATORCONTROLLER_H
