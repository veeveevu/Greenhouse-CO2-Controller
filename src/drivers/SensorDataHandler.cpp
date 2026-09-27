#include "SensorDataHandler.h"

#include <iostream>

SensorDataHandler::SensorDataHandler(const std::shared_ptr<ModbusClient>& client, PressureSensor &pressure_sensor)
    : co2_sensor(client, 240, 256, true), //using 16-bit signed int up to 32000 ppm
      temp_sensor(client, 241, 257, true),
      humidity_sensor(client, 241, 256, true),
      fan_counter_sensor(client, 1, 4, false),
		pressure_sensor(pressure_sensor)
{
}

//read sensors
void SensorDataHandler::sensors_read() {

    uint16_t raw_co2 = co2_sensor.read();
    auto co2_value = static_cast<int16_t>(raw_co2);
    data.co2_level_ppm = static_cast<float>(co2_value);

    uint16_t raw_temp = temp_sensor.read();
    auto temp_value = static_cast<int16_t>(raw_temp);
    data.temp_celsius = static_cast<float>(temp_value) / 10.0f;

    uint16_t raw_humidity = humidity_sensor.read();
    auto humidity_value = static_cast<int16_t>(raw_humidity);
    data.humidity_percent = static_cast<float>(humidity_value) / 10.0f;

	double pressure_value = pressure_sensor.read_pressure_pa();
	data.pressure_pa = static_cast<float>(pressure_value) / 10.0f;

    data.fan_pulse_counter = fan_counter_sensor.read();

    //check if 2 times read 0 -> fan stop -> update is_fan_running
    if (data.fan_pulse_counter == 0) {
        if (fan_zero_reads < 2) {
            ++fan_zero_reads;
        } else {
            data.is_fan_running = false;
        }
    } else {
        fan_zero_reads = 0;
        data.is_fan_running = true;
    }


    std::cout << "CO2: " << data.co2_level_ppm << " ppm\n"
		    << "Temperature: " << data.temp_celsius << " C\n"
		    << "Humidity: " << data.humidity_percent << " %\n"
		    << "Pressure: " << data.pressure_pa << " Pa\n"
		    << "Fan pulse: " << data.fan_pulse_counter << '\n';
}

SensorReading SensorDataHandler::return_sensor_data() const {
    return data;
}
