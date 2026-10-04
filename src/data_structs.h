//
// Created by vuhav on 26/09/2026.
//

#ifndef GREENHOUSE_DATA_STRUCTS_H
#define GREENHOUSE_DATA_STRUCTS_H
#include <cstdint>

//struct only for sensor_task
struct SensorReading {
    float co2_level_ppm = 0;
    float temp_celsius = 0;
    float humidity_percent = 0;
    float pressure_pa = 0;

	int co2_set_point;
    uint16_t fan_pulse_counter = 0;
	float fan_speed = 0;
    bool is_fan_running = false;

	bool operator==(const SensorReading &other) const
	{
		return (co2_level_ppm == other.co2_level_ppm) &&
			   (temp_celsius == other.temp_celsius) &&
			   (humidity_percent == other.humidity_percent) &&
			   (pressure_pa == other.pressure_pa) &&
			   (co2_set_point == other.co2_set_point);
	}

	bool operator!=(const SensorReading &other) const
	{
		return !(*this == other);
	}
};

struct NetworkSetting
{
	char ssid[20];
	char pwd[20];
};



#endif //GREENHOUSE_DATA_STRUCTS_H