//
// Created by Anh Huynh on 27.9.2026.
//

#ifndef GREENHOUSE_SYSTEMSTORAGE_H
#define GREENHOUSE_SYSTEMSTORAGE_H
#include "data_structs.h"
#include "semphr.h"
#include "UIEvent.h"


class SystemStorage
{
	public:
		SystemStorage();
		SensorReading get_data();
		void update_data(SensorReading new_data);
		void update_fan_speed(float new_fan_speed);
		void update_network(const char* ssid_input, const char* pwd_input);

		NetworkSetting get_network_settings() const;

		void update_wifi_status(WiFiStatus new_status);
		WiFiStatus get_wifi_status();


		void           set_co2_point(int new_co2_point);
		void           factory_reset();

		bool data_available_to_read() const;

	private:
		SensorReading data;
		SemaphoreHandle_t mutex;
		bool data_available = false;
		NetworkSetting network_setting;
		WiFiStatus wifi_status = WiFiStatus::IDLE;
};


#endif //GREENHOUSE_SYSTEMSTORAGE_H