//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_SENSORDATAHANDLER_H
#define GREENHOUSE_SENSORDATAHANDLER_H
#include "ModbusRegister.h"
#include "PressureSensor.h"

struct sensorData
{
	double co2_level = 0;
	double temp = 0;
	double humidity = 0;
	double pressure_sensor = 0;
	double co2_setting = 1500;
};

class SensorDataHandler
{
	public:
		explicit SensorDataHandler(const std::shared_ptr<ModbusClient> &client);
		void sensors_read();
		sensorData return_sensor_data() const;

	private:
		ModbusRegister co2_sensor;
		ModbusRegister temp_sensor;
		ModbusRegister humidity_sensor;
		PressureSensor pressure_sensor;

		sensorData data;
};


#endif //GREENHOUSE_SENSORDATAHANDLER_H