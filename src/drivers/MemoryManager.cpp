//
// Created by Anh Huynh on 23.9.2026.
//

#include "MemoryManager.h"

#include <cstring>

void MemoryManager::read_co2_setting(uint8_t *read_co2_dest)
{
	uint8_t dest[64];
	eeprom.read(dest,default_payload,co2_setting_addr);
	std::memcpy(read_co2_dest,dest,64);

}

void MemoryManager::read_network_setting(uint8_t *dest)
{
	eeprom.read(dest,default_payload,network_setting_addr);
}

void MemoryManager::save_new_co2_setting(int new_co2_setting)
{
	uint8_t save_data[sizeof(double)]; //8 bytes
	std::memcpy(save_data,&new_co2_setting,sizeof(double));

	eeprom.write(save_data,default_payload,co2_setting_addr);
}




