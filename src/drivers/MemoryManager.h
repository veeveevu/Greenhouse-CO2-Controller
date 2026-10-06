//
// Created by Anh Huynh on 23.9.2026.
//

#ifndef GREENHOUSE_MEMORYMANAGER_H
#define GREENHOUSE_MEMORYMANAGER_H
#include <vector>

#include "data_structs.h"
#include "EEPROM.h"
#define PAGE_SIZE 64

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
		std::vector<NetworkSetting> known_networks;
		int co2_setting_addr = 0;
		int default_payload = PAGE_SIZE;
		int network_setting_addr = co2_setting_addr + PAGE_SIZE + 1;
		int known_network_start_addr = network_setting_addr + PAGE_SIZE + 1;
		int known_network_addr_tracker = known_network_start_addr;
		int known_network_end_addr = known_network_start_addr + (PAGE_SIZE + 1)* 4; //Save maximum 4 known network
};


#endif //GREENHOUSE_MEMORYMANAGER_H