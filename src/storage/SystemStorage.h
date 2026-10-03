//
// Created by Anh Huynh on 27.9.2026.
//

#ifndef GREENHOUSE_SYSTEMSTORAGE_H
#define GREENHOUSE_SYSTEMSTORAGE_H
#include "data_structs.h"
#include "semphr.h"


class SystemStorage
{
	public:
		SystemStorage();
		SensorReading get_data();
		void update_data(SensorReading new_data);

		void update_fan_speed(float new_fan_speed);

		void set_co2_point(int new_co2_point);

	private:
		SensorReading data;
		SemaphoreHandle_t mutex;
};


#endif //GREENHOUSE_SYSTEMSTORAGE_H