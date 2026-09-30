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
		int16_t read_pressure_adc () const;
		float   read_pressure_pa();

	private:
		std::shared_ptr<PicoI2C> i2c;
		float pressure_value;
		float scale_factor = 240;
		float correction_factor = 60.0;
		uint8_t slave_address = 0x40;
};


#endif //GREENHOUSE_PRESSURESENSOR_H