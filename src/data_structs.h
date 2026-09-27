//
// Created by vuhav on 26/09/2026.
//

#ifndef GREENHOUSE_DATA_STRUCTS_H
#define GREENHOUSE_DATA_STRUCTS_H

//struct only for sensor_task
struct SensorReading {
    float co2_level_ppm = 0;
    float temp_celsius = 0;
    float humidity_percent = 0;
    float pressure_pa = 0;

    uint16_t fan_pulse_counter = 0;
    bool is_fan_running = false;

};

//gui cai nay cho queue cua UI
struct ActuatorState {
    float fan_power_percent = 0;
    bool is_valve_open = false;
};

//struct that controller read, UI set
struct Co2Setting {
    float co2_setpoint = 1500;
};



#endif //GREENHOUSE_DATA_STRUCTS_H