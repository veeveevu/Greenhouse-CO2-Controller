//
// Created by Anh Huynh on 23.9.2026.
//

#include "CO2Valve.h"
CO2Valve::CO2Valve()
	: co2_valve(27,true,true, true)
{};

void CO2Valve::open()
{
	co2_valve.write(true);
}

void CO2Valve::close()
{
	co2_valve.write(false);
}




