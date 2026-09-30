//
// Created by Anh Huynh on 23.9.2026.
//

#include "PressureSensor.h"

int16_t PressureSensor::read_pressure_adc() const
{
	uint8_t start_cmd[1] = {0xF1};
	i2c->write(slave_address,start_cmd,1);

	vTaskDelay(pdMS_TO_TICKS(20));
	uint8_t read_adc[2];
	i2c->read(slave_address,read_adc,2);

	int16_t raw_adc = static_cast<int16_t>(read_adc[0] << 8 | read_adc[1]);
	return raw_adc;
}

float PressureSensor::read_pressure_pa()
{
	int16_t adc = read_pressure_adc();
	pressure_value = adc * correction_factor / scale_factor;
	return pressure_value;
}



