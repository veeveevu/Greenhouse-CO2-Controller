//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_SENSORDATAHANDLER_H
#define GREENHOUSE_SENSORDATAHANDLER_H
#include "ModbusRegister.h"
#include "PressureSensor.h"
#include "data_structs.h"


class SensorDataHandler {
public:
    explicit SensorDataHandler(const std::shared_ptr<ModbusClient>& client, PressureSensor &pressure_sensor);
    void sensors_read();
    SensorReading return_sensor_data() const;

private:
    ModbusRegister co2_sensor;
    ModbusRegister temp_sensor;
    ModbusRegister humidity_sensor;
    ModbusRegister fan_counter_sensor;
    PressureSensor &pressure_sensor;

    uint8_t fan_zero_reads = 0;
    SensorReading data;
};


#endif //GREENHOUSE_SENSORDATAHANDLER_H
