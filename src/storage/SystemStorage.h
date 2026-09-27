//
// Created by Anh Huynh on 27.9.2026.
//

#ifndef GREENHOUSE_SYSTEMSTORAGE_H
#define GREENHOUSE_SYSTEMSTORAGE_H
#include "data_structs.h"


class SystemStorage
{
	public:
		SystemStorage();
		SensorReading get_data();
		SensorReading update_data(SensorReading new_data);
	private:
		SensorReading data;
};


#endif //GREENHOUSE_SYSTEMSTORAGE_H