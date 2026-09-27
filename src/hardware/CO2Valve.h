//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_CO2VALVE_H
#define GREENHOUSE_CO2VALVE_H
#include "GPIOPin.h"


class CO2Valve
{
	public:
		CO2Valve();
		void open();
		void close();
	private:
		GPIOPin co2_valve;
};


#endif //GREENHOUSE_CO2VALVE_H