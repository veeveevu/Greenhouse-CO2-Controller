//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_MEMORYMANAGER_H
#define GREENHOUSE_MEMORYMANAGER_H
#include "EEPROM.h"


class MemoryManager
{
	public:
		MemoryManager(std::shared_ptr<PicoI2C> i2c_0) : eeprom(EEPROM(i2c_0)) {};
		void save_new_co2_setting(int new_co2_setting);

		void reset_network_setting();

		void save_network_setting( char* ssid, char* pwd);
		void read_co2_setting(uint8_t *read_co2_dest);
		int read_network_setting(char* ssid, char* pwd);

	private:
		EEPROM eeprom;
		int co2_setting_addr = 0;
		int default_payload = 64;
		int network_setting_addr = 65;
};


#endif //GREENHOUSE_MEMORYMANAGER_H