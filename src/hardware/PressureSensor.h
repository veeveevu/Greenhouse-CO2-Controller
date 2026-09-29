//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_PRESSURESENSOR_H
#define GREENHOUSE_PRESSURESENSOR_H
#include <memory>

#include "PicoI2C.h"


class PressureSensor
{
	public:
		explicit PressureSensor(std::shared_ptr<PicoI2C> i2c_1) :i2c(i2c_1){};
		uint16_t read_pressure_adc () const;
		double   read_pressure_pa();

	private:
		std::shared_ptr<PicoI2C> i2c;
		double pressure_value;
		double correction_factor = 0.95;
		uint8_t slave_address = 0x40;
};


#endif //GREENHOUSE_PRESSURESENSOR_H