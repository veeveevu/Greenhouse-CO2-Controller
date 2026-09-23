//
// Created by Anh Huynh on 23.9.2026.
//

#include "SensorDataHandler.h"

SensorDataHandler::SensorDataHandler(const std::shared_ptr<ModbusClient> &client)
:	co2_sensor(client, 240, 0x0000, false), //Check documentation to find server addr, reg addr, hr
	temp_sensor(client, 241, 0x0002, false), //Check documentation to find server addr, reg addr, hr
	humidity_sensor(client, 241, 0x0000, false)
	//Note: server_address = modbus address
	//reg_addr = regiter address in hexadecimal, found in user guide
	//holding_register = true when it can be read and written
	//holding_register = false when it is read-only, read only or not can be found in user guide
{
	pressure_sensor = PressureSensor();
}

void SensorDataHandler::sensors_read()
{
	//Read data and save to the struct, for example
	data.co2_level = co2_sensor.read();
	data.temp = temp_sensor.read();
	data.humidity = humidity_sensor.read();

}

sensorData SensorDataHandler::return_sensor_data() const
{
	return data;
}


