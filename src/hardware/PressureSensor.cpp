//
// Created by Anh Huynh on 23.9.2026.
//

#include "PressureSensor.h"

uint16_t PressureSensor::read_pressure_adc() const
{
	uint8_t start_cmd[1] = {0xF1};
	i2c->write(slave_address,start_cmd,1);

	uint8_t read_adc[2];
	i2c->read(slave_address,read_adc,2);

	uint16_t raw_adc = (uint16_t) (read_adc[0] << 8 | read_adc[1]);
	return raw_adc;
}

void PressureSensor::read_pressure_pa()
{
	uint16_t adc = read_pressure_adc();
	pressure_value = adc * correction_factor;
}

double PressureSensor::get_pressure_value()
{
	return pressure_value;
}


