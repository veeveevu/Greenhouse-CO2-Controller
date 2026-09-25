//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_SENSORDATAHANDLER_H
#define GREENHOUSE_SENSORDATAHANDLER_H
#include "ModbusRegister.h"
#include "PressureSensor.h"

struct sensorData {
    float co2_ppm = 0;
    float temp_celsius = 0;
    float humidity_percent = 0;
    float pressure_pa = 0;
    float co2_setting = 1500;
    uint16_t fan_pulse_counter = 0;
    bool is_fan_running = false;
};

class SensorDataHandler {
public:
    explicit SensorDataHandler(const std::shared_ptr<ModbusClient>& client);
    void sensors_read();
    sensorData return_sensor_data() const;

private:
    ModbusRegister co2_sensor;
    ModbusRegister temp_sensor;
    ModbusRegister humidity_sensor;
    ModbusRegister fan_counter_sensor;
    //PressureSensor pressure_sensor;

    uint8_t fan_zero_reads = 0;
    sensorData data;
};


#endif //GREENHOUSE_SENSORDATAHANDLER_H
