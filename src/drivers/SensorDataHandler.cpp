//
// Created by Anh Huynh on 23.9.2026.
//

#include "SensorDataHandler.h"

SensorDataHandler::SensorDataHandler(const std::shared_ptr<ModbusClient>& client)
    : co2_sensor(client, 240, 0x0100, false), //using 16-bit signed int up to 32000 ppm
      temp_sensor(client, 241, 0x0101, false),
      humidity_sensor(client, 241, 0x0100, false),
      fan_counter_sensor(client, 1, 4, false)
/*
	Note: server_address = modbus address
	reg_addr = register address in hexadecimal, found in user guide
	holding_register = true when it can be read and written
	holding_register = false when it is read-only, read only or not can be found in user guide
*/
{
    pressure_sensor = PressureSensor();
}

void SensorDataHandler::sensors_read() {

    uint16_t raw_co2 = co2_sensor.read();
    auto co2_value = static_cast<int16_t>(raw_co2);
    data.co2_ppm = static_cast<float>(co2_value);

    uint16_t raw_temp = temp_sensor.read();
    auto temp_value = static_cast<int16_t>(raw_temp);
    data.temp_celsius = static_cast<float>(temp_value) / 10.0f;

    uint16_t raw_humidity = humidity_sensor.read();
    auto humidity_value = static_cast<int16_t>(raw_humidity);
    data.humidity_percent = static_cast<float>(humidity_value) / 10.0f;

    data.fan_pulse = fan_counter_sensor.read();
}

sensorData SensorDataHandler::return_sensor_data() const {
    return data;
}
